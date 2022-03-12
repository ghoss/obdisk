#include "obdisk.h"


// ob_listdir
// List filesystem directory with optional wildcard filter
//
void ob_listdir(FILE *fd, char *filearg, bool brute, bool verbose)
{
    int count = 0;
    int totalsize = 0;
    int invalid = 0;

    // Callback to print a directory entry
    //
    void print_dir(direntry_t *d)
    {
        fileheader_t hdr;
        uint32_t sz;
        uint32_t dd, mm, yy, dt;
        uint32_t hh, mn, ss, tm;

        // Get file header from sector stored in directory
        if (ob_get_fileheader(fd, d->adr, &hdr))
        {
            // Calculate file size
            sz = ob_get_filesize(&hdr);
            count ++;
            totalsize += sz;

            // Calculate file date
            dt = hdr.dt;
            dd = dt % 32; dt >>= 5;
            mm = dt % 16; dt >>= 4;
            yy = dt;

            // Calculate file time
            tm = hdr.tm;
            ss = tm % 64; tm >>= 6;
            mn = tm % 64; tm >>= 6;
            hh = tm;

            printf("%-32s %8d  %02d.%02d.%02d  %02d:%02d:%02d\n", 
                d->name, sz, dd, mm, yy, hh, mn, ss);
        }
        else
        {
            // Does not look like a valid file
            invalid ++;
            if (verbose)
                printf("%-32s ** INVALID FILE HEADER **\n", d->name);
        }
    }

    if (brute)
    {
        // Scan disk for sectors with directory headers
        ob_traverse_brute(fd, filearg, print_dir);
    }
    else
    {
        // Traverse the directory tree starting at its root
        ob_traverse(fd, dir_root_sector, filearg, print_dir);
    }

    if (verbose)
    {
        if (invalid == 0)
        {
            printf("> %d byte%s total in %d file%s.\n", 
                totalsize,
                (totalsize != 1) ? "s" : "", 
                count,
                (count != 1) ? "s" : ""
            );
        }
        else
        {
            printf(
                "> %d (valid?) file%s and "
                "%d invalid file header%s found.\n",
                count,
                (count != 1) ? "s" : "",
                invalid, (invalid != 1) ? "s" : ""
            );
        }
    }
}