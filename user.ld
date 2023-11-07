ENTRY(start)

/* 0x1000000 ~ 0x18000000の間にアプリケーションの各データを配置 */
SECTIONS {
  . = 0x1000000;

  .text :{
    KEEP(*(.text.start));
    *(.text .text.*);
  }

  .rodata : ALIGN(4) {
    *(.rodata .rodata.*);
  }

  .data : ALIGN(4) {
    *(.data .data.*);
  }

  .bss : ALIGN(4) {
    *(.bss .bss.* .sbss .sbss.*);

    . = ALIGN(4);
    . += 64 * 1024; /* 64KB */
    __stack_top = .;

    /* 第1引数の条件が満たされてなければリンク処理(clangコマンド)を失敗させる */
    ASSERT(. < 0x1800000, "too large executable");
  }
}
