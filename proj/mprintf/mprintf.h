/* Prototipos do mprintf */

#ifndef __MPRINTF__
#define __MPRINTF__

#include "../lpc_config.h"
#include <stdarg.h>

int atoi(char *str);
double atod(char *str);
int dprint(double x, int campo, int frac, void (*putc)(int));
int mprintf(void (*putc)(int), const char *format, ... );
int sprintf(char *, const char *format, ... );
#endif
