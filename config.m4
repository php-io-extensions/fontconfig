PHP_ARG_ENABLE([fontconfig],
  [whether to enable fontconfig support],
  [AS_HELP_STRING([--enable-fontconfig], [Enable the fontconfig bindings])],
  [no])

PHP_ARG_ENABLE([fontconfig-probes],
  [whether to build the fontconfig probe switch],
  [AS_HELP_STRING([--enable-fontconfig-probes],
    [Build the PHPFC_PROBE_OFF switch that tests/Probes drives: env PHPFC_PROBE_OFF=<site id> turns that kept refusal off. Never for a release build])],
  [no],
  [no])

if test "$PHP_FONTCONFIG" != "no"; then
  if test "$PHP_FONTCONFIG_PROBES" != "no"; then
    AC_DEFINE([PHPFC_PROBES], [1], [Kept refusal sites can be turned off through env PHPFC_PROBE_OFF])
  fi

  dnl fontconfig 2.15.0 is the floor.
  PKG_CHECK_MODULES([FONTCONFIG], [fontconfig >= 2.15.0])
  PHP_EVAL_INCLINE([$FONTCONFIG_CFLAGS])
  PHP_EVAL_LIBLINE([$FONTCONFIG_LIBS], [FONTCONFIG_SHARED_LIBADD])
  PHP_SUBST([FONTCONFIG_SHARED_LIBADD])

  PHP_NEW_EXTENSION([fontconfig],
    [src/fontconfig.c src/runtime.c src/FcPattern.c src/FcConfig.c],
    [$ext_shared],, [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -Wall -Wextra])
  PHP_ADD_BUILD_DIR([$ext_builddir/src])
fi
