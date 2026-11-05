/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5086259493bc9e858e247d728fb2b6584101728f */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_FcNameParse, 0, 1, FcPattern, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcPatternDestroy, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, p, FcPattern, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcPatternGetString, 0, 4, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, p, FcPattern, 0)
	ZEND_ARG_TYPE_INFO(0, object, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, s, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcPatternGetInteger, 0, 4, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, p, FcPattern, 0)
	ZEND_ARG_TYPE_INFO(0, object, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, i, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcPatternGetCharSet, 0, 4, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, p, FcPattern, 0)
	ZEND_ARG_TYPE_INFO(0, object, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(1, c, FcCharSet, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcCharSetHasChar, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, fcs, FcCharSet, 0)
	ZEND_ARG_TYPE_INFO(0, ucs4, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcCharSetDestroy, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, fcs, FcCharSet, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_FcPattern___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_FcPattern_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_FcPattern_fromPointer, 0, 1, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_FcCharSet___construct arginfo_class_FcPattern___construct

#define arginfo_class_FcCharSet_pointer arginfo_class_FcPattern_pointer

#define arginfo_class_FcCharSet_fromPointer arginfo_class_FcPattern_fromPointer

ZEND_FUNCTION(FcNameParse);
ZEND_FUNCTION(FcPatternDestroy);
ZEND_FUNCTION(FcPatternGetString);
ZEND_FUNCTION(FcPatternGetInteger);
ZEND_FUNCTION(FcPatternGetCharSet);
ZEND_FUNCTION(FcCharSetHasChar);
ZEND_FUNCTION(FcCharSetDestroy);
ZEND_METHOD(FcPattern, __construct);
ZEND_METHOD(FcPattern, pointer);
ZEND_METHOD(FcPattern, fromPointer);
ZEND_METHOD(FcCharSet, __construct);
ZEND_METHOD(FcCharSet, pointer);
ZEND_METHOD(FcCharSet, fromPointer);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(FcNameParse, arginfo_FcNameParse)
	ZEND_FE(FcPatternDestroy, arginfo_FcPatternDestroy)
	ZEND_FE(FcPatternGetString, arginfo_FcPatternGetString)
	ZEND_FE(FcPatternGetInteger, arginfo_FcPatternGetInteger)
	ZEND_FE(FcPatternGetCharSet, arginfo_FcPatternGetCharSet)
	ZEND_FE(FcCharSetHasChar, arginfo_FcCharSetHasChar)
	ZEND_FE(FcCharSetDestroy, arginfo_FcCharSetDestroy)
	ZEND_FE_END
};

static const zend_function_entry class_FcPattern_methods[] = {
	ZEND_ME(FcPattern, __construct, arginfo_class_FcPattern___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(FcPattern, pointer, arginfo_class_FcPattern_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(FcPattern, fromPointer, arginfo_class_FcPattern_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static const zend_function_entry class_FcCharSet_methods[] = {
	ZEND_ME(FcCharSet, __construct, arginfo_class_FcCharSet___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(FcCharSet, pointer, arginfo_class_FcCharSet_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(FcCharSet, fromPointer, arginfo_class_FcCharSet_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_FcPattern(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "FcPattern", class_FcPattern_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_FcCharSet(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "FcCharSet", class_FcCharSet_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}
