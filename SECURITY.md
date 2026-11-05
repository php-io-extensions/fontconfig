# Security Policy

## Supported versions

ext-fontconfig is pre-1.0. No 0.x release receives security fixes or advisories; fixes land in the
next release line. Security support starts with 1.0.

| Version | Security fixes |
|---------|----------------|
| < 1.0   | No             |

## Reporting a vulnerability

Please don't open a public issue for a security problem.

Report it privately through GitHub: the **Report a vulnerability** button on the
[php-io-extensions/fontconfig](https://github.com/php-io-extensions/fontconfig) repository's **Security** tab.
If that isn't available, email **info@projectsaturnstudios.com**.

Include what you found, the affected version, your platform, PHP build (NTS or ZTS) and the
fontconfig version (`php --ri fontconfig`), the pattern string that triggers it, and the smallest PHP
script that shows it. Reports are read and weighed for the release line in development; before 1.0
there is no response-time commitment.

A defect in fontconfig itself belongs with the fontconfig project. Report it here as well when
ext-fontconfig should guard against it.

## Security model

ext-fontconfig binds fontconfig one to one: it passes what PHP gives it to fontconfig, so it is
exactly as robust as the fontconfig it is linked against. Keep it patched; the installers link
whatever pkg-config finds. fontconfig reads the system's font configuration and font files; the
extension opens nothing itself.

- **Refusals.** The extension refuses a call only where fontconfig would read memory it should not or
  abort the process: a pattern that holds NULL in `FcConfigSubstitute()`, `FcDefaultSubstitute()`,
  `FcFontMatch()` and `FcFontSort()`, and a property read on an `FcFontSet` that holds NULL. Each
  refusal is a `ValueError`, and each has a probe test that shows the crash without it.
- **Pass-through.** The getters, `FcCharSetHasChar()` and the destroy calls check NULL themselves. A
  NULL or destroyed `FcConfig` is the current configuration.
- **Ownership.** The patterns of an `FcFontSet` are released with the last set that holds them, and a
  pattern's charset with its pattern, so a released handle is never used as a live one.
- **Addresses.** `fromPointer()` trusts any address it is given. Never pass it an integer from
  untrusted input.

A report is in scope when ext-fontconfig itself reads or writes memory it should not, leaks, or
crashes on a call that its README does not document as a pass-through.
