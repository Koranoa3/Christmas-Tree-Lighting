# ESP32 Christmas Light Controller

学校のクリスマスイベントに向けたツリー装飾用のESP32ベースのライトコントローラー。
パターンは一旦固定。時間があれば、Wifi・外部接続から制御するかも

- C++
- FastLED
- ESP32
- PlatformIO：ライブラリ管理とビルド環境
- Wokwi：シミュレーション

config.hでLEDの数や明るさ、デフォルトのモードなどを設定
main.cppでLEDの初期化とメインループを実装
patterns.cppでパターン定義