/*
 * Copyright (C) 2018 ZhangXinlin <zhangxinlin@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/mm.h>
#include <linux/memblock.h>
#include <linux/sched.h>
#include <linux/initrd.h>
#include <linux/console.h>
#include <linux/screen_info.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>
#include <linux/sched/task.h>
#include <linux/swiotlb.h>

#include <asm/setup.h>
#include <asm/sections.h>
#include <asm/pgtable.h>
#include <asm/smp.h>
#include <asm/sbi.h>
#include <asm/tlbflush.h>
#include <asm/thread_info.h>

volatile uint64_t tohost __attribute__((section(".htif")));
volatile uint64_t fromhost __attribute__((section(".htif")));
spinlock_t htif_lock;
volatile int htif_console_buf;

#define TOHOST(base_int)	(uint64_t *)(base_int + TOHOST_OFFSET)
#define FROMHOST(base_int)	(uint64_t *)(base_int + FROMHOST_OFFSET)

#define TOHOST_OFFSET		((uintptr_t)tohost - (uintptr_t)__htif_base)
#define FROMHOST_OFFSET		((uintptr_t)fromhost - (uintptr_t)__htif_base)

#if 1 //__riscv_xlen == 64
# define TOHOST_CMD(dev, cmd, payload) \
  (((uint64_t)(dev) << 56) | ((uint64_t)(cmd) << 48) | (uint64_t)(payload))
#else
# define TOHOST_CMD(dev, cmd, payload) ({ \
  if ((dev) || (cmd)) __builtin_trap(); \
  (payload); })
#endif
#define FROMHOST_DEV(fromhost_value) ((uint64_t)(fromhost_value) >> 56)
#define FROMHOST_CMD(fromhost_value) ((uint64_t)(fromhost_value) << 8 >> 56)
#define FROMHOST_DATA(fromhost_value) ((uint64_t)(fromhost_value) << 16 >> 16)

static void __check_fromhost(void)
{
  uint64_t fh = fromhost;
  if (!fh)
    return;
  fromhost = 0;

  // this should be from the console
  /// assert(FROMHOST_DEV(fh) == 1);
  switch (FROMHOST_CMD(fh)) {
    case 0:
      htif_console_buf = 1 + (uint8_t)FROMHOST_DATA(fh);
      break;
    case 1:
      break;
    default:
	;// assert(0);
  }
}

static void __set_tohost(uintptr_t dev, uintptr_t cmd, uintptr_t data)
{
  while (tohost)
    __check_fromhost();
  tohost = TOHOST_CMD(dev, cmd, data);
}

int sbi_console_getchar(void)
{
	spin_lock(&htif_lock);
    __check_fromhost();
    int ch = htif_console_buf;
    if (ch >= 0) {
      htif_console_buf = -1;
      __set_tohost(1, 0, 0);
    }
	spin_unlock(&htif_lock);

  return ch - 1;
}

void sbi_console_putchar(int ch)
{
    spin_lock(&htif_lock);
    __set_tohost(1, 1, ch);
    spin_unlock(&htif_lock);
}

void sbi_shutdown(void)
{
	while (1) {
		fromhost = 0;
		tohost = 1;
	}
}
