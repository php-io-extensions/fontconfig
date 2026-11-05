#include "runtime.h"

/* The boxes key: one PHP object per native address and class, so a fromPointer() of another class at the
 * same address does not clear a live handle. */
typedef struct {
	void *ptr;
	zend_class_entry *ce;
} phpfc_box_key;

static zend_object *phpfc_create_object(zend_class_entry *ce)
{
	phpfc_handle *intern = zend_object_alloc(sizeof(phpfc_handle), ce);

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = ce->default_object_handlers;
	intern->ptr = NULL;
	intern->owners = 0;
	ZVAL_UNDEF(&intern->children);

	return &intern->std;
}

static void phpfc_forget(phpfc_handle *intern)
{
	phpfc_box_key key = { intern->ptr, intern->std.ce };

	if (intern->ptr != NULL
		&& zend_hash_str_find_ptr(&PHPFC_G(boxes), (const char *) &key, sizeof(key)) == &intern->std) {
		zend_hash_str_del(&PHPFC_G(boxes), (const char *) &key, sizeof(key));
	}
}

static void phpfc_free_object(zend_object *object)
{
	phpfc_handle *intern = phpfc_handle_from(object);

	phpfc_forget(intern);
	intern->ptr = NULL;
	zval_ptr_dtor(&intern->children);
	zend_object_std_dtor(object);
}

/* isset() and empty() on a record property ask the native record; a handle that holds NULL has none. */
static int phpfc_has_record(zend_object *obj, zend_string *name, int check_empty, void **cache_slot)
{
	zval rv, *value;
	int has;

	if (phpfc_handle_from(obj)->ptr == NULL) {
		return zend_std_has_property(obj, name, check_empty, cache_slot);
	}
	value = obj->handlers->read_property(obj, name, BP_VAR_IS, NULL, &rv);
	has = check_empty == ZEND_PROPERTY_NOT_EMPTY ? zend_is_true(value) : Z_TYPE_P(value) != IS_NULL;
	if (value == &rv) {
		zval_ptr_dtor(&rv);
	}
	return has;
}

/* var_dump() and print_r() show the native record, read field by field. */
static HashTable *phpfc_record_debug_info(zend_object *obj, int *is_temp)
{
	zend_property_info *info;
	HashTable *fields;

	if (phpfc_handle_from(obj)->ptr == NULL) {
		*is_temp = 0;
		return zend_std_get_properties(obj);
	}
	*is_temp = 1;
	fields = zend_new_array(zend_hash_num_elements(&obj->ce->properties_info));
	ZEND_HASH_MAP_FOREACH_PTR(&obj->ce->properties_info, info) {
		zval rv, copy, *value;

		if (info->flags & ZEND_ACC_STATIC) {
			continue;
		}
		value = obj->handlers->read_property(obj, info->name, BP_VAR_IS, NULL, &rv);
		ZVAL_COPY(&copy, value);
		if (value == &rv) {
			zval_ptr_dtor(&rv);
		}
		zend_hash_update(fields, info->name, &copy);
	} ZEND_HASH_FOREACH_END();

	return fields;
}

/* The GC sees the children a handle keeps alive. */
static HashTable *phpfc_get_gc(zend_object *obj, zval **table, int *n)
{
	zend_get_gc_buffer *buffer = zend_get_gc_buffer_create();
	zval *props_table;
	int props_count;
	HashTable *props = zend_std_get_gc(obj, &props_table, &props_count);

	for (int i = 0; i < props_count; i++) {
		zend_get_gc_buffer_add_zval(buffer, &props_table[i]);
	}
	zend_get_gc_buffer_add_zval(buffer, &phpfc_handle_from(obj)->children);
	zend_get_gc_buffer_use(buffer, table, n);

	return props;
}

