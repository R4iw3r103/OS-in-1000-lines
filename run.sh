#!/bin/bash
# OpenSBIを起動するスクリプト
set -xue

# QEMUのパスを指定
QEMU=/opt/homebrew/bin/qemu-system-riscv32

# clangのパス
CC=/opt/homebrew/opt/llvm/bin/clang

CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib"
# -std=c11: C11を使用, -02: 最適化を有効
# -g3: デバック情報を最大限出力, -Wall: 主要な警告を有効
# -Wextra: さらに追加の警告を有効, --target=riscv32: 32ビット用RISC-V用にコンパイル
# -ffreestanding: ホスト環境の標準ライブラリを使用しない
# -nostdlib: 標準ライブラリをリンクしない
# -Wl, -Tkernel.ld: 隣家スクリプト指定
# -Wl, -Map=kernel.map: マップファイルを出力

OBJCOPY=/opt/homebrew/opt/llvm/bin/llvm-objcopy

# シェルをビルド
$CC $CFLAGS -Wl,-Tuser.ld -Wl,-Map=shell.map -o shell.elf shell.c user.c common.c
  # ビルドしたELFファイルを生バイナリ(ベースアドレスから実際にメモリ上に展開される内容が入ったもの)形式に変換
$OBJCOPY --set-section-flags .bss=alloc,contents -O binary shell.elf shell.bin
  # 生バイナリ形式の実行イメージをC言語に埋め込める形式に変換する処理
$OBJCOPY -Ibinary -Oelf32-littleriscv shell.bin shell.bin.o

# カーネルをビルド
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
  kernel.c common.c shell.bin.o

$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
  -kernel kernel.elf
# -machine: virtマシンとして起動
# -bios default: デフォルトのBIOS(ここではOpenSBI)を使用
# -nographic: ウィンドウなしでQEMUを起動
# -serial mon:stdio: QEMUの標準入出力を仮想マシンのシリアルポートに接続。mon:指定によりQEMUモニターへの切り替えも可能に
# --no-reboot: 仮想マシンがクラッシュしたら、再起動せずに停止させる
