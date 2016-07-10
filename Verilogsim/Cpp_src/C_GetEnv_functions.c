// Copyright (c) 2013-2016 Bluespec, Inc.  All Rights Reserved

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "C_GetEnv_functions.h"

uint64_t c_getenv_u64 (char *name)
{
    uint64_t x;

    char *value = getenv (name);
    if (value == NULL) return 0;
    if (sscanf (value, "%" SCNi64 "", & x) != 1) return 0;
    return x;
}

// ================================================================
// Testing

#if 0

int main (int argc, char *argv[])
{
    if (argc != 2) {
	fprintf (stderr, "Usage:    %s name\n", argv[0]);
	return 1;
    }
    printf ("%s = %0" PRId64 "\n", argv[1], c_getenv_u64 (argv[1]));
    return 0;
}

#endif
