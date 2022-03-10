#include <string.h>
#include <fnmatch.h>
#include "obdisk.h"


// ob_get_sector
// Get disk sector with index n
//
void ob_get_sector(FILE *fd, diskadr_t n, sector_t *p)
{  
    // Locate disk position
    long ofs = (n / disk_divider) * disk_sector_size;

    if (fseek(fd, ofs, SEEK_SET) != 0)
        error(1, errno, "get_sector: seek failed at # %n", n);

    // Read sector from disk
    if (fread(p, 1, disk_sector_size, fd) != disk_sector_size)
        error(1, errno, "get_sector: read failed at # %d", n);
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
//
void ob_get_fileheader(FILE *fd, diskadr_t n, fileheader_t *hdr)
{
    sector_t sec;

    ob_get_sector(fd, n, &sec);
    memcpy(hdr, &sec, sizeof(fileheader_t));

    if (hdr->mark != disk_hdrmarker)
        error(1, 0, "Expected file marker at sector %d", n);
}


// ob_traverse
// Traverse directory at sector n
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
    ob_get_sector(fd, n, (sector_t *) p);
    if (p->mark != disk_dirmarker)
        error(1, 0, "Expected dir marker at sector %d", n);

    // Recursively descend into left size of b-tree
    if (p->p0 != 0)
        ob_traverse(fd, p->p0, filearg, callproc);

    // Traverse right side of b-tree
    for (int i = 0; i < p->m; i ++)
    {
        direntry_t *de = &(p->e[i]);

        // Check if filename matches filter
        if ((filearg == NULL) || (fnmatch(filearg, de->name,
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