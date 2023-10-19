/* boot関数をエントリポイントに */
ENTRY(boot)

SECTIONS {
  . = 0x80200000; /* ベースアドレスを0x80200000に */

  /* .text(コード領域)セクションの配置 */
  .text :{
    KEEP(*(.text.boot)); /* .text.bootセクションを先頭に */
    *(.text .text.*); /* 全ファイル中の.textと.text.で始まる名前のセクションを配置する */
  }

  /* .rodata(定数データ領域、ReadOnly)セクションの配置 */
  .rodata : ALIGN(4) {
    *(.rodata .rodata.*);
  }

  /* .data(読み書き可能データ領域)セクションの配置 */
  .data : ALIGN(4) {
    *(.data .data.*);
  }

  /* .bss(読み書き可能データ領域だが、初期値が0の変数を配置する)セクションの配置 */
  .bss : ALIGN(4) {
    __bss = .; /* __bssというシンボル(関数や静的変数)に.(l現在アドレス)を割り当て */
    *(.bss .bss.* .sbss .sbss.*);
    __bss_end = .;
  }

  . = ALIGN(4); /* 4バイト境界になるようにアドレスを調整する */
  . += 128 * 1024; /* 128KB */
  __stack_top = .;

  /* 動的割り当て可能なメモリ領域を定義 */
  . =ALIGN(4096); /* 4KB境界に配置されるように */
  __free_ram = .;
  . += 64 * 1024 * 1024; /* 64MB */
  __free_ram_end = .;
}
