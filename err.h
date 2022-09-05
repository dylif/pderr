#ifndef PDERR_H
#define PDERR_H

#ifdef _WIN32
#define LACK_PROGNAME
#endif

#ifdef LACK_PROGNAME
#ifdef PDERR_CUSTOM_PROGNAME
char *__progname = PDERR_CUSTOM_PROGNAME
#else
#error LACK_PROGNAME defined but PDERR_CUSTOM_PROGNAME is not defined
#endif
#else
extern char *__progname;
#endif

#undef LACK_PROGNAME

#define progname __progname

#include <stdarg.h>

void vwarn(const char *fmt, va_list args);
void vwarnx(const char *fmt, va_list args);
void verr(int eval, const char *fmt, va_list args);
void verrx(int eval, const char *fmt, va_list args);
void warn(const char *fmt, ...);
void warnx(const char *fmt, ...);
void err(int eval, const char *fmt, ...);
void errx(int eval, const char *fmt, ...);

#ifndef PDERR_IMPL
#define PDERR_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void vwarn(const char *fmt, va_list args)
{
	fputs(progname, stderr);
	if (fmt != NULL)
	{
		fputs(": ", stderr);
		vfprintf(stderr, fmt, args);
	}
	fputs(": ", stderr);
	fputs(strerror(errno), stderr);
	putc('\n', stderr);
}

void vwarnx(const char *fmt, va_list args)
{
	fputs(progname, stderr);
	fputs(": ", stderr);
	if (fmt != NULL)
		vfprintf(stderr, fmt, args);
	putc('\n', stderr);
}

void verr(int eval, const char *fmt, va_list args)
{
	vwarn(fmt, args);
	exit(eval);
}

void verrx(int eval, const char *fmt, va_list args)
{
	vwarnx(fmt, args);
	exit(eval);
}

void warn(const char *fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	vwarn(fmt, argptr);
	va_end(argptr);
}

void warnx(const char *fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	vwarnx(fmt, argptr);
	va_end(argptr);
}

void err(int eval, const char *fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	verr(eval, fmt, argptr);
	/* No need to call va_end since we terminate */
}

void errx(int eval, const char *fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	verrx(eval, fmt, argptr);
	/* No need to call va_end since we terminate */
}

#undef progname

#endif /* PDERR_IMPL */

#endif /* PDERR_H */
