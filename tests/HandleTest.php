<?php

declare(strict_types=1);

it('keeps a handle when another class boxes its address', function (): void {
    $pattern = FcNameParse('monospace');
    $address = $pattern->pointer();

    FcCharSet::fromPointer($address);

    expect($pattern->pointer())->toBe($address)
        ->and(FcPattern::fromPointer($address))->toBe($pattern);
});

it('answers isset and var_dump from the native font set', function (): void {
    [$config, $pattern] = substituted('monospace');
    $set = FcFontSort($config, $pattern, false, $coverage, $result);

    ob_start();
    var_dump($set);
    $dump = (string) ob_get_clean();

    expect(isset($set->nfont))->toBeTrue()
        ->and(empty($set->fonts))->toBeFalse()
        ->and($dump)->toContain('["nfont"]=>'."\n  int({$set->nfont})")
        ->and($dump)->not->toContain('uninitialized');

    FcFontSetDestroy($set);
    FcCharSetDestroy($coverage);
});

it('answers isset false for a font set that holds NULL, without throwing', function (): void {
    expect(isset(FcFontSet::fromPointer(0)->nfont))->toBeFalse();
});

it('uses the current configuration for a destroyed FcConfig, as fontconfig does for NULL', function (): void {
    $config = FcInitLoadConfigAndFonts();
    FcConfigDestroy($config);
    $pattern = FcNameParse('monospace');

    expect($config->pointer())->toBe(0)
        ->and(FcConfigSubstitute($config, $pattern, FcMatchPattern))->toBeTrue();
    FcDefaultSubstitute($pattern);
    expect(FcFontMatch($config, $pattern, $result))->toBeInstanceOf(FcPattern::class)
        ->and($result)->toBe(FcResultMatch);
});
