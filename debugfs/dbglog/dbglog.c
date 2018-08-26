// This file provides debug log feature by using 
// debugfs internally.

#include <linux/debugfs.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/kernel.h>

#include <stdarg.h>

#include "dbglog.h"

#define BUFSIZE (256)

typedef struct {
        struct dentry *dentry;
        char buf[BUFSIZE];
} dbglog_private_t;

static ssize_t 
dbglog_file_read(struct file *f, char __user *buf, size_t len, loff_t *ppos) {
        dbglog_private_t *priv = (dbglog_private_t *)file_inode(f)->i_private;

        return simple_read_from_buffer(buf, len, ppos, priv->buf, BUFSIZE);
}

// define file operations for debug log file.
static struct file_operations g_dbglog_fops = {
        .read = dbglog_file_read,
};

dbglog_handle_t *dbglog_open(const char *name) {
        struct dentry *dentry;
        dbglog_private_t *priv;

        priv = (dbglog_private_t *)kzalloc(sizeof(*priv), GFP_KERNEL);
        if (IS_ERR(priv)) {
                printk("dbglog: can not create private data\n");
                return NULL;
        }

        dentry = debugfs_create_file(name, 0644, NULL, priv, &g_dbglog_fops);
        if (dentry == NULL || dentry == ERR_PTR(-ENODEV)) {
                kfree(priv);
                printk("dbglog: can not create debug log\n");
                return NULL;
        }

        priv->dentry = dentry;

        // cast to prevent exporting private data structure.
        return (dbglog_handle_t *)priv;
}

void dbglog_close(dbglog_handle_t *h) {
        dbglog_private_t *priv;

        if (h == NULL) {
                printk("dbglog: NULL handler\n");
                return;
        }

        priv = (dbglog_private_t *)h;
        
        debugfs_remove(priv->dentry);
        kfree(priv);

        h = NULL;
}

int dbglog_printf(dbglog_handle_t *h, const char *fmt, ...) {
        int ret;
        dbglog_private_t *priv;
        va_list va_args;

        if (h == NULL) {
                printk("dbglog: NULL handler\n");
                return -1;
        }

        priv = (dbglog_private_t *)h;

        va_start(va_args, fmt);

        ret = vsnprintf(priv->buf, BUFSIZE, fmt, va_args);

        va_end(va_args);

        return ret;
}
        
