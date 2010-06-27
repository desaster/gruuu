#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void debug_vba(const char *format, ...)
{
	va_list args;
	char foo[255];
	va_start(args, format);

	vsprintf(foo, format, args);
	strcat(foo, "\r\n");

	asm volatile(
			"mov r0, %0;"
			"swi 0xff;"
			: // no ouput
			: "r" (foo)
			: "r0");

	va_end(args);
}

void debug_gba(const char *format, ...) {
}
