<?php

declare(strict_types=1);

require_once __DIR__.'/../scripts/support.php';

it('counts the FcPublic functions of each header, across lines, sorted, once each, not in comments or macros', function (): void {
    $header = "FcPublic FcPattern *\nFcNameParse (const FcChar8 *name);\n"
        ."FcPublic FcBool\nFcConfigSubstitute (FcConfig\t*config,\n\t\t    FcPattern\t*p,\n\t\t    FcMatchKind\tkind);\n"
        ."/* FcPublic void FcCommentedOut (void); */\n"
        ."#define FcPublic _FCFUNCPROTOBEGIN\n";

    expect(support_counted(['fontconfig.h' => $header]))
        ->toBe(['fontconfig.h' => ['FcConfigSubstitute', 'FcNameParse']]);
});

it('reads the top-level functions of a stub, not class methods', function (): void {
    $stub = "<?php\nfunction FcNameParse(string \$name): ?FcPattern {}\nfinal class FcPattern { public function pointer(): int {} }\nfunction FcPatternDestroy(FcPattern \$p): void {}\n";

    expect(support_functions($stub))->toBe(['FcNameParse', 'FcPatternDestroy']);
});

it('renders one line per function and lists the bindings that match none', function (): void {
    $md = support_render(['fontconfig.h' => ['FcNameParse', 'FcPatternDestroy']], ['FcNameParse' => true, 'fc_helper' => true], 'fontconfig 2.15.0', '2026-11-05');

    expect($md)->toContain('- [x] `FcNameParse`')
        ->toContain('- [ ] `FcPatternDestroy`')
        ->toContain('## fontconfig.h (1 / 2)')
        ->toContain('fontconfig 2.15.0 — 1 / 2 members bound (50.0%) — 2026-11-05')
        ->toContain('- `fc_helper`');
});

it('computes the badge from the counts', function (): void {
    expect(support_badge('fontconfig API', 15, 300))
        ->toBe('[![fontconfig API](https://img.shields.io/badge/fontconfig_API-5.0%25_%2815%2F300%29-blue?style=flat-square)](SUPPORT.md)');
});

it('compares a support list without its date', function (): void {
    expect(support_same("fontconfig 2.15.0 — 2 / 4 members bound (50.0%) — 2026-11-05\n", "fontconfig 2.15.0 — 2 / 4 members bound (50.0%) — 2026-11-09\n"))->toBeTrue();
});
