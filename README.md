# esp32_simple_wav_player
no I2S device required. Just connect ESP32 and speaker with two lines (GND/GPIO25).

## Description

ESP32に定型文をしゃべらせるシンプルな Wev Playerで、時計や天気予報などを作るベースとして用意しました。
外部回路がなくてもスピーカーを繋ぐだけで音声がでます。テスト用に複数のサンプリングレートの音声を聞き比べられるようにしました。

## Features

- 複数サンプリングレートのサポート(4KHz, 8KHz, 16KHz, ...)
- 8bit/16bitのデータサイズサポート
- 複数データの連続再生
- ...

## Requirement

- ESP32 x 1
- Speaker x 1
- (2KΩ 半固定抵抗、0.1μ コンデンサ)


## Usage

1. Audacity などのソフトで、PCM形式のWavファイルを用意します。ファイルヘッダから情報を取得するので、RAW(header-less)にはしません。
2. HxD　などのソフトで、音声ファイルをテキストに変換します。C言語の配列形式でデータを出力し、includeデータにします。

## Installation

Arduino IDE と Arduino core for the ESP32　でコンパイルし、ESP32に転送。

## Author

kghrlabo (http://kghr.blog.fc2.com/)

## License

TBD
