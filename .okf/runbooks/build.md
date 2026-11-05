---
type: Runbook
title: Build, install, test
description: Release and probe builds on macOS and Linux, and the test runs that gate a commit.
resource: install-macos.sh
tags: [fontconfig, build]
status: draft
generated: { by: claude/opus-5.5, at: 2026-11-05T12:00:00-05:00 }
sources:
  - id: mac
    resource: install-macos.sh
    title: macOS installer
---

# Overview

macOS: `bash install-macos.sh` builds in a temp copy for Homebrew `php@8.4` and `php@8.4-zts`, installs, ad-hoc signs and enables `30-fontconfig.ini`. It fails on any warning from `src/`.[^mac]

Linux (Debian trixie, Ubuntu 24.04, whose fontconfig 2.15.0 is the floor): `bash install-debian-trixie.sh` in the checkout. It needs sudo to install the `.so` and the ini files.

Probe build: `FONTCONFIG_CONFIGURE=--enable-fontconfig-probes bash install-macos.sh /opt/homebrew/opt/php@8.4/bin/php`. Run `tests/Probes`, then reinstall the release build. Never ship a probe build.

Tests: `composer install`, then `XDEBUG_MODE=off php84 -d memory_limit=128M vendor/bin/pest` and the same with `zhp`. The fallback test needs a CJK font. Gate a commit on the exit code.

[^mac]: macOS installer
