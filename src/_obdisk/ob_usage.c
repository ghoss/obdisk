#include <stdio.h>
#include <config.h>

#include "obdisk.h"


void ob_version()
{
    fprintf(stderr, PKG " v" PACKAGE_VERSION " (" VERSION_BUILD_DATE ")\n");
}


void ob_usage()
{
    fprintf(stderr,
        "USAGE: " PKG " [-lxhfvV] [-d dest_dir] image_file [file_arg]\n\n"
        "-l\tList directory of image_file\n"
        "\tIf file_arg is omitted: list all entries\n"
        "\totherwise, list files matching regex in file_arg\n\n"
        "-x\tExtract files on image_file\n\n"
        "-d\tExtract into destination dir 'dest_dir' (must already exist)\n\n"
        "-f\tBrute force mode (ignores disk structure; use with caution)\n\n"
        "-v\tVerbose output\n\n"
       "-h\tShow this help information\n\n"
        "-V\tShow version information\n\n"
        "image_file is the filename of a disk image of an\n"
        "original Oberon Ceres machine.\n\n"
    );
}