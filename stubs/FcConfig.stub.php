<?php

/** @generate-class-entries */

/**
 * An FcConfig. One PHP object per native pointer; dropping it does not call FcConfigDestroy().
 * fromPointer() trusts any address other than 0.
 */
final class FcConfig
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): static {}
}

/**
 * An FcFontSet. One PHP object per native pointer; dropping it does not call FcFontSetDestroy().
 * nfont and fonts read the FcFontSet fields each time PHP reads them; the patterns in fonts belong to the
 * set. fromPointer() trusts any address other than 0.
 */
final class FcFontSet
{
    public readonly int $nfont;
    public readonly int $sfont;
    public readonly array $fonts;

    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): static {}
}

function FcInitLoadConfigAndFonts(): ?FcConfig {}

function FcConfigDestroy(FcConfig $config): void {}

function FcConfigSubstitute(?FcConfig $config, FcPattern $p, int $kind): bool {}

function FcDefaultSubstitute(FcPattern $pattern): void {}

function FcFontMatch(?FcConfig $config, FcPattern $p, ?int &$result): ?FcPattern {}

/** $csp is the coverage FcCharSet, which the caller owns (FcCharSetDestroy). */
function FcFontSort(?FcConfig $config, FcPattern $p, bool $trim, ?FcCharSet &$csp, ?int &$result): ?FcFontSet {}

function FcFontSetDestroy(FcFontSet $s): void {}
