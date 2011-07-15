using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <set>

#include <bigmc.h>

void print_usage(char **argv) {
	fprintf(stderr,
	"Usage: %s [-hvV] <modelfile>\n"
	"\t-h\t\tDisplay this help and exit\n"
	"\t-V\t\tPrint verbose debugging output\n"
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
	
	while ((c = getopt (argc, argv, "hvV")) != -1)
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
		printf ("Model file: %s\n", argv[index]);
		modelfile = argv[index];
	}

	if(!modelfile) {
		fprintf(stderr, "Error: Missing model file\n");
		print_usage(argv);
		return 1;
	}
	

	return 0;
}
