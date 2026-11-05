<?php

/** @generate-class-entries */

/**
 * An FcPattern. One PHP object per native pointer; dropping it does not call FcPatternDestroy(). A pattern
 * read from an FcFontSet belongs to the set: FcFontSetDestroy() releases it. fromPointer() trusts any
 * address other than 0.
 */
final class FcPattern
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): static {}
}

/**
 * An FcCharSet. One read from a pattern belongs to the pattern; the one FcFontSort() writes belongs to the
 * caller (FcCharSetDestroy). fromPointer() trusts any address other than 0.
 */
final class FcCharSet
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): static {}
}

function FcNameParse(string $name): ?FcPattern {}

function FcPatternDestroy(FcPattern $p): void {}

/** $s is a copy of the string the pattern holds, or null when the result is not FcResultMatch. */
function FcPatternGetString(FcPattern $p, string $object, int $n, ?string &$s): int {}

/** $i is null when the result is not FcResultMatch. */
function FcPatternGetInteger(FcPattern $p, string $object, int $n, ?int &$i): int {}

/** $c belongs to $p, or is null when the result is not FcResultMatch. */
function FcPatternGetCharSet(FcPattern $p, string $object, int $n, ?FcCharSet &$c): int {}

function FcCharSetHasChar(FcCharSet $fcs, int $ucs4): bool {}

function FcCharSetDestroy(FcCharSet $fcs): void {}
