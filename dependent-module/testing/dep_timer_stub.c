#include "dep_timer_stub.h"

delayed_func_call_args_t g_delayed_func_call_args;

void delayed_func_call_args_init() {
        g_delayed_func_call_args.func = NULL;
        g_delayed_func_call_args.msec = 0;
        g_delayed_func_call_args.data = 0;
}

void delayed_func_after(delayed_func_t func, unsigned long msec, unsigned long data) {
        g_delayed_func_call_args.func = func;
        g_delayed_func_call_args.msec = msec;
        g_delayed_func_call_args.data = data;
}

void delayed_func_stop(void) {
        delayed_func_call_args_init();
}

void delayed_func_reset(unsigned long msec) {
        g_delayed_func_call_args.msec = msec;
}
