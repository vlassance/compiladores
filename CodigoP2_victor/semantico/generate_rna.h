#ifndef GENERATE_RNA_H
# define GENERATE_RNA_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include "../syn/automata.h"

#define POS_INICIAL 2

void generate_rna_file(int meta, int n_fatos, int fatos[]);

#endif