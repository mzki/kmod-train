#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/printk.h>
#include <linux/jiffies.h>

#include "dep_timer.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mzki");
MODULE_DESCRIPTION("timer kernel module");
MODULE_VERSION("0.0.1");

static const char * const MOD_NAME           = "timer module";
static const unsigned long     DEFAULT_TIMER_MSEC = 100; 

static struct timer_list g_timer_list;

//
// static functions
//

static void timer_handler_works_fine(unsigned long arg) {
        pr_info("%s: timer works fine. arg 0x%lx\n", MOD_NAME, arg);
}

static inline unsigned long msec_after(unsigned long msec) {
        return jiffies + msecs_to_jiffies(msec);
}

static int __init mod_init(void) {
        // firstly init required.
        init_timer(&g_timer_list);
        delayed_func_after(timer_handler_works_fine, DEFAULT_TIMER_MSEC, 1);

        pr_info("%s: installed\n", MOD_NAME);
        return 0;
}

static void __exit mod_exit(void) {
        delayed_func_stop();
        pr_info("%s: uninstalled\n", MOD_NAME);
}

module_init(mod_init);
module_exit(mod_exit);

//
// Exported functions
// 

void delayed_func_after(delayed_func_t func, unsigned long msec, unsigned long data) {
        if (func == NULL) {
                return;
        }

        delayed_func_stop();
        g_timer_list.expires  = msec_after(msec);
        g_timer_list.data     = data;
        g_timer_list.function = func;
        add_timer(&g_timer_list);
}
EXPORT_SYMBOL(delayed_func_after);

void delayed_func_stop() {
        if (timer_pending(&g_timer_list)) {
                del_timer(&g_timer_list);
                init_timer(&g_timer_list);
        }
}
EXPORT_SYMBOL(delayed_func_stop);

void delayed_func_reset(unsigned long msec) {
        if (timer_pending(&g_timer_list)) {
                delayed_func_t prev_func = g_timer_list.function;
                unsigned long  prev_data = g_timer_list.data;

                del_timer(&g_timer_list);
                init_timer(&g_timer_list);

                g_timer_list.function = prev_func;
                g_timer_list.data     = prev_data;
        }
        mod_timer(&g_timer_list, msec_after(msec));
}
EXPORT_SYMBOL(delayed_func_reset);
