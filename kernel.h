#pragma once
#include "common.h"

#define PROCS_MAX 8      // 最大プロセス数
#define PROC_UNUSED 0    // 未使用のプロセス管理構造体
#define PROC_RUNNABLE 1  // 実行可能なプロセス
#define SATP_SV32 (1u << 31)
#define PAGE_V    (1 << 0) // 有効化ビット
#define PAGE_R    (1 << 1) // 読み込み可能
#define PAGE_W    (1 << 2) // 書き込み可能
#define PAGE_X    (1 << 3) // 実行可能
#define PAGE_U    (1 << 4) // ユーザモードでアクセス可能
struct sbiret {
  long error;
  long value;
};

struct process {
  int pid;                // プロセスID
  int state;              // プロセスの状態
  vaddr_t sp;             // コンテキストスイッチ時のスタックポインタ
  uint32_t *page_table;
  // カーネルスタック:
  // コンテキストスイッチ時のCPUレジスタ、関数の戻り先、各関数でのローカル変数などが入っている
  // これをプロセスごとに用意することで、別の実行コンテキストを持ち、スイッチで状態の保存と復元ができる
  uint8_t stack[8192];
};

struct trap_frame {
  uint32_t ra;
  uint32_t gp;
  uint32_t tp;
  uint32_t t0;
  uint32_t t1;
  uint32_t t2;
  uint32_t t3;
  uint32_t t4;
  uint32_t t5;
  uint32_t t6;
  uint32_t a0;
  uint32_t a1;
  uint32_t a2;
  uint32_t a3;
  uint32_t a4;
  uint32_t a5;
  uint32_t a6;
  uint32_t a7;
  uint32_t s0;
  uint32_t s1;
  uint32_t s2;
  uint32_t s3;
  uint32_t s4;
  uint32_t s5;
  uint32_t s6;
  uint32_t s7;
  uint32_t s8;
  uint32_t s9;
  uint32_t s10;
  uint32_t s11;
  uint32_t sp;
} __attribute__((packed));

// CSRレジスタの読みを行うためのマクロ
#define READ_CSR(reg) \
  ({ \
    unsigned long __tmp; \
    __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; \
  })

// CSRレジスタの書きを行うためのマクロ
#define WRITE_CSR(reg, value) \
  do { \
    uint32_t __tmp = (value); \
    __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
  } while (0)

// __FILE__: エラーが起きたファイル名、__LINE__: 行番号
// ##__VA_ARGS__: 可変超引数を受け取るマクロを定義するときに使える。空の時に直前の,を削除して辻褄を合わせる
#define PANIC(fmt, ...) \
  do { \
    printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    while (1) {} \
  } while (0)
