#ifndef CONFIG_H
#define CONFIG_H

// ===== ユーザー設定 =====
// このファイルを config.h にコピーして使用してください
// config.h は .gitignore に追加されているため、個人の設定を保持できます

#define NUM_LEDS 50             // LEDの数 (max 300)
#define BRIGHTNESS 80             // 全体の明るさ (0-255)
#define DEFAULT_BUTTERFLY_MODE false // デフォルトでバタフライモードを有効にする
#define DEFAULT_CENTER_PERCENT 50  // デフォルトの中央部分の割合 (0-100)
#define DEFAULT_ANIM_SPEED_INDEX 1 // デフォルトアニメーション速度インデックス
#define DEFAULT_PATTERN_INDEX 0 // デフォルトパターンインデックス

#endif // CONFIG_H
