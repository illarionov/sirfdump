#ifndef _GPIO_H
#define _GPIO_H

extern void pxa_gpio_mode_set(int gpio_mode);
extern int pxa_gpio_mode_get(int gpio);

extern int pxa_gpio_read(int gpio);
extern int pxa_gpio_set(int gpio, int val);

extern char *pxa_gpio_mode2txt(int mode);

#endif
