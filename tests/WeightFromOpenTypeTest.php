<?php

declare(strict_types=1);

it('converts an OpenType weight to fontconfig\'s weight scale', function (float $openType, float $fontconfig): void {
    expect(FcWeightFromOpenTypeDouble($openType))->toBe($fontconfig);
})->with([
    'thin' => [100.0, 0.0],
    'extra light' => [200.0, 40.0],
    'regular' => [400.0, 80.0],
    'medium' => [500.0, 100.0],
    'bold' => [700.0, 200.0],
    'black' => [900.0, 210.0],
    'between regular and medium' => [450.0, 90.0],
    'past the table' => [1200.0, 215.0],
    'negative' => [-1.0, -1.0],
]);
