#include "runtime.h"
#include "../stubs/FcPattern_arginfo.h"

zend_class_entry *phpfc_ce_FcPattern;
zend_class_entry *phpfc_ce_FcCharSet;

PHPFC_HANDLE_METHODS(FcPattern)
PHPFC_HANDLE_METHODS(FcCharSet)

ZEND_FUNCTION(FcNameParse)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	phpfc_box(return_value, FcNameParse((const FcChar8 *) ZSTR_VAL(name)), phpfc_ce_FcPattern, NULL);
}

ZEND_FUNCTION(FcPatternDestroy)
{
	zval *p;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(p, phpfc_ce_FcPattern)
	ZEND_PARSE_PARAMETERS_END();

	FcPatternDestroy(phpfc_handle_ptr(p));
	phpfc_release(Z_OBJ_P(p));
}

ZEND_FUNCTION(FcPatternGetString)
{
	zval *p, *s;
	zend_string *object;
	zend_long n;
	FcChar8 *value = NULL;
	FcResult result;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_OBJECT_OF_CLASS(p, phpfc_ce_FcPattern)
		Z_PARAM_PATH_STR(object)
		Z_PARAM_LONG(n)
		Z_PARAM_ZVAL(s)
	ZEND_PARSE_PARAMETERS_END();

	result = FcPatternGetString(phpfc_handle_ptr(p), ZSTR_VAL(object), (int) n, &value);
	if (result == FcResultMatch) {
		ZEND_TRY_ASSIGN_REF_STRING(s, (const char *) value);
	} else {
		ZEND_TRY_ASSIGN_REF_NULL(s);
	}
	RETURN_LONG(result);
}

ZEND_FUNCTION(FcPatternGetInteger)
{
	zval *p, *i;
	zend_string *object;
	zend_long n;
	int value = 0;
	FcResult result;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_OBJECT_OF_CLASS(p, phpfc_ce_FcPattern)
		Z_PARAM_PATH_STR(object)
		Z_PARAM_LONG(n)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();

	result = FcPatternGetInteger(phpfc_handle_ptr(p), ZSTR_VAL(object), (int) n, &value);
	if (result == FcResultMatch) {
		ZEND_TRY_ASSIGN_REF_LONG(i, value);
	} else {
		ZEND_TRY_ASSIGN_REF_NULL(i);
	}
	RETURN_LONG(result);
}

ZEND_FUNCTION(FcPatternGetCharSet)
{
	zval *p, *c, boxed;
	zend_string *object;
	zend_long n;
	FcCharSet *value = NULL;
	FcResult result;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_OBJECT_OF_CLASS(p, phpfc_ce_FcPattern)
		Z_PARAM_PATH_STR(object)
		Z_PARAM_LONG(n)
		Z_PARAM_ZVAL(c)
	ZEND_PARSE_PARAMETERS_END();

	result = FcPatternGetCharSet(phpfc_handle_ptr(p), ZSTR_VAL(object), (int) n, &value);
	phpfc_box(&boxed, result == FcResultMatch ? value : NULL, phpfc_ce_FcCharSet, Z_OBJ_P(p));
	ZEND_TRY_ASSIGN_REF_TMP(c, &boxed);
	RETURN_LONG(result);
}

ZEND_FUNCTION(FcCharSetHasChar)
{
	zval *fcs;
	zend_long ucs4;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJECT_OF_CLASS(fcs, phpfc_ce_FcCharSet)
		Z_PARAM_LONG(ucs4)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_BOOL(FcCharSetHasChar(phpfc_handle_ptr(fcs), (FcChar32) ucs4));
}

ZEND_FUNCTION(FcCharSetDestroy)
{
	zval *fcs;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(fcs, phpfc_ce_FcCharSet)
	ZEND_PARSE_PARAMETERS_END();

	FcCharSetDestroy(phpfc_handle_ptr(fcs));
	phpfc_release(Z_OBJ_P(fcs));
}

void phpfc_register_FcPattern(int module_number)
{
	(void) module_number;
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	phpfc_ce_FcPattern = register_class_FcPattern();
	phpfc_handle_setup(phpfc_ce_FcPattern, NULL);
	phpfc_ce_FcCharSet = register_class_FcCharSet();
	phpfc_handle_setup(phpfc_ce_FcCharSet, NULL);
}