void phpfc_handle_setup_in(zend_object_handlers *h, zend_class_entry *ce, zend_object_read_property_t read)
{
	memcpy(h, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	h->offset = XtOffsetOf(phpfc_handle, std);
	h->free_obj = phpfc_free_object;
	h->clone_obj = NULL;
	h->get_gc = phpfc_get_gc;
	if (read != NULL) {
		h->read_property = read;
		h->has_property = phpfc_has_record;
		h->get_debug_info = phpfc_record_debug_info;
	}
	ce->create_object = phpfc_create_object;
	ce->default_object_handlers = h;
	ce->ce_flags |= ZEND_ACC_NOT_SERIALIZABLE | ZEND_ACC_FINAL;
}

/* Drops the children whose native object was destroyed, so a long-lived parent does not grow. */
static void phpfc_prune_children(phpfc_handle *owner)
{
	zend_ulong index;
	zval *child;

	ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL(owner->children), index, child) {
		if (phpfc_handle_from(Z_OBJ_P(child))->ptr == NULL) {
			zend_hash_index_del(Z_ARRVAL(owner->children), index);
		}
	} ZEND_HASH_FOREACH_END();
}

void phpfc_box(zval *rv, void *ptr, zend_class_entry *ce, zend_object *parent)
{
	phpfc_box_key key = { ptr, ce };
	zend_object *existing;

	if (ptr == NULL) {
		ZVAL_NULL(rv);
		return;
	}
	existing = zend_hash_str_find_ptr(&PHPFC_G(boxes), (const char *) &key, sizeof(key));
	if (existing != NULL) {
		ZVAL_OBJ_COPY(rv, existing);
	} else {
		object_init_ex(rv, ce);
		phpfc_handle_from(Z_OBJ_P(rv))->ptr = ptr;
		zend_hash_str_update_ptr(&PHPFC_G(boxes), (const char *) &key, sizeof(key), Z_OBJ_P(rv));
	}

	if (parent != NULL) {
		phpfc_handle *owner = phpfc_handle_from(parent);
		zval child;

		if (Z_TYPE(owner->children) != IS_ARRAY) {
			array_init(&owner->children);
		}
		/* Keyed by the child's object handle, so a parent owns a child once however often it boxes it. */
		if (zend_hash_index_find(Z_ARRVAL(owner->children), Z_OBJ_P(rv)->handle) == NULL) {
			phpfc_prune_children(owner);
			ZVAL_OBJ_COPY(&child, Z_OBJ_P(rv));
			zend_hash_index_add_new(Z_ARRVAL(owner->children), Z_OBJ_P(rv)->handle, &child);
			phpfc_handle_from(Z_OBJ_P(rv))->owners++;
		}
	}
}

void *phpfc_handle_ptr(zval *zv)
{
	return phpfc_handle_from(Z_OBJ_P(zv))->ptr;
}

void *phpfc_live_pattern(zval *zv, uint32_t arg_num)
{
	void *ptr = phpfc_handle_ptr(zv);

	if (!PHPFC_PROBE_OFF("fontconfig-null-pattern") && ptr == NULL) {
		zend_argument_value_error(arg_num, "holds NULL: it was destroyed, or made from address 0"); /* binding:fontconfig-null-pattern */
	}
	return ptr;
}

void phpfc_release(zend_object *obj)
{
	phpfc_handle *intern = phpfc_handle_from(obj);
	zval children, *child;

	phpfc_forget(intern);
	intern->ptr = NULL;
	ZVAL_COPY_VALUE(&children, &intern->children);
	ZVAL_UNDEF(&intern->children);
	if (Z_TYPE(children) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(children), child) {
			phpfc_handle *owned = phpfc_handle_from(Z_OBJ_P(child));

			if (owned->owners > 0 && --owned->owners == 0) {
				phpfc_release(Z_OBJ_P(child));
			}
		} ZEND_HASH_FOREACH_END();
	}
	zval_ptr_dtor(&children);
}

#ifdef PHPFC_PROBES
bool phpfc_probe_off(const char *id)
{
	const char *off = getenv("PHPFC_PROBE_OFF");

	return off != NULL && strcmp(off, id) == 0;
}
#endif
