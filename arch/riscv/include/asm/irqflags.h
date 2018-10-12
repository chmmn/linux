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


#ifndef _ASM_RISCV_IRQFLAGS_H
#define _ASM_RISCV_IRQFLAGS_H

#include <asm/processor.h>
#include <asm/csr.h>

#ifdef CONFIG_MACHINE_MODE
static inline unsigned long arch_local_save_flags(void)
{
	return csr_read(mstatus);
}

static inline void arch_local_irq_enable(void)
{
	csr_set(mstatus, SR_MIE);
}

/* unconditionally disable interrupts */
static inline void arch_local_irq_disable(void)
{
	csr_clear(mstatus, SR_MIE);
}

/* get status and disable interrupts */
static inline unsigned long arch_local_irq_save(void)
{
	return csr_read_clear(mstatus, SR_MIE);
}

/* test flags */
static inline int arch_irqs_disabled_flags(unsigned long flags)
{
	return !(flags & SR_MIE);
}

/* test hardware interrupt enable bit */
static inline int arch_irqs_disabled(void)
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

/* set interrupt enabled status */
static inline void arch_local_irq_restore(unsigned long flags)
{
	csr_set(mstatus, flags & SR_MIE);
}

#else
/* read interrupt enabled status */
static inline unsigned long arch_local_save_flags(void)
{
	return csr_read(sstatus);
}

/* unconditionally enable interrupts */
static inline void arch_local_irq_enable(void)
{
	csr_set(sstatus, SR_SIE);
}

/* unconditionally disable interrupts */
static inline void arch_local_irq_disable(void)
{
	csr_clear(sstatus, SR_SIE);
}

/* get status and disable interrupts */
static inline unsigned long arch_local_irq_save(void)
{
	return csr_read_clear(sstatus, SR_SIE);
}

/* test flags */
static inline int arch_irqs_disabled_flags(unsigned long flags)
{
	return !(flags & SR_SIE);
}

/* test hardware interrupt enable bit */
static inline int arch_irqs_disabled(void)
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

/* set interrupt enabled status */
static inline void arch_local_irq_restore(unsigned long flags)
{
	csr_set(sstatus, flags & SR_SIE);
}
#endif /* MACHINE_MODE */

#endif /* _ASM_RISCV_IRQFLAGS_H */
