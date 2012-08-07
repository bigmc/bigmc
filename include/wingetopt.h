#ifdef __GNUC__
#include <getopt.h>
#else

#ifndef _WINGETOPT_H_
#define _WINGETOPT_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int getopt(int nargc, char * const nargv[], const char *ostr);

extern char* optarg;
extern int opterr, optind, optopt;

#ifdef __cplusplus
}
#endif

#endif  /* _WINGETOPT_H */
#endif  /* __GNUC__ */

