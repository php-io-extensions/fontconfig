<?php

declare(strict_types=1);

// Site fontconfig-null-record (outcome 1): FcFontSet's nfont, sfont and fonts read the native set. A set
// that holds NULL (destroyed by FcFontSetDestroy, or made by fromPointer(0)) has no set to read.

beforeEach(fn () => requireProbeBuild());

it('refuses a field read through a destroyed font set, which dereferences NULL (SIGSEGV) without the refusal', function (): void {
    $code = <<<'PHP'
        $pattern = FcNameParse('monospace');
        FcConfigSubstitute(null, $pattern, FcMatchPattern);
        FcDefaultSubstitute($pattern);
        $set = FcFontSort(null, $pattern, true, $coverage, $result);
        FcFontSetDestroy($set);

        return $set->nfont;
    PHP;

    $kept = fcChild($code);
    $off = fcChild($code, 'fontconfig-null-record');

    expect($kept['signal'])->toBeNull()
        ->and($kept['threw'])->toBe('ValueError: FcFontSet holds NULL: it was destroyed, or made from address 0')
        ->and($off['signal'])->toBe(PROBE_SIGSEGV);
});
