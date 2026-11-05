---
type: Module
title: Handles
description: One PHP object per native pointer and class, sets owning their patterns and patterns their charsets, and the NULL refusals.
resource: src/runtime.c
tags: [fontconfig, zend, lifetime]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-05T12:00:00-05:00 }
sources:
  - id: runtime
    resource: src/runtime.h
    title: phpfc_handle and the identity table
---

# Overview

A handle is `phpfc_handle { void *ptr; zval children; uint32_t owners; zend_object std; }`. `ptr` is the native object, or NULL once destroyed or when made by `fromPointer(0)`. `free_obj` drops the identity entry and the children; it never calls a destroy function.[^runtime]

`phpfc_box(rv, ptr, ce, parent)` returns the PHP object already stored for that address and class, or a new one. The identity table is module global `boxes`, keyed by (address, class), one per ZTS thread, so a `fromPointer()` of another class at the same address does not disturb a live handle. With a parent, the object goes into the parent's `children`, keyed by object handle so a parent owns it once, and its `owners` count goes up: the patterns of an `FcFontSet` (boxed when `$set->fonts` is read), the charset `FcPatternGetCharSet` reads. Two `FcFontSort` sets from one config share its patterns, so one pattern can have two owners. Before a parent takes a new child it drops the children already destroyed, and `get_gc` reports its children.[^runtime]

`phpfc_release` clears `ptr` and, for each child, lowers `owners` and releases the child when it reaches 0; `FcPatternDestroy`, `FcFontSetDestroy`, `FcCharSetDestroy` and `FcConfigDestroy` call it after the native call. Destroying one sorted set leaves a pattern the other set still owns usable.[^runtime]

fontconfig checks NULL in `FcPatternGet*`, `FcCharSetHasChar` and the destroy calls, so a handle holding NULL passes through there. A NULL config is the current configuration, so a destroyed `FcConfig` passes through too and the call works on the current configuration. `FcConfigSubstitute` and `FcDefaultSubstitute` dereference the pattern, and `FcFontMatch` and `FcFontSort` assert it, so `phpfc_live_pattern` (a `helpers.marshal` function) refuses a pattern holding NULL (site `fontconfig-null-pattern`). The `FcFontSet` field reader refuses a set holding NULL (site `fontconfig-null-record`); `isset()`, `empty()` and `var_dump()` read the set's fields the same way, and answer from the standard handlers, without throwing, for a set holding NULL. `phpfc_handle_setup` is a macro that gives each call site its own static handler table, so there is no limit on the number of handle classes.[^runtime]

[^runtime]: phpfc_handle and the identity table
