#ifndef __DEPENDENT_MOD__
#define __DEPENDENT_MOD__

typedef void (*delayed_func_t)(unsigned long);

void delayed_func_after(delayed_func_t func, unsigned long msec, unsigned long data);
void delayed_func_stop(void);
void delayed_func_reset(unsigned long msec);

#endif 
