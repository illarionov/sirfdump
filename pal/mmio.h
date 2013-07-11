
#ifndef _MMIO_H
#define _MMIO_H

#include <sys/types.h>

extern u_int32_t mmio_rd(unsigned long phys_addr);
extern int mmio_wr(unsigned long phys_addr, u_int32_t value);

unsigned long mmio_getmembyname(char *name);

extern int mmio_init(void);
extern void mmio_fini(void);

#endif
