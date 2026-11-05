<?php

declare(strict_types=1);

// Site fontconfig-null-pattern (outcome 1): FcConfigSubstitute and FcDefaultSubstitute dereference the
// pattern they are given; FcFontMatch and FcFontSort assert it is not NULL and abort. A pattern that holds
// NULL (destroyed by FcPatternDestroy, or made by fromPointer(0)) gives them NULL.

beforeEach(fn () => requireProbeBuild());

it('refuses a destroyed pattern in FcConfigSubstitute, which dereferences it (SIGSEGV) without the refusal', function (): void {
    $code = <<<'PHP'
        $pattern = FcNameParse('monospace');
        FcPatternDestroy($pattern);

        return FcConfigSubstitute(null, $pattern, FcMatchPattern);
    PHP;

    $kept = fcChild($code);
    $off = fcChild($code, 'fontconfig-null-pattern');

    expect($kept['signal'])->toBeNull()
        ->and($kept['threw'])->toBe('ValueError: FcConfigSubstitute(): Argument #2 ($p) holds NULL: it was destroyed, or made from address 0')
        ->and($off['signal'])->toBe(PROBE_SIGSEGV);
});

it('refuses a NULL pattern in FcFontMatch, which asserts and aborts (SIGABRT) without the refusal', function (): void {
    $code = 'return FcFontMatch(null, FcPattern::fromPointer(0), $result);';

    $kept = fcChild($code);
    $off = fcChild($code, 'fontconfig-null-pattern');

    expect($kept['threw'])->toBe('ValueError: FcFontMatch(): Argument #2 ($p) holds NULL: it was destroyed, or made from address 0')
        ->and($off['signal'])->toBe(PROBE_SIGABRT);
});

it('passes a NULL pattern to FcPatternGetString, which answers FcResultNoMatch', function (): void {
    expect(fcChild('return FcPatternGetString(FcPattern::fromPointer(0), FC_FAMILY, 0, $s);')['result'])->toBe(1);
});
