/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2002-2003  David McCullough <davidm@snapgear.com>
 * Copyright (C) 1998       Kenneth Albanowski <kjahds@kjahds.com>
 *                          The Silver Hammer Group, Ltd.
 *
 * This file provides the definitions and structures needed to
 * support uClinux flat-format executables.
 */
#ifndef _LINUX_FLAT_H
#define _LINUX_FLAT_H

#include <uapi/linux/flat.h>
#include <asm/flat.h>

/*
 * While it would be nice to keep this header clean,  users of older
 * tools still need this support in the kernel.  So this section is
 * purely for compatibility with old tool chains.
 *
 * DO NOT make changes or enhancements to the old format please,  just work
 *        with the format above,  except to fix bugs with old format support.
 */

#include <asm/byteorder.h>

#define	OLD_FLAT_VERSION			0x00000002L
#define OLD_FLAT_RELOC_TYPE_TEXT	0
#define OLD_FLAT_RELOC_TYPE_DATA	1
#define OLD_FLAT_RELOC_TYPE_BSS		2

typedef union {
	uint32_t	value;
	struct {
# if defined(mc68000) && !defined(CONFIG_COLDFIRE)
		int32_t offset : 30;
		uint32_t type : 2;
#   	define OLD_FLAT_FLAG_RAM    0x1 /* load program entirely into RAM */
# elif defined(__BIG_ENDIAN_BITFIELD)
		int32_t type : 2;
		uint32_t offset : 30;
#   	define OLD_FLAT_FLAG_RAM    0x1 /* load program entirely into RAM */
# elif defined(__LITTLE_ENDIAN_BITFIELD)
		int32_t offset : 30;
		uint32_t type : 2;
#   	define OLD_FLAT_FLAG_RAM    0x1 /* load program entirely into RAM */
# else
#   	error "Unknown bitfield order for flat files."
# endif
	} reloc;
} flat_v2_reloc_t;

#endif /* _LINUX_FLAT_H */
