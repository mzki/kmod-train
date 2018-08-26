#ifndef _DBGLOG_H_
#define _DBGLOG_H_

// using dbglog module requires GPL license.
// you can use this macro to define required module license.
#define DBGLOG_REQUIRED_MODULE_LICENSE() MODULE_LICENSE("GPL")

typedef void dbglog_handle_t;

dbglog_handle_t * dbglog_open(const char *name);
int dbglog_printf(dbglog_handle_t *h, const char *fmt, ...);
void dbglog_close(dbglog_handle_t *h);

#endif

