#include <stdio.h>
#include <config.h>

#include "obdisk.h"


void ob_version()
{
    fprintf(stderr, PACKAGE_STRING " (" VERSION_BUILD_DATE ")\n");
}


void ob_usage()
{
    fprintf(stderr,
        "USAGE: " PACKAGE " [-l|-x|-h|-v|-V] image_file [file_arg]\n\n"
        "-l\tList directory of image_file\n"
        "\tIf file_arg is omitted: list all entries\n"
        "\totherwise, list files matching regex in file_arg\n\n"
        "-x\tExtract files on image_file\n\n"
        "-v\tVerbose output\n\n"
       "-h\tShow this help information\n\n"
        "-V\tShow version information\n\n"
        "image_file is the filename of a disk image of an\n"
        "original Oberon Ceres machine.\n\n"
    );
}