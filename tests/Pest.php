<?php

declare(strict_types=1);

if (! extension_loaded('fontconfig')) {
    throw new RuntimeException('The fontconfig extension is not loaded; run ./install-macos.sh or ./install-debian-trixie.sh');
}

/** Signal numbers a child process dies by (SIGBUS is 10 on macOS, 7 on Linux). */
const PROBE_SIGABRT = 6;
const PROBE_SIGBUS = PHP_OS_FAMILY === 'Darwin' ? 10 : 7;
const PROBE_SIGSEGV = 11;

/**
 * Skips a tests/Probes test unless the loaded ext-fontconfig is a probe build (--enable-fontconfig-probes),
 * the only build where env PHPFC_PROBE_OFF turns a kept refusal off.
 */
function requireProbeBuild(): void
{
    if (! defined('PHPFC_PROBES')) {
        test()->markTestSkipped('ext-fontconfig is not a probe build (configure --enable-fontconfig-probes): PHPFC_PROBES is undefined');
    }
}

/**
 * Runs $code in a child of this PHP, with the same ini configuration. The code is a function body: what it
 * returns comes back as 'result' (JSON), a throw as 'threw' ("Class: message"). $siteOff names the kept site
 * env PHPFC_PROBE_OFF turns off (a probe build only); null keeps every site.
 *
 * @return array{signal: ?int, exit: ?int, result: mixed, threw: ?string, survived: bool, stdout: string, stderr: string}
 */
function fcChild(string $code, ?string $siteOff = null): array
{
    $script = <<<PHP
    <?php
    declare(strict_types=1);
    try {
        \$result = (static function () {
    {$code}
        })();
        echo "\\nRESULT ", json_encode(\$result, JSON_PRESERVE_ZERO_FRACTION | JSON_PARTIAL_OUTPUT_ON_ERROR), "\\n";
    } catch (Throwable \$t) {
        echo "\\nREFUSED ", get_class(\$t), ': ', \$t->getMessage(), "\\n";
    }
    echo "SURVIVED\\n";
    PHP;

    $base = tempnam(sys_get_temp_dir(), 'fc-child');
    $file = $base.'.php';
    file_put_contents($file, $script);

    $env = getenv();
    unset($env['PHPFC_PROBE_OFF']);
    $env['XDEBUG_MODE'] = 'off';
    if ($siteOff !== null) {
        $env['PHPFC_PROBE_OFF'] = $siteOff;
    }

    // No -n: the child reads the same ini files and PHP_INI_SCAN_DIR as this PHP, so it loads the same module.
    $process = proc_open([PHP_BINARY, '-d', 'memory_limit=128M', $file],
        [0 => ['file', '/dev/null', 'r'], 1 => ['pipe', 'w'], 2 => ['pipe', 'w']], $pipes, null, $env);
    $stdout = (string) stream_get_contents($pipes[1]);
    $stderr = (string) stream_get_contents($pipes[2]);
    fclose($pipes[1]);
    fclose($pipes[2]);
    do {
        $status = proc_get_status($process);
    } while ($status['running'] && usleep(10_000) === null);
    proc_close($process);
    @unlink($base);
    @unlink($file);

    return [
        'signal' => $status['signaled'] ? $status['termsig'] : null,
        'exit' => $status['signaled'] ? null : $status['exitcode'],
        'result' => preg_match('/^RESULT (.*)$/m', $stdout, $m) === 1 ? json_decode($m[1], true) : null,
        'threw' => preg_match('/^REFUSED (.*)$/m', $stdout, $m) === 1 ? $m[1] : null,
        'survived' => str_contains($stdout, "SURVIVED\n"),
        'stdout' => $stdout,
        'stderr' => $stderr,
    ];
}

/**
 * The current configuration and a substituted pattern for $name, ready for FcFontMatch or FcFontSort.
 *
 * @return array{0: FcConfig, 1: FcPattern}
 */
function substituted(string $name): array
{
    $config = FcInitLoadConfigAndFonts() ?? throw new RuntimeException('FcInitLoadConfigAndFonts failed');
    $pattern = FcNameParse($name) ?? throw new RuntimeException("FcNameParse failed for {$name}");
    FcConfigSubstitute($config, $pattern, FcMatchPattern);
    FcDefaultSubstitute($pattern);

    return [$config, $pattern];
}
