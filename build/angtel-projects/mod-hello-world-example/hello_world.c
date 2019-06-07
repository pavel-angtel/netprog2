// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2019 JSC Angstrem-Telecom. All rights reserved.

#include <linux/module.h>
#include <linux/kernel.h>

static int kernel_module_init(void)
{
	printk(KERN_INFO "Hello, World!\n");

	return 0;
}

static void kernel_module_exit(void)
{
	printk(KERN_INFO "Goodbye, World.\n");
}

module_init(kernel_module_init);
module_exit(kernel_module_exit);

MODULE_AUTHOR("JSC Angstrem-Telecom");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello, World!");
