<div align="center" id="header">

<h1>ext-fontconfig</h1>

<h3>fontconfig bound 1:1 into PHP 8.4+, at the 2.15.0 API</h3>


[![CI](https://img.shields.io/github/actions/workflow/status/php-io-extensions/fontconfig/ci.yml?style=flat-square&label=CI)](https://github.com/php-io-extensions/fontconfig/actions/workflows/ci.yml)
[![fontconfig API](https://img.shields.io/badge/fontconfig_API-6.0%25_%2814%2F232%29-blue?style=flat-square)](SUPPORT.md)
[![Packagist](https://img.shields.io/packagist/v/php-io-extensions/fontconfig?style=flat-square)](https://packagist.org/packages/php-io-extensions/fontconfig)
[![PHP](https://img.shields.io/packagist/dependency-v/php-io-extensions/fontconfig/php?style=flat-square&label=PHP)](https://packagist.org/packages/php-io-extensions/fontconfig)
[![License](https://img.shields.io/github/license/php-io-extensions/fontconfig?style=flat-square)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-linux_%7C_macos-blue?style=flat-square&logo=linux&logoColor=white)](#requirements)
[![OKF](https://img.shields.io/badge/OKF-0.2-6f42c1?style=flat-square&label=knowledge%20bundle)](.okf/index.md)
[![PIE](https://img.shields.io/badge/PIE-installable-4f5b93?style=flat-square&logo=php&logoColor=white)](https://github.com/php/pie)

</div>

Text that names a font by family, "monospace" or "Inter Bold", needs a file before anything can be drawn. Which file depends on the machine: what is installed, where, what the user and the distribution configured as the default sans or monospace, and which installed font has a character the first choice lacks, such as 中 in a Latin monospace. Hard-coding paths breaks on the next machine, and scanning font directories yourself means parsing every font to learn its family, style and coverage.

<b>fontconfig</b> answers that question on Linux and BSD desktops, and Homebrew's build answers it on macOS from the system fonts: parse a name into a pattern, let the configuration fill in the defaults, and ask for the best match, or for the whole list in order of preference with the characters each font covers. It is written in C, and its documentation covers every call.

<b>The only problem?</b> PHP can't reach it. This extension binds fontconfig one to one under the C names, at the fontconfig 2.15.0 API: the configuration, patterns parsed from names, substitution, matching and sorting, pattern properties, and character sets. Handles are one PHP object per native pointer, a font set's patterns are read from the native set as a list, and `FcResult` comes back through a by-reference argument, as in C. No defaults, no composites, no opinions: what fontconfig documents is what you call.

## Basic usage

Most ways you will interface with ext-fontconfig are in two parts -

Build a pattern and match it. `FcNameParse()` turns a name into a pattern, `FcConfigSubstitute()` and `FcDefaultSubstitute()` fill in what the configuration and the defaults say, and `FcFontMatch()` answers the best font with its file and face index:

```php
$config = FcInitLoadConfigAndFonts();
$pattern = FcNameParse('monospace');
FcConfigSubstitute($config, $pattern, FcMatchPattern);
FcDefaultSubstitute($pattern);

$match = FcFontMatch($config, $pattern, $result);
FcPatternGetString($match, FC_FAMILY, 0, $family);
FcPatternGetString($match, FC_STYLE, 0, $style);
FcPatternGetString($match, FC_FILE, 0, $file);
FcPatternGetInteger($match, FC_INDEX, 0, $index);
echo "{$family} {$style}: {$file} #{$index}\n";
```

Find a fallback. `FcFontSort()` answers every font in order of preference and the characters they cover together; the first one whose charset has the character is the one to draw it with. Destroy what fontconfig handed you when you're done:

```php
$set = FcFontSort($config, $pattern, true, $coverage, $result);
foreach ($set->fonts as $font) {
    FcPatternGetCharSet($font, FC_CHARSET, 0, $charset);
    if (FcCharSetHasChar($charset, 0x4E2D)) {
        FcPatternGetString($font, FC_FILE, 0, $fallback);
        echo "中: {$fallback}\n";
        break;
    }
}

FcCharSetDestroy($coverage);
FcFontSetDestroy($set);
FcPatternDestroy($match);
FcPatternDestroy($pattern);
FcConfigDestroy($config);
```

```console
$ php match.php
Andale Mono Regular: /System/Library/Fonts/Supplemental/Andale Mono.ttf #0
中: /System/Library/Fonts/ヒラギノ丸ゴ ProN W4.ttc
```

The file and index go straight to ext-freetype's `FT_New_Face()`. The answers depend on the machine's fonts and configuration: this is a Mac.

## Requirements

| | |
|---|---|
| OS | Linux or macOS. `composer.json` lists those two families. On macOS, Homebrew's fontconfig reads the system fonts. |
| PHP | 8.4 or newer, NTS or ZTS. |
| Libraries | fontconfig 2.15.0 or newer, with its headers, found through `pkg-config` (`fontconfig`). 2.15.0 is the floor: nothing newer is bound. |

To build it you need a C compiler, `pkg-config`, fontconfig's development files, and the development files of the PHP you intend to load it into:

```bash
# Debian, Ubuntu, Raspberry Pi OS
sudo apt install build-essential pkg-config libfontconfig-dev php8.4-dev

# macOS: Xcode Command Line Tools, and Homebrew's PHP ships phpize and php-config
xcode-select --install
brew install pkgconf fontconfig php@8.4
```

## Installation

### With PHP PIE (recommended)

[PIE](https://github.com/php/pie) is the PHP Foundation's installer for extensions. It downloads the tagged source from Packagist, builds it against the PHP on your `PATH`, installs the module and enables it. `sudo` where your PHP's extension directory is root-owned, as on Debian; not on a Homebrew Mac. Install fontconfig first:

```bash
pie install php-io-extensions/fontconfig
```

### Manually (install script)

The repository ships two installers, one per platform. Each one builds for each PHP it targets, installs `fontconfig.so` into that PHP's `extension_dir`, writes `30-fontconfig.ini` into its `conf.d` scan directory, and checks the module loads. The macOS one builds in a temporary copy of the checkout and ad-hoc signs the module so the system loads it on first use; the Debian one builds in the checkout, writes the ini into the CLI, FPM and Apache `conf.d` of that PHP, and cleans the build products out afterwards.

```bash
git clone https://github.com/php-io-extensions/fontconfig.git
cd fontconfig

./install-debian-trixie.sh             # Debian Trixie, Ubuntu 24.04+, Raspberry Pi OS: the php on PATH
./install-macos.sh                     # Homebrew php@8.4 and php@8.4-zts
./install-macos.sh /path/to/bin/php    # the macOS script takes specific PHP binaries
```

### Manually (phpize)

```bash
git clone https://github.com/php-io-extensions/fontconfig.git
cd fontconfig
phpize
./configure --enable-fontconfig
make -j"$(nproc 2>/dev/null || sysctl -n hw.ncpu)"
sudo make install
```

Then enable it: add `extension=fontconfig.so` to your `php.ini`, or drop that line into a file in the scan directory that `php --ini` reports.

### Verify

`php --ri fontconfig` also names the fontconfig headers it was built against and the library it runs with:

```console
$ php --ri fontconfig

fontconfig

fontconfig support => enabled
Version => 0.11.1
fontconfig headers => 2.18.3
fontconfig library => 2.18.3
```

## API

Function, handle and constant names are the C names, so [fontconfig's developer reference](https://www.freedesktop.org/software/fontconfig/fontconfig-devel/) is the reference: 14 functions, one stub per group in [`stubs/`](stubs), each declared with its PHP types. [SUPPORT.md](SUPPORT.md) lists every function fontconfig 2.15.0 declares in `fontconfig.h` and `fcfreetype.h`, bound or not; the API badge is computed from it.

| Stub | Functions | What it covers |
|---|---|---|
| `FcConfig` | 7 | `FcInitLoadConfigAndFonts()`, `FcConfigDestroy()`, `FcConfigSubstitute()`, `FcDefaultSubstitute()`, `FcFontMatch()`, `FcFontSort()`, `FcFontSetDestroy()`; the `FcConfig` and `FcFontSet` handles. |
| `FcPattern` | 7 | `FcNameParse()`, `FcPatternDestroy()`, `FcPatternGetString()`, `FcPatternGetInteger()`, `FcPatternGetCharSet()`, `FcCharSetHasChar()`, `FcCharSetDestroy()`; the `FcPattern` and `FcCharSet` handles. |
| `fontconfig` | — | The `FC_*` property names, the `FcMatch*` kinds and the `FcResult*` values. |

### Translations

Every C parameter is its own PHP parameter, in C order. The only differences from C:

- `const FcChar8 *` and `const char *` are a `string`. A property name is one of the `FC_*` string constants (`FC_FILE`, `FC_FAMILY`, `FC_CHARSET`, …).
- An out-parameter is a by-reference parameter: `FcFontMatch($config, $pattern, $result)` writes the `FcResult` to `$result`. `FcPatternGetString()`, `FcPatternGetInteger()` and `FcPatternGetCharSet()` write the value, or `null` when the result is not `FcResultMatch`; the string is a copy.
- A `null` `FcConfig` is NULL: fontconfig uses the current configuration. So is an `FcConfig` that holds NULL (destroyed with `FcConfigDestroy()`, or made from address 0): a call with it works on the current configuration, not on the destroyed one. This is not refused, because NULL is a valid argument and nothing crashes.
- `FcBool` is `bool`. `FcMatchKind`, `FcResult` and `FcChar32` are `int`.
- `FcConfig`, `FcPattern`, `FcFontSet` and `FcCharSet` are `final` handles with a private constructor, not cloneable or serializable, with `pointer(): int` and `static fromPointer(int $pointer): static`. `fromPointer()` trusts any address. The same native pointer and class in one thread is the same PHP object. `FcFontSet::$nfont`, `$sfont` and `$fonts` read the native set each time PHP reads them, and so do `isset()`, `empty()`, `var_dump()` and `print_r()`.

### Lifetime

Dropping the last PHP reference does not destroy the native object. The patterns in `$fonts` belong to the set, and `FcFontSetDestroy()` releases them; two sets sorted from one configuration share its patterns, and a shared pattern is released with the last set that holds it. A charset read with `FcPatternGetCharSet()` belongs to its pattern, and `FcPatternDestroy()` releases it. The charset `FcFontSort()` writes to `$csp` belongs to the caller: destroy it with `FcCharSetDestroy()`.

### Refusals

The extension refuses two calls, each of which crashes the process otherwise ([`tests/Probes`](tests/Probes) shows each crash on a probe build):

- An `FcPattern` that holds NULL (destroyed, or made from address 0) in `FcConfigSubstitute()`, `FcDefaultSubstitute()`, `FcFontMatch()` or `FcFontSort()`: `ValueError`. The first two dereference it; the last two assert and abort.
- Reading `nfont`, `sfont` or `fonts` of an `FcFontSet` that holds NULL: `ValueError`.

A NULL pattern passes through to `FcPatternGet*()`, which answers `FcResultNoMatch`; a NULL charset to `FcCharSetHasChar()`, which answers `false`; and NULL to every destroy call.

## Extensions you might use with this one

* [ext-freetype](https://github.com/php-io-extensions/freetype): opens the file and face index fontconfig answers, and renders its glyphs.
* [ext-harfbuzz](https://github.com/php-io-extensions/harfbuzz): shapes each run of text with the font fontconfig picked for it.

## AI-assisted development

This codebase was built with LLM assistance under human direction. Prompts, reviews, and architectural decisions were owned by a professional software engineer — the model accelerates drafting and exploration; it does not replace judgment on APIs, dependency direction, or what ships.

## Agentic Documentation

This repository carries an [Open Knowledge Format](https://github.com/GoogleCloudPlatform/knowledge-catalog/blob/main/okf/SPEC.md) bundle at [`.okf/`](.okf/): Markdown with YAML frontmatter, readable by you and loadable by a coding agent. It records what the README doesn't: every function, class and constant by stub, how handles keep one PHP object per native pointer, who owns a font set's patterns and a pattern's charset, which NULL handles pass through and which are refused, how to build release and probe builds, how to add a binding, and a log of every change that altered behaviour.

Start at [`.okf/index.md`](.okf/index.md). [`AGENTS.md`](AGENTS.md) is the short rule sheet an agent reads before touching the C, the stubs, or packaging; each rule points into the bundle for the reason behind it.

Point your agent at the repo and it has the same context the maintainer has. Point yourself at it when the README's answer is "it does X" and you want "because".

## Resources

* [fontconfig developer reference](https://www.freedesktop.org/software/fontconfig/fontconfig-devel/): the manual for every call this extension binds; this README only adds what PHP changes.
* [fontconfig 2.15.0](https://gitlab.freedesktop.org/fontconfig/fontconfig/-/tree/2.15.0): the upstream tag the stubs and [SUPPORT.md](SUPPORT.md) are checked against.
* [PIE](https://github.com/php/pie): the PHP Foundation's extension installer.
* [`gen_stub.php`](https://github.com/php/php-src/blob/master/build/gen_stub.php): the php-src stub format. [`stubs/`](stubs) holds the API declarations; each `*_arginfo.h` is generated from its stub.
* [Open Knowledge Format](https://github.com/GoogleCloudPlatform/knowledge-catalog/blob/main/okf/SPEC.md): the spec the [`.okf/`](.okf/) bundle follows.

## Running the test suite

The suite is Pest v4: matching monospace to a font file, finding a fallback font for U+4E2D, sets that share patterns, missing properties, the current configuration, handle identity, and the coverage script. It runs on Linux and macOS against the machine's own fonts; the fallback test needs an installed font with CJK coverage (`fonts-noto-cjk` on Debian). Build the module, then enable it through an ini scan directory rather than `-d`, so the child processes the tests start load it too:

```bash
git clone https://github.com/php-io-extensions/fontconfig.git
cd fontconfig
phpize && ./configure --enable-fontconfig && make -j"$(nproc 2>/dev/null || sysctl -n hw.ncpu)"
composer install
mkdir -p /tmp/fontconfig-ini && echo "extension=$PWD/modules/fontconfig.so" > /tmp/fontconfig-ini/fontconfig.ini
PHP_INI_SCAN_DIR=":/tmp/fontconfig-ini" php -d memory_limit=128M vendor/bin/pest
```

```console
Tests:    4 skipped, 14 passed (44 assertions)
Duration: 0.21s
```

The four skipped tests are in [`tests/Probes`](tests/Probes): they run only against a probe build (`./configure --enable-fontconfig --enable-fontconfig-probes`, or `FONTCONFIG_CONFIGURE=--enable-fontconfig-probes ./install-macos.sh`), which can turn each refusal off to show the crash it prevents. Never install a probe build for use.

The leading `:` keeps your PHP's own scan directory and adds the second one. If that PHP already loads an installed `fontconfig.so`, the suite warns `Module "fontconfig" is already loaded` and tests the installed one instead; copy the scan directory somewhere, replace its fontconfig ini with the line above, and point `PHP_INI_SCAN_DIR` at the copy without the colon. A module only loads in the PHP whose `phpize` built it: for Homebrew's ZTS build, run `phpize` and pass `--with-php-config` from `/opt/homebrew/opt/php@8.4-zts/bin`. Set `XDEBUG_MODE=off` for any timing you intend to read.

## Contributing

Open an issue first for anything beyond a typo. Pull requests change a stub in `stubs/` and the C under `src/` together, regenerate the `*_arginfo.h` files with gen_stub, regenerate [SUPPORT.md](SUPPORT.md) and the API badge with `php scripts/support.php`, and add a Pest test for the behaviour. A new refusal needs a probe test that shows the crash it prevents. CI must be green on every cell of the matrix, NTS and ZTS alike. Read [`AGENTS.md`](AGENTS.md) for the rules the code follows.

## Security

ext-fontconfig hands names and patterns to fontconfig, which reads the system's font configuration and font files. See [SECURITY.md](SECURITY.md) for what it guards against and how to report a vulnerability.

## License

MIT. See [LICENSE](LICENSE). fontconfig is under its own MIT-style license; it is linked, not shipped.
