---
type: Runbook
title: Adding a binding
description: Declare it in the stub for its group, regenerate arginfo, implement it beside that group, test it, and refresh coverage.
resource: stubs/
tags: [fontconfig, contributing]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-05T12:00:00-05:00 }
sources:
  - id: build
    resource: runbooks/build.md
    title: Build runbook
---

# Overview

1. Check the function exists in fontconfig 2.15.0 (the floor). Add it, its class or its `@cvalue` constant to the stub of its group: `stubs/FcPattern.stub.php`, `FcConfig.stub.php`, or `fontconfig.stub.php` for constants. A new group is a new stub and a new `src/<Group>.c`, added to `config.m4` and registered from `PHP_MINIT_FUNCTION` in `src/fontconfig.c`. ext-guard checks the stub against the fontconfig headers.
2. `php84 /opt/homebrew/opt/php@8.4/lib/php/build/gen_stub.php stubs`. Commit the stub and the arginfo.
3. Implement it in the C file that includes that arginfo, one written-out `ZEND_FUNCTION` per stub function. Probe what fontconfig does with a NULL handle before choosing between pass-through and `phpfc_live_pattern`. A new handle class gets `phpfc_handle_setup` and `PHPFC_HANDLE_METHODS`, and `pointer`/`fromPointer` under `glue` in `bindings.json` (the maintainer adds those).
4. Write the test first, watch it fail, build per [build](/runbooks/build.md), watch it pass.
5. `php84 scripts/support.php` refreshes SUPPORT.md and the README badge.

[^build]: Build runbook
