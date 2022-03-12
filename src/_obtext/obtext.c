#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <config.h>
#include "obtext.h"


void main(int argc, char **argv)
{
	char c;
	char *infile = NULL;
	FILE *infile_fd = NULL;
	char *outdir = NULL;
	bool verbose = false;

	// Parse command line options
	opterr = 0;
	while ((c = getopt (argc, argv, "hVvd:")) != -1)
	{
		switch (c)
		{
		case 'd' :
			outdir = optarg;
			break;

		case 'v' :
			verbose = true;
			break;

		case 'h' :
			ob_usage();
			exit(0);

		case 'V' :
			ob_version();
			exit(0);

		case '?' :
			error(1, 0,
				"Unrecognized option (run \"" PACKAGE " -h\" for help)."
			);
			break;

		default :
			break;
		}
	}

	if (optind == argc)
		error(1, 0, "No input file(s) specified.");

	if (verbose)
		ob_version();

	// Loop through input files
	while (optind < argc)
	{
		char *infile = argv[optind ++];
		
		if (verbose)
			printf("> Input file name: %s\n", infile);
		
		// Convert input file
		ob_convert(infile, verbose);
	}
}