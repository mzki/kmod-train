#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#include "dep_timer.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mzki");
MODULE_DESCRIPTION("dependent kernel module");
MODULE_VERSION("0.0.1");

static const char * const MOD_NAME = "dependent module";

static const unsigned long TICK_MSEC = 1000; // 1sec

static void tick_handler(unsigned long data) {
        static const unsigned long ODD_TICK_MSEC  = (unsigned long)(TICK_MSEC / 2);
        static const unsigned long EVEN_TICK_MSEC = (unsigned long)((double)TICK_MSEC * 1.5);

        static uint64_t tick_count = 0;

        unsigned long reset_msec = (tick_count & 0x1) == 1 ? ODD_TICK_MSEC : EVEN_TICK_MSEC;
        
        tick_count++;
        pr_info("%s: current tick = %llx\n", MOD_NAME, tick_count);
        delayed_func_reset(reset_msec);
}

static int __init mod_init(void) {
        delayed_func_after((delayed_func_t)tick_handler, TICK_MSEC, 0);
        pr_info("%s: installed\n", MOD_NAME);
        return 0;
}

static void __exit mod_exit(void) {
        delayed_func_stop();
        pr_info("%s: uninstalled\n", MOD_NAME);
}

module_init(mod_init);
module_exit(mod_exit);
