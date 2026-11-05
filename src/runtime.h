#ifndef PHPFC_RUNTIME_H
#define PHPFC_RUNTIME_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_fontconfig.h"
#include <fontconfig/fontconfig.h>

ZEND_BEGIN_MODULE_GLOBALS(fontconfig)
	HashTable boxes;               /* (native address, class) => zend_object*, live handles only, not refcounted */
ZEND_END_MODULE_GLOBALS(fontconfig)

ZEND_EXTERN_MODULE_GLOBALS(fontconfig)
#define PHPFC_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(fontconfig, v)

typedef struct {
	void *ptr;                     /* the native object; NULL once destroyed, or from fromPointer(0) */
	zval children;                 /* handles whose native object goes with this one: a set's patterns, a pattern's charset */
	uint32_t owners;               /* parents holding this handle in their children: two sorted sets share the config's patterns */
	zend_object std;
} phpfc_handle;

static zend_always_inline phpfc_handle *phpfc_handle_from(zend_object *obj)
{
	return (phpfc_handle *) ((char *) obj - XtOffsetOf(phpfc_handle, std));
}

/* Handle class setup: object layout, no clone, not serializable, final. read is the class's
 * read_property handler for the C record fields it exposes, or NULL. Each call site keeps its own
 * static handler table, so there is no limit on the number of handle classes. */
void phpfc_handle_setup_in(zend_object_handlers *h, zend_class_entry *ce, zend_object_read_property_t read);
#define phpfc_handle_setup(ce, read) do { static zend_object_handlers phpfc_h_; phpfc_handle_setup_in(&phpfc_h_, (ce), (read)); } while (0)
/* The PHP object holding ptr, or a new one of class ce; NULL ptr → null. The object becomes a child of
 * parent (NULL for none); it is released when its last parent is, or by its own destroy call. */
void phpfc_box(zval *rv, void *ptr, zend_class_entry *ce, zend_object *parent);
/* The native pointer a handle holds (NULL once destroyed, or from fromPointer(0)). */
void *phpfc_handle_ptr(zval *zv);
/* The FcPattern of parameter arg_num for a call that dereferences it: NULL, with a ValueError, when the
 * handle holds NULL. */
void *phpfc_live_pattern(zval *zv, uint32_t arg_num);
/* The native object is gone: pointer cleared, identity entry dropped, children released. */
void phpfc_release(zend_object *obj);

#ifdef PHPFC_PROBES
bool phpfc_probe_off(const char *id);
# define PHPFC_PROBE_OFF(id) phpfc_probe_off(id)
#else
# define PHPFC_PROBE_OFF(id) 0
#endif

/* __construct (private, empty), pointer() and fromPointer() for a handle class. fromPointer(0) answers a
 * handle that holds NULL. Any other address is trusted. */
#define PHPFC_HANDLE_METHODS(cls) \
	ZEND_METHOD(cls, __construct) { ZEND_PARSE_PARAMETERS_NONE(); } \
	ZEND_METHOD(cls, pointer) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_LONG((zend_long) (uintptr_t) phpfc_handle_ptr(ZEND_THIS)); } \
	ZEND_METHOD(cls, fromPointer) { zend_long p; ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(p) ZEND_PARSE_PARAMETERS_END(); \
		zend_class_entry *scope = zend_get_called_scope(execute_data); \
		if (p == 0) { object_init_ex(return_value, scope); return; } \
		phpfc_box(return_value, (void *) (uintptr_t) p, scope, NULL); }

extern zend_class_entry *phpfc_ce_FcConfig;
extern zend_class_entry *phpfc_ce_FcPattern;
extern zend_class_entry *phpfc_ce_FcFontSet;
extern zend_class_entry *phpfc_ce_FcCharSet;

void phpfc_register_FcConfig(int module_number);
void phpfc_register_FcPattern(int module_number);

#endif
