/*
 * Copyright (C) 2012 Regents of the University of California
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


#ifndef _ASM_RISCV_MMU_H
#define _ASM_RISCV_MMU_H

#ifndef __ASSEMBLY__

typedef struct {
#ifdef CONFIG_MMU
	void *vdso;
#else
	unsigned long end_brk;
#ifdef CONFIG_BINFMT_ELF_FDPIC
	unsigned long exec_fdpic_loadmap;
	unsigned long interp_fdpic_loadmap;
#endif
#endif
#ifdef CONFIG_SMP
	/* A local icache flush is needed before user execution can resume. */
	cpumask_t icache_stale_mask;
#endif
} mm_context_t;
#endif /* __ASSEMBLY__ */

#endif /* _ASM_RISCV_MMU_H */
