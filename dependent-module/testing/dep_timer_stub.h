#ifndef __DEP_TIMER_STUB__
#define __DEP_TIMER_STUB__


#include <linux/stddef.h>

#include "dep_timer.h"

typedef struct {
        delayed_func_t func;
        unsigned long  msec;
        unsigned long  data;
} delayed_func_call_args_t;

extern delayed_func_call_args_t g_delayed_func_call_args;

void delayed_func_call_args_init(void);

#endif
