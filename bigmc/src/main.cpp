using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <set>

#include <bigmc.h>

void print_usage(char **argv) {
	fprintf(stderr,
	"Usage: %s [-hvV] [-m <max memory>] <modelfile>\n"
	"\t-h\t\tDisplay this help and exit\n"
	"\t-V\t\tPrint verbose debugging output\n"
	"\t-m x\t\tSpecify x megabytes as the maximum usable memory\n"
	"\t-v\t\tPrint version information and exit\n",
	argv[0]);
}

void print_version() {
	cout << "BigMC version " << VERSION << " (http://www.itu.dk/~gdpe/bigmc)\n";
	cout << "Copyright (c) 2011 Gian Perrone <gdpe at itu.dk>\n";
}

int main(int argc, char**argv) {
	int c;
	int verbose = 0;
	int maxmem = 0;
	
	while ((c = getopt (argc, argv, "hvVm:")) != -1)
		switch (c) {
		case 'h':
			print_usage(argv);
			return 0;
		case 'V':
			verbose = 1;
			break;
		case 'v':
			print_version();
			return 0;
		case 'm':
			maxmem = atoi(optarg);
			break;
		case '?':
			if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr, "Unknown option character `\\x%x'.\n",
				optopt);
		default:
			abort ();
           	}

	int index;
	char *modelfile = NULL;

	for (index = optind; index < argc; index++) {
		modelfile = argv[index];
	}

	if(!modelfile) {
		parser::init(NULL);
	} else {
		FILE *fp = fopen(modelfile, "r");
		if(fp == NULL) {
			perror("Cannot read model file");
			return 1;
		} else
			fclose(fp);

		parser::init(modelfile);
	}

	parser::parse();

	parser::cleanup();

	return 0;
}
