#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7ba76658, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xdf9dcf72, __VMLINUX_SYMBOL_STR(no_llseek) },
	{ 0x1a52dccf, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0xa4875d68, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0xf4fa543b, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0xed88d04c, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xc3cbdffb, __VMLINUX_SYMBOL_STR(i2c_transfer) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x36bab7, __VMLINUX_SYMBOL_STR(i2c_master_send) },
	{ 0x86f9204c, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xe2e8065e, __VMLINUX_SYMBOL_STR(memdup_user) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x4a9863e1, __VMLINUX_SYMBOL_STR(mutex_trylock) },
	{ 0x787ec31d, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x2528437c, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x8621c74f, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0xeecfc88d, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x820592c7, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0x7c7ff5f7, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x401052cf, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xc2b4bd88, __VMLINUX_SYMBOL_STR(class_unregister) },
	{ 0x463b8bd1, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x73e20c1c, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0x7f1e3f1a, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:chip_i2c");
