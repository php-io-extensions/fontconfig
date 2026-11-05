<?php

declare(strict_types=1);

it('matches monospace to an existing font file', function (): void {
    $config = FcInitLoadConfigAndFonts();
    $pattern = FcNameParse('monospace');

    expect($config)->toBeInstanceOf(FcConfig::class)
        ->and($pattern)->toBeInstanceOf(FcPattern::class)
        ->and(FcConfigSubstitute($config, $pattern, FcMatchPattern))->toBeTrue();
    FcDefaultSubstitute($pattern);

    $match = FcFontMatch($config, $pattern, $result);
    expect($result)->toBe(FcResultMatch)
        ->and($match)->toBeInstanceOf(FcPattern::class)
        ->and(FcPatternGetString($match, FC_FILE, 0, $file))->toBe(FcResultMatch)
        ->and(is_file($file))->toBeTrue()
        ->and(FcPatternGetInteger($match, FC_INDEX, 0, $index))->toBe(FcResultMatch)
        ->and($index)->toBeGreaterThanOrEqual(0);

    FcPatternDestroy($match);
    FcPatternDestroy($pattern);
    FcConfigDestroy($config);
});

it('finds a fallback font that has U+4E2D', function (): void {
    [$config, $pattern] = substituted('monospace');
    $set = FcFontSort($config, $pattern, true, $coverage, $result);

    expect($result)->toBe(FcResultMatch)
        ->and($set)->toBeInstanceOf(FcFontSet::class)
        ->and($set->nfont)->toBe(count($set->fonts))
        ->and($coverage)->toBeInstanceOf(FcCharSet::class)
        ->and(FcCharSetHasChar($coverage, 0x4E2D))->toBeTrue();

    $has = array_filter($set->fonts, fn (FcPattern $font): bool => FcPatternGetCharSet($font, FC_CHARSET, 0, $chars) === FcResultMatch
        && FcCharSetHasChar($chars, 0x4E2D));
    expect($has)->not->toBeEmpty();

    FcCharSetDestroy($coverage);
    FcFontSetDestroy($set);
});

it('keeps a pattern two sorted sets share usable after the first set is destroyed', function (): void {
    [$config, $pattern] = substituted('monospace');
    $first = FcFontSort($config, $pattern, false, $firstCoverage, $result);
    $second = FcFontSort($config, $pattern, false, $secondCoverage, $result);
    $shared = $first->fonts[0];

    expect($second->fonts[0])->toBe($shared);
    FcFontSetDestroy($first);

    expect(FcPatternGetString($shared, FC_FILE, 0, $file))->toBe(FcResultMatch)
        ->and(is_file($file))->toBeTrue();

    FcFontSetDestroy($second);
    FcCharSetDestroy($firstCoverage);
    FcCharSetDestroy($secondCoverage);
});

it('answers FcResultNoMatch and null for a property the pattern lacks', function (): void {
    expect(FcPatternGetString(FcNameParse('monospace'), FC_FILE, 0, $file))->toBe(FcResultNoMatch)
        ->and($file)->toBeNull();
});

it('substitutes in the current configuration when config is null', function (): void {
    $pattern = FcNameParse('monospace');

    expect(FcConfigSubstitute(null, $pattern, FcMatchPattern))->toBeTrue();
    FcDefaultSubstitute($pattern);
    expect(FcFontMatch(null, $pattern, $result))->toBeInstanceOf(FcPattern::class)
        ->and($result)->toBe(FcResultMatch);
});
