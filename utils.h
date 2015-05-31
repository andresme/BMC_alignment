//
// Created by andres on 5/30/15.
//

#ifndef ALIGNMENT_UTILS_H
#define ALIGNMENT_UTILS_H

#include "globals.h"
#include <stdlib.h>

thread_data_t *setup_thread_data(int num_threads, int imax, int jmax);
array_max_t find_array_max(int temp[], int size);
int similarity_score(char a, char b);
int init();

#endif //ALIGNMENT_UTILS_H
