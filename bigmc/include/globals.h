#ifndef _GLOBALS_H
#define _GLOBALS_H

struct global_config {
	bool debug;
	int verbose_level;
	unsigned long max_steps;
	unsigned long report_interval;
	char *graph_out;
	bool check_local;
	char *model_file;
	char *bigmc_home;
};

extern global_config global_cfg; 

#endif
