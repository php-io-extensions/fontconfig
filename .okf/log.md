# Log

## 2026-11-08

* **Version**: 0.11.1 in `PHP_FONTCONFIG_VERSION` (`php_fontconfig.h`) and the README's phpinfo block; nothing else changed.

## 2026-11-05

* Bundle created with ext-fontconfig 0.11.0: 14 functions at the fontconfig 2.15.0 API, the floor (Ubuntu 24.04's fontconfig): configuration, name parsing, substitution, matching, sorting, pattern getters for strings, integers and charsets, `FcCharSetHasChar`, and the destroy calls. `FcCharSetDestroy` is bound because `FcFontSort` hands the caller a charset. Built on Homebrew PHP 8.4 NTS and ZTS against fontconfig 2.18.3, and on Ubuntu against 2.15.0. [bindings](api/bindings.md), [handles](architecture/handles.md), [build](runbooks/build.md).
* **Refusals**: a pattern holding NULL in `FcConfigSubstitute`/`FcDefaultSubstitute` (SIGSEGV) and `FcFontMatch`/`FcFontSort` (assert, SIGABRT): site `fontconfig-null-pattern`; an `FcFontSet` field read holding NULL (SIGSEGV): site `fontconfig-null-record`. Pass through, probed: `FcPatternGet*` on NULL (`FcResultNoMatch`), `FcCharSetHasChar` on NULL (false), every destroy on NULL, a NULL or destroyed config (the current configuration).
* **Shared patterns**: two `FcFontSort` sets share the config's patterns; a pattern is released only when its last owning set is (`owners`), so destroying one set leaves the other's patterns usable.
* **Handle runtime**: identity is keyed by address and class, so a `fromPointer()` of another class does not clear a live handle. A parent drops destroyed children when it gains a new one, and `get_gc` reports its children. `FcFontSet` answers `isset()`, `empty()` and `var_dump()` from the set. Handler tables are static per class, with no fixed limit.
* **Coverage**: `scripts/support.php` counts every `FcPublic … name (` of `fontconfig.h` and `fcfreetype.h` at the 2.15.0 tag.
* **Weight and slant values**: the `FC_WEIGHT_*` (19) and `FC_SLANT_*` (3) constants of `fontconfig.h` (lines 145–167 at 2.15.0), as `@cvalue` ints. [bindings](api/bindings.md).
