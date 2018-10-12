// SPDX-License-Identifier: GPL-2.0
#include <linux/uaccess.h>

/* out-of-line parts */

#ifndef INLINE_COPY_FROM_USER
unsigned long _copy_from_user(void *to, const void __user *from, unsigned long n)
{
	unsigned long res = n;
	might_fault();
	if (likely(access_ok(VERIFY_READ, from, n))) {
		kasan_check_write(to, n);
		res = raw_copy_from_user(to, from, n);
	}
	if (unlikely(res))
		memset(to + (n - res), 0, res);
	return res;
}
EXPORT_SYMBOL(_copy_from_user);
#endif

#ifndef INLINE_COPY_TO_USER
unsigned long _copy_to_user(void __user *to, const void *from, unsigned long n)
{
	might_fault();
	if (likely(access_ok(VERIFY_WRITE, to, n))) {
		kasan_check_read(from, n);
#if 1
		memcpy(to, from, n);
		n = 0;
#else
		n = raw_copy_to_user(to, from, n);
		if (n) {
			printk("%s ret %d\n", __func__, n);
		}
#endif
	} else
		printk ("%s check fail\n", __func__);
	return n;
}
EXPORT_SYMBOL(_copy_to_user);
#endif
