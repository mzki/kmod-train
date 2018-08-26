#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#include "dbglog.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mzki");
MODULE_DESCRIPTION("module1 using dbglog");
MODULE_VERSION("0.0.1");

static const char * const MOD_NAME = "module2";
static const char * const LOG_MOD_NAME = "module2-log";

static dbglog_handle_t *dbglog_handle;

static int __init mod_init(void) {
        int ret;

        dbglog_handle = dbglog_open(LOG_MOD_NAME);
        if (dbglog_handle == NULL) {
                pr_err("%s: failed to create dbglog file\n", MOD_NAME);
                return -1;
        }

        ret = dbglog_printf(dbglog_handle, "%s: dbglog printf!!\n", MOD_NAME);
        if (ret < 0) {
                dbglog_close(dbglog_handle);
                pr_err("%s: failed to print dbglog file\n", MOD_NAME);
                return -1;
        }

        pr_info("%s: installed\n", MOD_NAME);
        return 0;
}

static void __exit mod_exit(void) {
        dbglog_close(dbglog_handle);
        pr_info("%s: uninstalled\n", MOD_NAME);
}

module_init(mod_init);
module_exit(mod_exit);
