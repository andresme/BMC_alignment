#ifndef ALIGNMENT_UTILS_H
#define ALIGNMENT_UTILS_H

#include "globals.h"
#include <stdlib.h>

thread_data_t *setup_thread_data(int num_threads, int imax, int jmax);
array_max_t find_array_max(int temp[], int size);
int similarity_score(char a, char b);
int initStart(int numThreads, char *v_string, char *w_string);
int initMatrix(enum GAP_TYPE v_type, enum GAP_TYPE w_type);

#endif //ALIGNMENT_UTILS_H