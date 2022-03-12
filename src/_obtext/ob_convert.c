//=====================================================
// obtext
// Oberon Ceres Text File Conversion
//
// Guido Hoss, 12.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include "obtext.h"


void ob_convert(char *f, bool verbose)
{
    FILE *fd;

    error(1, 0, "obtext is not implemented yet!");

    // Open input file
    if (! (fd = fopen(f, "r")))
    {
        error(0, errno, "Can't open input file '%s'", f);
        return;
    }

    // Close input file
    fclose(fd);
}