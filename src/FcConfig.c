#include "runtime.h"
#include "../stubs/FcConfig_arginfo.h"

zend_class_entry *phpfc_ce_FcConfig;
zend_class_entry *phpfc_ce_FcFontSet;

PHPFC_HANDLE_METHODS(FcConfig)
PHPFC_HANDLE_METHODS(FcFontSet)

/* FcFontSet fields, read from the native set each time PHP reads them. */
static zval *phpfc_font_set_read(zend_object *obj, zend_string *name, int type, void **cache_slot, zval *rv)
{
	FcFontSet *set = phpfc_handle_from(obj)->ptr;

	if (!PHPFC_PROBE_OFF("fontconfig-null-record") && set == NULL) {
		zend_value_error("FcFontSet holds NULL: it was destroyed, or made from address 0"); /* binding:fontconfig-null-record */
		return &EG(uninitialized_zval);
	}

	if (zend_string_equals_literal(name, "nfont")) {
		ZVAL_LONG(rv, set->nfont);
		return rv;
	}
	if (zend_string_equals_literal(name, "sfont")) {
		ZVAL_LONG(rv, set->sfont);
		return rv;
	}
	if (zend_string_equals_literal(name, "fonts")) {
		zval pattern;

		array_init_size(rv, set->nfont);
		for (int i = 0; i < set->nfont; i++) {
			phpfc_box(&pattern, set->fonts[i], phpfc_ce_FcPattern, obj);
			add_next_index_zval(rv, &pattern);
		}
		return rv;
	}
	return zend_std_read_property(obj, name, type, cache_slot, rv);
}

ZEND_FUNCTION(FcInitLoadConfigAndFonts)
{
	ZEND_PARSE_PARAMETERS_NONE();

	phpfc_box(return_value, FcInitLoadConfigAndFonts(), phpfc_ce_FcConfig, NULL);
}

ZEND_FUNCTION(FcConfigDestroy)
{
	zval *config;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(config, phpfc_ce_FcConfig)
	ZEND_PARSE_PARAMETERS_END();

	FcConfigDestroy(phpfc_handle_ptr(config));
	phpfc_release(Z_OBJ_P(config));
}

ZEND_FUNCTION(FcConfigSubstitute)
{
	zval *config, *p;
	zend_long kind;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(config, phpfc_ce_FcConfig)
		Z_PARAM_OBJECT_OF_CLASS(p, phpfc_ce_FcPattern)
		Z_PARAM_LONG(kind)
	ZEND_PARSE_PARAMETERS_END();

	FcPattern *pattern = phpfc_live_pattern(p, 2);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	RETURN_BOOL(FcConfigSubstitute(config != NULL ? phpfc_handle_ptr(config) : NULL, pattern, (FcMatchKind) kind));
}

ZEND_FUNCTION(FcDefaultSubstitute)
{
	zval *pattern;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(pattern, phpfc_ce_FcPattern)
	ZEND_PARSE_PARAMETERS_END();

	FcPattern *p = phpfc_live_pattern(pattern, 1);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	FcDefaultSubstitute(p);
}

ZEND_FUNCTION(FcFontMatch)
{
	zval *config, *p, *result;
	FcResult r = FcResultNoMatch;
	FcPattern *match;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(config, phpfc_ce_FcConfig)
		Z_PARAM_OBJECT_OF_CLASS(p, phpfc_ce_FcPattern)
		Z_PARAM_ZVAL(result)
	ZEND_PARSE_PARAMETERS_END();

	FcPattern *pattern = phpfc_live_pattern(p, 2);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	match = FcFontMatch(config != NULL ? phpfc_handle_ptr(config) : NULL, pattern, &r);
	ZEND_TRY_ASSIGN_REF_LONG(result, r);
	phpfc_box(return_value, match, phpfc_ce_FcPattern, NULL);
}

ZEND_FUNCTION(FcFontSort)
{
	zval *config, *p, *csp, *result, coverage;
	bool trim;
	FcResult r = FcResultNoMatch;
	FcCharSet *cs = NULL;
	FcFontSet *set;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(config, phpfc_ce_FcConfig)
		Z_PARAM_OBJECT_OF_CLASS(p, phpfc_ce_FcPattern)
		Z_PARAM_BOOL(trim)
		Z_PARAM_ZVAL(csp)
		Z_PARAM_ZVAL(result)
	ZEND_PARSE_PARAMETERS_END();

	FcPattern *pattern = phpfc_live_pattern(p, 2);
	if (EG(exception)) {
		RETURN_THROWS();
	}
	set = FcFontSort(config != NULL ? phpfc_handle_ptr(config) : NULL, pattern, trim, &cs, &r);
	phpfc_box(&coverage, cs, phpfc_ce_FcCharSet, NULL);
	ZEND_TRY_ASSIGN_REF_TMP(csp, &coverage);
	ZEND_TRY_ASSIGN_REF_LONG(result, r);
	phpfc_box(return_value, set, phpfc_ce_FcFontSet, NULL);
}

ZEND_FUNCTION(FcFontSetDestroy)
{
	zval *s;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(s, phpfc_ce_FcFontSet)
	ZEND_PARSE_PARAMETERS_END();

	FcFontSetDestroy(phpfc_handle_ptr(s));
	phpfc_release(Z_OBJ_P(s));
}

void phpfc_register_FcConfig(int module_number)
{
	(void) module_number;
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	phpfc_ce_FcConfig = register_class_FcConfig();
	phpfc_handle_setup(phpfc_ce_FcConfig, NULL);
	phpfc_ce_FcFontSet = register_class_FcFontSet();
	phpfc_handle_setup(phpfc_ce_FcFontSet, phpfc_font_set_read);
}
