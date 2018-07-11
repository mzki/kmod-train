#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mzki");
MODULE_DESCRIPTION("unit testing for dep_timer module");
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

// include mod_test 
#include "../dep_timer.c"

//
// stub functions
//
static uint32_t g_timer_handler_counter = 0;

static void timer_handler_stub(unsigned long arg) {
        g_timer_handler_counter++;
}

// 
// setup/teardown for each unit test
//
static void setup(void) {
        g_timer_handler_counter = 0;
}

static void teardown(void) {
        // do nothing
}

//
// init/exit
//
static int __init utest_mod_init(void) {
        // calling target modules' init
        mod_init();

        setup();
        {
                const char const *test_name      = "delayed_func_after";
                const uint32_t    expect_called  = 1;
                const unsigned long msec         = 10;

                delayed_func_after((delayed_func_t)timer_handler_stub, msec, 0);
                msleep(2*msec);

                if (g_timer_handler_counter != expect_called) {
                        printk("Fail: %s: different timer handler count. expect  %u, got %u\n", 
                                test_name, expect_called, g_timer_handler_counter);
                        return -1;
                }
        }
        teardown();

        setup();
        {
                const char const *test_name      = "delayed_func_stop";
                const uint32_t    expect_called  = 0;
                const unsigned long msec         = 10;

                delayed_func_after((delayed_func_t)timer_handler_stub, msec, 0);
                mdelay(msec/2);
                delayed_func_stop();
                msleep(msec); // over the time the handler was supposed to be called.

                if (g_timer_handler_counter != expect_called) {
                        printk("Fail: %s: differenct timer handler count. expect  %u, got %u\n", 
                                test_name, expect_called, g_timer_handler_counter);
                        return -1;
                }
        }
        teardown();

        setup();
        {
                const char const *test_name      = "after stop restart timer";
                const uint32_t    expect_called  = 1;
                const unsigned long msec         = 10;

                delayed_func_after((delayed_func_t)timer_handler_stub, msec, 0);
                mdelay(msec/4);
                delayed_func_stop();

                // restart function
                delayed_func_after((delayed_func_t)timer_handler_stub, msec/2, 0);
                msleep(2*msec);

                if (g_timer_handler_counter != expect_called) {
                        printk("Fail: %s: differenct timer handler count. expect  %u, got %u\n", 
                                test_name, expect_called, g_timer_handler_counter);
                        return -1;
                }
        }
        teardown();

        setup();
        {
                const char const *test_name      = "delayed_func_reset";
                const uint32_t    expect_called1 = 1;
                const uint32_t    expect_called2 = 2;
                const unsigned long msec         = 10;

                delayed_func_after((delayed_func_t)timer_handler_stub, msec, 0);
                mdelay(msec/4);

                // restart function
                delayed_func_reset(msec);
                msleep(2*msec); // over the time the handler was supposed to be called.

                if (g_timer_handler_counter != expect_called1) {
                        printk("Fail: %s: differenct timer handler count. expect  %u, got %u\n", 
                                test_name, expect_called1, g_timer_handler_counter);
                        return -1;
                }

                // restart function after the handler is already fired
                delayed_func_reset(msec/2);
                msleep(msec); // over the time the handler was supposed to be called.

                if (g_timer_handler_counter != expect_called2) {
                        printk("Fail: %s: differenct timer handler count. expect  %u, got %u\n", 
                                test_name, expect_called2, g_timer_handler_counter);
                        return -1;
                }
        }
        teardown();

        printk("%s: Test PASS\n", MOD_NAME);
        return 0; // OK 
}

static void __exit utest_mod_exit(void) {
        // calling target module's exit
        mod_exit();
}
