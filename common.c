#include "common.h"

void putchar(char ch);

void printf(const char *fmt, ...) {
    va_list vargs;
    va_start(vargs, fmt);

    while (*fmt) {
        // %で始まった場合の処理
        if (*fmt == '%') {
            fmt++; // %の次の文字を見るためにインクリメント
            switch (*fmt) {
                // ヌル文字の場合は%を出力してendへジャンプ
                case '\0':
                    putchar('%');
                    goto end;
                // %の場合はそのまま出力
                case '%':
                    putchar('%');
                    break;
                // %s: 文字列出力
                case 's': {
                    const char *s = va_arg(vargs, const char *);
                    while (*s) {
                        putchar(*s);
                        s++;
                    }
                    break;
                }
                // %d: 10進数出力
                case 'd': {
                    int value = va_arg(vargs, int);
                    // 負の値だった場合は-を出力して絶対値に変換
                    if (value < 0) {
                        putchar('-');
                        value = -value;
                    }

                    // 最上位の桁を求める
                    int divisor = 1;
                    while (value / divisor > 9)
                        divisor *= 10;
                    // 最上位の桁から順に出力
                    while (divisor > 0) {
                        putchar('0' + value / divisor);
                        value %= divisor;
                        divisor /= 10;
                    }
                    break;
                }
                // %x: 16進数出力
                case 'x': {
                    int value = va_arg(vargs, int);
                     // ニブル（16進数の1桁）から順に出力。右シフトを用いて何文字目かで抽出。
                    for (int i = 7; i >= 0; i--) {
                        int nibble = (value >> (i * 4)) & 0xf;
                        putchar("0123456789abcdef"[nibble]);
                    }
                }
            }
        } else {
            // % じゃなかったらそのまま出力
            putchar(*fmt);
        }
        fmt++;
    }
end:
    va_end(vargs);
}

// bufの先頭からnバイト分をcで埋める
void *memset(void *buf, char c, size_t n) {
    uint8_t *p = (uint8_t *) buf;
    while (n--)
        *p++ = c;
    return buf;
}

// srcからnバイト分をdstにコピーする
void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t *d = (uint8_t *) dst;
    const uint8_t *s = (const uint8_t *) src;
    while (n--)
        *d++ = *s++;
    return dst;
}

// srcの文字列をdstにコピーする
char *strcpy(char *dst, const char *src) {
    char *d = dst;
    while (*src)
        *d++ = *src++;
    *d = '\0';
    return dst;
}

// s1とs2を比較し、等しければ0を、s1が大きければ正の値を、s2が大きければ負の値を返す
// e.g. if !strcmp(s1, s2) => s1 == s2
int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2)
            break;
        s1++;
        s2++;
    }

    return *(unsigned char *)s1 - *(unsigned char *)s1;
}
