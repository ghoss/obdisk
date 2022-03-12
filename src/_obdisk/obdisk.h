//=====================================================
// obdisk
// Oberon Ceres Disk Utility
//
// Guido Hoss, 12.03.2022
//
// Published by Guido Hoss under GNU Public License V3.
//=====================================================

#ifndef _OBDISK_H
#define _OBDISK_H   1

#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// OBERON CERES LOGICAL FILESYSTEM DEFINITIONS
// (Derived from official Oberon V4 FileDir.MOD)

// Disk sector address
typedef uint32_t diskadr_t;

// Disk structure constants
#define disk_sector_size    1024    // Sector size in bytes
#define disk_index_size		(disk_sector_size / sizeof(diskadr_t))
#define disk_divider		29      // Division factor for sector numbers
#define dir_page_size       24      // Number of dir entries per page
#define dir_filename_len    32      // Max. filename length in chars
#define dir_filler_len      52      // Size of filler area in dir page
#define dir_root_sector		29		// Sector of first directory page
#define file_extab_size		12		// File extension table size
#define file_sectab_size	64		// File sector table size
#define disk_dirmarker		0x9B1EA38D
#define disk_hdrmarker		0x9BA71D86

// Filename
typedef char filename_t[dir_filename_len];

// Structure of a file header
typedef struct {
	uint32_t mark;
	filename_t name;
	uint16_t alen;
	uint16_t blen;
	uint32_t dt;
	uint32_t tm;
	diskadr_t extab[file_extab_size];
	diskadr_t sectab[file_sectab_size];
} fileheader_t;

// Structure of a directory entry
typedef struct {
	filename_t name;
	diskadr_t adr;
	diskadr_t p;
} direntry_t;

// Structure of a directory page
typedef struct {
	uint32_t mark;
	uint32_t m;
	diskadr_t p0;
	uint8_t fill[dir_filler_len];
	direntry_t e[dir_page_size];
} dirpage_t;

// Structure of an disk sector
typedef struct {
	union {
		diskadr_t adr[disk_index_size];		// Directory sector
		fileheader_t hdr;					// File header
		uint8_t b[disk_sector_size];		// Data sector
	};
} sector_t;


// Function declarations
void ob_version();
void ob_usage();
void ob_listdir(FILE *fd, char *filearg, bool brute, bool verbose);
void ob_extract(
	FILE *fd, char *filearg, char *outdir,
	bool brute, bool verbose
);
bool ob_get_fileheader(FILE *fd, diskadr_t n, fileheader_t *hdr);
uint32_t ob_get_filesize(fileheader_t *hdr);
bool ob_get_sector(FILE *fd, diskadr_t n, sector_t *p);
void ob_traverse(
	FILE *fd, diskadr_t n, char *filearg,
	void (*callproc)(direntry_t *)
);
void ob_traverse_brute(
	FILE *fd, char *filearg, void (*callproc)(direntry_t *)
);

#endif