---
type: API
title: Bindings
description: Every bound fontconfig function, handle and constant group, by the stub that declares it.
resource: stubs/
tags: [fontconfig, api]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-05T12:00:00-05:00 }
sources:
  - id: stubs
    resource: stubs/
    title: The stubs
---

# Overview

| Stub | C file | Functions | Classes |
|---|---|---|---|
| `fontconfig.stub.php` | `src/fontconfig.c` | — | — (constants: 57 `FC_*` property names, `FcMatch*`, `FcResult*`) |
| `FcPattern.stub.php` | `src/FcPattern.c` | `FcNameParse`, `FcPatternDestroy`, `FcPatternGetString`, `FcPatternGetInteger`, `FcPatternGetCharSet`, `FcCharSetHasChar`, `FcCharSetDestroy` | `FcPattern`, `FcCharSet` |
| `FcConfig.stub.php` | `src/FcConfig.c` | `FcInitLoadConfigAndFonts`, `FcConfigDestroy`, `FcConfigSubstitute`, `FcDefaultSubstitute`, `FcFontMatch`, `FcFontSort`, `FcFontSetDestroy` | `FcConfig`, `FcFontSet` |

`FC_CACHE_SUFFIX`, `FC_DIR_CACHE_FILE` and `FC_USER_CACHE_FILE` are file names, not property names, and are not bound.[^stubs]

[^stubs]: The stubs
