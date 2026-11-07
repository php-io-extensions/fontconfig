<?php

declare(strict_types=1);

it('binds the FC_WEIGHT_* and FC_SLANT_* values of fontconfig.h', function (string $name, int $value): void {
    expect(defined($name))->toBeTrue()
        ->and(constant($name))->toBe($value);
})->with([
    ['FC_WEIGHT_THIN', 0],
    ['FC_WEIGHT_EXTRALIGHT', 40],
    ['FC_WEIGHT_ULTRALIGHT', 40],
    ['FC_WEIGHT_LIGHT', 50],
    ['FC_WEIGHT_DEMILIGHT', 55],
    ['FC_WEIGHT_SEMILIGHT', 55],
    ['FC_WEIGHT_BOOK', 75],
    ['FC_WEIGHT_REGULAR', 80],
    ['FC_WEIGHT_NORMAL', 80],
    ['FC_WEIGHT_MEDIUM', 100],
    ['FC_WEIGHT_DEMIBOLD', 180],
    ['FC_WEIGHT_SEMIBOLD', 180],
    ['FC_WEIGHT_BOLD', 200],
    ['FC_WEIGHT_EXTRABOLD', 205],
    ['FC_WEIGHT_ULTRABOLD', 205],
    ['FC_WEIGHT_BLACK', 210],
    ['FC_WEIGHT_HEAVY', 210],
    ['FC_WEIGHT_EXTRABLACK', 215],
    ['FC_WEIGHT_ULTRABLACK', 215],
    ['FC_SLANT_ROMAN', 0],
    ['FC_SLANT_ITALIC', 100],
    ['FC_SLANT_OBLIQUE', 110],
]);

it('reads a parsed bold italic pattern back as FC_WEIGHT_BOLD and FC_SLANT_ITALIC', function (): void {
    $pattern = FcNameParse('monospace:weight=bold:slant=italic');

    expect(FcPatternGetInteger($pattern, FC_WEIGHT, 0, $weight))->toBe(FcResultMatch)
        ->and($weight)->toBe(FC_WEIGHT_BOLD)
        ->and(FcPatternGetInteger($pattern, FC_SLANT, 0, $slant))->toBe(FcResultMatch)
        ->and($slant)->toBe(FC_SLANT_ITALIC);

    FcPatternDestroy($pattern);
});
