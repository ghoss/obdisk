//=====================================================
// obtext
// Oberon Ceres Text File Conversion
//
// Guido Hoss, 12.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include <stdio.h>
#include <config.h>
#include "obtext.h"


void ob_version()
{
    fprintf(stderr, PKG " v" PACKAGE_VERSION " (" VERSION_BUILD_DATE ")\n");
}


void ob_usage()
{
    fprintf(stderr,
        "USAGE: " PKG " [-hvV] input_file [input_file...]\n\n"
        "-v\tVerbose output\n\n"
       "-h\tShow this help information\n\n"
        "-V\tShow version information\n\n"
        "input_file is the filename of a Ceres text file.\n\n"
    );
}