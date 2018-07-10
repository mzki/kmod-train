#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#include <stdarg.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mzki");
MODULE_DESCRIPTION("simple independent kernel module");
MODULE_VERSION("0.0.1");


static int print_info_impl(const char *fmt, ...) {
        int ret;
        va_list args;

        va_start(args, fmt);
        ret = vprintk(fmt, args);
        va_end(args);

        return ret;
}

// export funtion pointer for unit testing
static int (*g_print_info)(const char *, ...) = print_info_impl;


static const char * const MOD_NAME = "simple module";

static int __init mod_init(void) {
        g_print_info("%s installed\n", MOD_NAME);
        return 0;
}

static void __exit mod_exit(void) {
        g_print_info("%s uninstalled\n", MOD_NAME);
}

module_init(mod_init);
module_exit(mod_exit);
