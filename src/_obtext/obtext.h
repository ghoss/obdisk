#ifndef _OBTEXT_H
#define _OBTEXT_H   1

#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


// Function declarations
void ob_version();
void ob_usage();
void ob_convert(char *f, bool verbose);

#endif