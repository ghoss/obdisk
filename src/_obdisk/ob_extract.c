//=====================================================
// obdisk
// Oberon Ceres Disk Utility
//
// Guido Hoss, 12.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#include <string.h>
#include "obdisk.h"

// ob_extract
// Extract files to specified directory (or current directory
// if outdir == NULL). filearg contains an optional wildcard filter.
//
void ob_extract(
    FILE *fd, char *filearg, char *outdir, bool brute, bool verbose
)
{
    int count = 0;

    // Extract a sector with specified number of bytes
    // Closes output file and returns FALSE if the sector could not be read
    // Aborts if the sector could not be written.
    //
    bool extract_sector(FILE *ofd, diskadr_t sec, uint32_t *n, uint32_t start)
    {
        uint32_t num_b;
        sector_t data;
        
        num_b = (*n > disk_sector_size - start) ? 
            (disk_sector_size - start) : *n;

        if (num_b > 0)
        {
            if (! ob_get_sector(fd, sec, &data))
            {
                if (verbose)
                    error(0, 0, 
                        "Can't read sector %d. Output file truncated.", sec
                    );
                
                fclose(ofd);
                return false;
            }

            if (fwrite(&(data.b[start]), num_b, 1, ofd) != 1)
                error(1, errno, "Write error");
        }

        *n -= num_b;
        return true;
    }

    // Callback to extract a file specified in *d
    //
    void extract_file(direntry_t *d)
    {
        fileheader_t hdr;
        FILE *ofd;
        char fn[dir_filename_len + 1];
        uint32_t sz;
        int mx;

        // Ensure that filename is null terminated
        fn[dir_filename_len] = '\0';
        strncpy(fn, d->name, dir_filename_len);

        // Get file header from sector stored in directory
        if (! ob_get_fileheader(fd, d->adr, &hdr))
        {
            // Not a valid file?
            if (verbose)
                error(0, 0, "%s: Invalid file header!", fn);
            return;
        }

        // Get file size from header
        sz = ob_get_filesize(&hdr);
        count ++;
        if (verbose)
            printf("%s ... %d bytes\n", fn, sz);

        // Create output file
        if (! (ofd = fopen(fn, "w")))
            error(1, errno, "Can't create output file");

        // Extract filler data from file header
        if (! extract_sector(ofd, d->adr, &sz, sizeof(fileheader_t)))
            return;

        // Extract remaining sectors in primary sector table
        // The first entry  is identical to the fileheader sector
        // already extracted above
        mx = (hdr.alen < file_sectab_size) ? 
                hdr.alen : (file_sectab_size - 1);

        for (int i = 1; i <= mx; i ++)
        {
            if (! extract_sector(ofd, hdr.sectab[i], &sz, 0))
                return;
        }

        // Extract sectors of extension table
        if (hdr.alen >= file_sectab_size)
        {
            // Number of external index sectors
            mx = hdr.alen - file_sectab_size + 1;
            int ext_n = mx / disk_index_size;

            // Get index sectors from extension table
            for (int i = 0; i <= ext_n; i ++)
            {
                sector_t idx;
                if (! ob_get_sector(fd, hdr.extab[i], &idx))
                {
                    if (verbose)
                        error(0, 0, 
                            "Can't read sector %d. Output file truncated.", 
                            hdr.extab[i]
                        );
                    break;
                }

                // Get all data sectors from index sector
                int max_j = (mx > disk_index_size ? disk_index_size : mx);
                mx -= max_j;
                for (int j = 0; j < max_j; j ++)
                {
                    if (! extract_sector(ofd, idx.adr[j], &sz, 0))
                        return;
                }
            }
        }

        // Close output file
        fclose(ofd);
    }

    if (brute)
    {
        // Scan disk for sectors with directory headers
        ob_traverse_brute(fd, filearg, extract_file);
    }
    else
    {
        // Traverse the directory tree starting at its root
        ob_traverse(fd, dir_root_sector, filearg, extract_file);
    }

    if (verbose)
    {
        printf("> %d file%s extracted.\n", count, (count != 1) ? "s" : "");
        if (brute)
            printf("> Some files may have been duplicates.\n");
    }
}