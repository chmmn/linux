/*
 *
 * Copyright (C) 2018 ZhangXinlin <zhangxinlin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _ASM_RISCV_PAGE_NOMMU_H
#define _ASM_RISCV_PAGE_NOMMU_H

#define clear_page(page)	memset((page), 0, PAGE_SIZE)
#define copy_page(to,from)	memcpy((to), (from), PAGE_SIZE)

#define clear_user_page(page, vaddr, pg)	clear_page(page)
#define copy_user_page(to, from, vaddr, pg)	copy_page(to, from)

typedef unsigned long pte_t;
typedef unsigned long pmd_t;
typedef unsigned long pgd_t[2];
typedef unsigned long pgprot_t;
typedef struct page *pgtable_t;

#define pte_val(x)      (x)
#define pmd_val(x)      (x)
#define pgd_val(x)	((x)[0])
#define pgprot_val(x)   (x)

#define __pte(x)        (x)
#define __pmd(x)        (x)
#define __pgprot(x)     (x)

#endif /* _ASM_RISCV_PAGE_NOMMU_H */
