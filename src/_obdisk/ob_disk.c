//=====================================================
// obdisk
// Oberon Ceres Disk Utility
//
// Guido Hoss, 12.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include <string.h>
#include <fnmatch.h>
#include "obdisk.h"

// ob_get_sector
// Get disk sector with index n
//
bool ob_get_sector(FILE *fd, diskadr_t n, sector_t *p)
{  
    // Locate disk position
    long ofs = (n / disk_divider) * disk_sector_size;
    if (fseek(fd, ofs, SEEK_SET) != 0) return false;

    // Read sector from disk
    return (fread(p, 1, disk_sector_size, fd) == disk_sector_size);
}


// ob_get_filesize
// Calculate size of file with header hdr
//
uint32_t ob_get_filesize(fileheader_t *hdr)
{
    int sz = (uint32_t)(hdr->alen) * disk_sector_size 
        + hdr->blen - sizeof(fileheader_t);

    return (sz > 0) ? (uint32_t)sz : 0;
}


// ob_get_fileheader
// Get a file header from disk sector n
// Returns TRUE if a valid file marker was found
//
bool ob_get_fileheader(FILE *fd, diskadr_t n, fileheader_t *hdr)
{
    sector_t sec;

    if (! ob_get_sector(fd, n, &sec)) return false;
    memcpy(hdr, &sec, sizeof(fileheader_t));

    return (hdr->mark == disk_hdrmarker);
}


// ob_traverse
// Traverse directory at sector n with recursion
// (Normal mode; assumes a "normal" filesystem)
//
void ob_traverse(
	FILE *fd, diskadr_t n, char *filearg,
	void (*callproc)(direntry_t *))
{
    // Allocate memory for sector
    dirpage_t *p = malloc(sizeof(dirpage_t));

    if (! p)
        error(1, errno, "ob_traverse: can't allocate memory");

    // Get current page
    if ((! ob_get_sector(fd, n, (sector_t *) p))
        || (p->mark != disk_dirmarker))
    {
        // No fatal error in order to allow listing of other files
        error(0, 0, "Directory sector %d invalid", n);
        return;
    }

    // Recursively descend into left size of b-tree
    if (p->p0 != 0)
        ob_traverse(fd, p->p0, filearg, callproc);

    // Traverse right side of b-tree
    for (int i = 0; i < p->m; i ++)
    {
        direntry_t *de = &(p->e[i]);
        char fn[dir_filename_len + 1];

        // Filename might not be null-terminated!
        strncpy(fn, de->name, dir_filename_len);
        fn[dir_filename_len] = '\0';

        // Check if filename matches filter
        if ((filearg == NULL) || (fnmatch(filearg, fn,
            FNM_PATHNAME) == 0))
        {
            callproc(de);
        }

        // Recursively descend into right side of b-tree
        if (de->p != 0)
            ob_traverse(fd, de->p, filearg, callproc);
    }

    free(p);
}


// ob_traverse_brute
// Alternative to ob_traverse which scans the entire disk
// for directory pages based on their dir_marker.
//
void ob_traverse_brute(
	FILE *fd, char *filearg, void (*callproc)(direntry_t *)
) {
    dirpage_t p;    
    uint32_t count = 0;

    while (true)
    {
        // Run until no more sectors readable
        if (! ob_get_sector(fd, count, (sector_t *) &p)) break;
        count ++;

        // Only handle sectors with valid dir_marker
        if (p.mark == disk_dirmarker) 
        {
            // Traverse right side of b-tree
            for (int i = 0; i < p.m; i ++)
            {
                direntry_t *de = &(p.e[i]);

                // Check if filename matches filter
                if ((filearg == NULL) || (fnmatch(filearg, de->name,
                    FNM_PATHNAME) == 0))
                {
                    callproc(de);
                }
            }
        }
    }
}