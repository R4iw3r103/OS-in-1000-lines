#pragma once

// __FILE__: エラーが起きたファイル名、__LINE__: 行番号
// ##__VA_ARGS__: 可変超引数を受け取るマクロを定義するときに使える。空の時に直前の,を削除して辻褄を合わせる
#define PANIC(fmt, ...) \
  do { \
    printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    while (1) {} \
  } while (0)
struct sbiret {
  long error;
  long value;
};
