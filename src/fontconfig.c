/*
 * fontconfig: 1:1 bindings of fontconfig at the 2.15.0 API. Functions, handles, structs and constants
 * keep their C names. Dropping the last PHP reference does not destroy the native object.
 */

#include "runtime.h"
#include "ext/standard/info.h"
#include "../stubs/fontconfig_arginfo.h"

ZEND_DECLARE_MODULE_GLOBALS(fontconfig)

static PHP_GINIT_FUNCTION(fontconfig)
{
#if defined(COMPILE_DL_FONTCONFIG) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	zend_hash_init(&fontconfig_globals->boxes, 64, NULL, NULL, 1);
}

static PHP_GSHUTDOWN_FUNCTION(fontconfig)
{
	zend_hash_destroy(&fontconfig_globals->boxes);
}

PHP_MINIT_FUNCTION(fontconfig)
{
	register_fontconfig_symbols(module_number);
#ifdef PHPFC_PROBES
	/* A probe build: env PHPFC_PROBE_OFF=<site id> turns that kept refusal off (tests/Probes). */
	REGISTER_BOOL_CONSTANT("PHPFC_PROBES", true, CONST_PERSISTENT);
#endif
	phpfc_register_FcPattern(module_number);
	phpfc_register_FcConfig(module_number);

	return SUCCESS;
}

PHP_RINIT_FUNCTION(fontconfig)
{
#if defined(COMPILE_DL_FONTCONFIG) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

PHP_MINFO_FUNCTION(fontconfig)
{
	char built[32], loaded[32];
	int version = FcGetVersion();

	snprintf(built, sizeof(built), "%d.%d.%d", FC_MAJOR, FC_MINOR, FC_REVISION);
	snprintf(loaded, sizeof(loaded), "%d.%d.%d", version / 10000, (version / 100) % 100, version % 100);
	php_info_print_table_start();
	php_info_print_table_row(2, "fontconfig support", "enabled");
	php_info_print_table_row(2, "Version", PHP_FONTCONFIG_VERSION);
	php_info_print_table_row(2, "fontconfig headers", built);
	php_info_print_table_row(2, "fontconfig library", loaded);
	php_info_print_table_end();
}

zend_module_entry fontconfig_module_entry = {
	STANDARD_MODULE_HEADER,
	"fontconfig",
	NULL,
	PHP_MINIT(fontconfig),
	NULL,
	PHP_RINIT(fontconfig),
	NULL,
	PHP_MINFO(fontconfig),
	PHP_FONTCONFIG_VERSION,
	PHP_MODULE_GLOBALS(fontconfig),
	PHP_GINIT(fontconfig),
	PHP_GSHUTDOWN(fontconfig),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_FONTCONFIG
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(fontconfig)
#endif
