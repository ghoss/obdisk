#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <config.h>
#include "obdisk.h"


// Implemented operation modes
typedef enum {
	M_LISTDIR,
	M_EXTRACT,
	M_UNKNOWN
} mode_type;


void main(int argc, char **argv)
{
	char c;
	char *imgfile = NULL;
	FILE *imgfile_fd = NULL;
	char *outdir = NULL;
	char *filearg = NULL;
	mode_type mode = M_UNKNOWN;
	bool verbose = false;
	bool brute = false;

	// Parse command line options
	opterr = 0;
	while ((c = getopt (argc, argv, "Vvlxhfd:")) != -1)
	{
		switch (c)
		{
		case 'l' :
			mode = M_LISTDIR;
			break;

		case 'x' :
			mode = M_EXTRACT;
			break;

		case 'd' :
			outdir = optarg;
			break;

		case 'v' :
			verbose = true;
			break;

		case 'f' :
			brute = true;
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

	// Check for image_file
	if (optind < argc)
	{
		// Get image file name
		imgfile = argv[optind];
		if (! (imgfile_fd = fopen(imgfile, "r")))
		{
			error(1, errno, "Can't open image file '%s'", imgfile);
		}
		if (verbose)
		{
			ob_version();
			printf("> Image file name: %s\n", imgfile);
		}
	}
	else
	{
		error(1, 0, "No image file specified.");
	}

	// Check for optional file_arg argument
	if (optind + 1 < argc)
	{
		filearg = argv[optind + 1];
	}
	if (verbose)
	{
		printf("> File argument: '%s'\n", filearg ? filearg : "*");
		if (brute)
			printf("> Brute force mode enabled\n");
	}

	// Execute requested program function
	switch (mode)
	{
	case M_LISTDIR :
		ob_listdir(imgfile_fd, filearg, brute, verbose);
		break;

	case M_EXTRACT :
		// Check if output directory exists and change to it
		//
		if ((outdir != NULL) && (*outdir != '\0'))
		{
			if (chdir(outdir) != 0)
				error(1, errno, "Invalid output directory '%s'", outdir);
		}
		if (verbose)
			printf("> Destination dir: '%s'\n", outdir ? outdir : ".");

		ob_extract(imgfile_fd, filearg, outdir, brute, verbose);
		break;

	default :
		error(0, 0, 
			"No function requested (run \"" PACKAGE " -h\" for help)."
		);
		break;
	}

	// Close image file
	fclose(imgfile_fd);
}