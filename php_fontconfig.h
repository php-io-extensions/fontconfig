#ifndef PHP_FONTCONFIG_H
#define PHP_FONTCONFIG_H

extern zend_module_entry fontconfig_module_entry;
#define phpext_fontconfig_ptr &fontconfig_module_entry

#define PHP_FONTCONFIG_VERSION "0.11.0"

#if defined(ZTS) && defined(COMPILE_DL_FONTCONFIG)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif
