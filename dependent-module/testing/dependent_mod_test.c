#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#include "dep_timer_stub.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mzki");
MODULE_DESCRIPTION("unit testing for dependent module");
MODULE_VERSION("0.0.1");

static int  __init utest_mod_init(void);
static void __exit utest_mod_exit(void);

module_init(utest_mod_init);
module_exit(utest_mod_exit);

// remove module_init/exit from test target 
// after module_init/exit is called.
#undef module_init
#undef module_exit
#define module_init(x) 
#define module_exit(x) 

// include test target
#include "../dependent_mod.c"

// 
// setup/teardown for each unit test
//
static void setup(void) {
       delayed_func_call_args_init(); 
}

static void teardown(void) {
        // do nothing
}

//
// init/exit
//
static int __init utest_mod_init(void) {

        setup();
        {
                const char const *test_name      = "mod_init calls delayed_func_after";
                int ret;

                ret = mod_init();
                if (ret != 0) {
                        printk("Fail: %s: mod_init erorr. got %d\n", test_name, ret);
                        return -1;
                }

                if (g_delayed_func_call_args.func != tick_handler) {
                        printk("Fail: %s: different handler. expect  0x%lx, got 0x%lx\n", 
                                test_name, (unsigned long)tick_handler, (unsigned long)g_delayed_func_call_args.func);
                        return -1;
                }

                if (g_delayed_func_call_args.msec != TICK_MSEC) {
                        printk("Fail: %s: different msec. expect  %lu, got %lu\n", 
                                test_name, TICK_MSEC, g_delayed_func_call_args.msec);
                        return -1;
                }

                if (g_delayed_func_call_args.data != 0) {
                        printk("Fail: %s: different data. expect  %d, got %lu\n", 
                                test_name, 0, g_delayed_func_call_args.data);
                        return -1;
                }
        }
        teardown();

        setup();
        {
                const char const *test_name      = "tick_handler calls delayed_func_reset";
                delayed_func_t prev_func = NULL;

                mod_init();

                prev_func = g_delayed_func_call_args.func;

                // call tick_handler
                tick_handler(g_delayed_func_call_args.data);

                if (g_delayed_func_call_args.func != prev_func) {
                        printk("Fail: %s: different handler. expect  0x%lx, got 0x%lx\n", 
                                test_name, (unsigned long)prev_func, (unsigned long)g_delayed_func_call_args.func);
                        return -1;
                }

                if (g_delayed_func_call_args.msec != EVEN_TICK_MSEC) {
                        printk("Fail: %s: different msec. expect  %lu, got %lu\n", 
                                test_name, EVEN_TICK_MSEC, g_delayed_func_call_args.msec);
                        return -1;
                }

                if (g_delayed_func_call_args.data != 0) {
                        printk("Fail: %s: different data. expect  %d, got %lu\n", 
                                test_name, 0, g_delayed_func_call_args.data);
                        return -1;
                }

                // twice calling tick_handler
                tick_handler(g_delayed_func_call_args.data);

                if (g_delayed_func_call_args.func != prev_func) {
                        printk("Fail: %s: different handler. expect  0x%lx, got 0x%lx\n", 
                                test_name, (unsigned long)prev_func, (unsigned long)g_delayed_func_call_args.func);
                        return -1;
                }

                if (g_delayed_func_call_args.msec != ODD_TICK_MSEC) {
                        printk("Fail: %s: different msec. expect  %lu, got %lu\n", 
                                test_name, ODD_TICK_MSEC, g_delayed_func_call_args.msec);
                        return -1;
                }

                if (g_delayed_func_call_args.data != 0) {
                        printk("Fail: %s: different data. expect  %d, got %lu\n", 
                                test_name, 0, g_delayed_func_call_args.data);
                        return -1;
                }
        }
        teardown();

        setup();
        {
                const char const *test_name      = "mod_exit";
                delayed_func_t prev_func = NULL;

                mod_init();
                prev_func = g_delayed_func_call_args.func;

                mod_exit();

                if (g_delayed_func_call_args.func != NULL) {
                        printk("Fail: %s: different handler. expect  0x%lx, got 0x%lx\n", 
                                test_name, (unsigned long)prev_func, (unsigned long)g_delayed_func_call_args.func);
                        return -1;
                }

                if (g_delayed_func_call_args.msec != 0) {
                        printk("Fail: %s: different msec. expect  %d, got %lu\n", 
                                test_name, 0, g_delayed_func_call_args.msec);
                        return -1;
                }

                if (g_delayed_func_call_args.data != 0) {
                        printk("Fail: %s: different data. expect  %d, got %lu\n", 
                                test_name, 0, g_delayed_func_call_args.data);
                        return -1;
                }

        }
        teardown();

        printk("%s_test: Test PASS\n", MOD_NAME);
        return 0; // OK 
}

static void __exit utest_mod_exit(void) {
        // calling target module's exit
        mod_exit();
}
