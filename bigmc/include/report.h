#ifndef _REPORT_H
#define _REPORT_H

#define DEBUG	global_cfg.debug
#define REPORT(x) (x >= global_cfg.verbose_level)

#define REPORT_FATAL 1
#define REPORT_WARN  2
#define REPORT_INFO  3

std::ostream &report(int level, char *file, char *lineno, string module);
std::ostream &ferror(string module);
std::ostream &rwarning(string module);
std::ostream &rinfo(string module);

#endif
