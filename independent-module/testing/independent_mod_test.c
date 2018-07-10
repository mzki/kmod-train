#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mzki");
MODULE_DESCRIPTION("unit testing for simple independent kernel module");
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
#include "../independent_mod.c"

//
// stub functions
//
enum { BUF_SIZE = 100 };
static char print_info_stub_buffer[BUF_SIZE] = {0};

static int print_info_stub(const char *fmt, ...) {
        const int fmt_len  = strlen(fmt);
        const int used_len = fmt_len < BUF_SIZE ? fmt_len : BUF_SIZE;

        strncpy(print_info_stub_buffer, fmt, used_len);
        return used_len;
}

// 
// setup/teardown for each unit test
//
static void setup(void) {
        g_print_info = print_info_stub; // set stub function to spy its content
        print_info_stub_buffer[0] = '\0';
}

static void teardown(void) {
        // do nothing
}

//
// init/exit
//
static int __init utest_mod_init(void) {
        int ret;

        setup();
        {
                const char const *test_name      = "mod_init";
                const char const *expect_msg     = "%s installed\n";
                const int         expect_msg_len = strlen(expect_msg);

                ret = mod_init();
                if (ret != 0) {
                        printk("Fail: %s: returns non-zero: %d\n", test_name, ret);
                        return -1;
                }

                if (strncmp(print_info_stub_buffer, expect_msg, expect_msg_len) != 0) {
                        printk("Fail: %s: expect msg %s, got %s\n", 
                                test_name, expect_msg, print_info_stub_buffer);
                        return -1;
                }
        }
        teardown();

        setup();
        {
                const char const *test_name      = "mod_exit";
                const char const *expect_msg     = "%s uninstalled\n";
                const int         expect_msg_len = strlen(expect_msg);

                mod_exit();
                if (strncmp(print_info_stub_buffer, expect_msg, expect_msg_len) != 0) {
                        printk("Fail: %s: expect msg %s, got %s\n", 
                                test_name, expect_msg, print_info_stub_buffer);
                        return -1;
                }
        }
        teardown();

        printk("%s: PASS\n", MOD_NAME);
        return 0; // OK 
}

static void __exit utest_mod_exit(void) {
}
