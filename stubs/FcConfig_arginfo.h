/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a902ba6a89d7814c0594bd025fa65c8e6b4ab2f3 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_FcInitLoadConfigAndFonts, 0, 0, FcConfig, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcConfigDestroy, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, config, FcConfig, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcConfigSubstitute, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, config, FcConfig, 1)
	ZEND_ARG_OBJ_INFO(0, p, FcPattern, 0)
	ZEND_ARG_TYPE_INFO(0, kind, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcDefaultSubstitute, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, pattern, FcPattern, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_FcFontMatch, 0, 3, FcPattern, 1)
	ZEND_ARG_OBJ_INFO(0, config, FcConfig, 1)
	ZEND_ARG_OBJ_INFO(0, p, FcPattern, 0)
	ZEND_ARG_TYPE_INFO(1, result, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_FcFontSort, 0, 5, FcFontSet, 1)
	ZEND_ARG_OBJ_INFO(0, config, FcConfig, 1)
	ZEND_ARG_OBJ_INFO(0, p, FcPattern, 0)
	ZEND_ARG_TYPE_INFO(0, trim, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(1, csp, FcCharSet, 1)
	ZEND_ARG_TYPE_INFO(1, result, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_FcFontSetDestroy, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, s, FcFontSet, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_FcConfig___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_FcConfig_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_FcConfig_fromPointer, 0, 1, IS_STATIC, 0)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_FcFontSet___construct arginfo_class_FcConfig___construct

#define arginfo_class_FcFontSet_pointer arginfo_class_FcConfig_pointer

#define arginfo_class_FcFontSet_fromPointer arginfo_class_FcConfig_fromPointer

ZEND_FUNCTION(FcInitLoadConfigAndFonts);
ZEND_FUNCTION(FcConfigDestroy);
ZEND_FUNCTION(FcConfigSubstitute);
ZEND_FUNCTION(FcDefaultSubstitute);
ZEND_FUNCTION(FcFontMatch);
ZEND_FUNCTION(FcFontSort);
ZEND_FUNCTION(FcFontSetDestroy);
ZEND_METHOD(FcConfig, __construct);
ZEND_METHOD(FcConfig, pointer);
ZEND_METHOD(FcConfig, fromPointer);
ZEND_METHOD(FcFontSet, __construct);
ZEND_METHOD(FcFontSet, pointer);
ZEND_METHOD(FcFontSet, fromPointer);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(FcInitLoadConfigAndFonts, arginfo_FcInitLoadConfigAndFonts)
	ZEND_FE(FcConfigDestroy, arginfo_FcConfigDestroy)
	ZEND_FE(FcConfigSubstitute, arginfo_FcConfigSubstitute)
	ZEND_FE(FcDefaultSubstitute, arginfo_FcDefaultSubstitute)
	ZEND_FE(FcFontMatch, arginfo_FcFontMatch)
	ZEND_FE(FcFontSort, arginfo_FcFontSort)
	ZEND_FE(FcFontSetDestroy, arginfo_FcFontSetDestroy)
	ZEND_FE_END
};

static const zend_function_entry class_FcConfig_methods[] = {
	ZEND_ME(FcConfig, __construct, arginfo_class_FcConfig___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(FcConfig, pointer, arginfo_class_FcConfig_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(FcConfig, fromPointer, arginfo_class_FcConfig_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static const zend_function_entry class_FcFontSet_methods[] = {
	ZEND_ME(FcFontSet, __construct, arginfo_class_FcFontSet___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(FcFontSet, pointer, arginfo_class_FcFontSet_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(FcFontSet, fromPointer, arginfo_class_FcFontSet_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_FcConfig(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "FcConfig", class_FcConfig_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	return class_entry;
}

static zend_class_entry *register_class_FcFontSet(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "FcFontSet", class_FcFontSet_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL);

	zval property_nfont_default_value;
	ZVAL_UNDEF(&property_nfont_default_value);
	zend_string *property_nfont_name = zend_string_init("nfont", sizeof("nfont") - 1, 1);
	zend_declare_typed_property(class_entry, property_nfont_name, &property_nfont_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_nfont_name);

	zval property_sfont_default_value;
	ZVAL_UNDEF(&property_sfont_default_value);
	zend_string *property_sfont_name = zend_string_init("sfont", sizeof("sfont") - 1, 1);
	zend_declare_typed_property(class_entry, property_sfont_name, &property_sfont_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_LONG));
	zend_string_release(property_sfont_name);

	zval property_fonts_default_value;
	ZVAL_UNDEF(&property_fonts_default_value);
	zend_string *property_fonts_name = zend_string_init("fonts", sizeof("fonts") - 1, 1);
	zend_declare_typed_property(class_entry, property_fonts_name, &property_fonts_default_value, ZEND_ACC_PUBLIC|ZEND_ACC_READONLY, NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ARRAY));
	zend_string_release(property_fonts_name);

	return class_entry;
}
