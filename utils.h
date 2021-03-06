#ifndef ALIGNMENT_UTILS_H
#define ALIGNMENT_UTILS_H

#include "globals.h"
#include <stdlib.h>
#include <math.h>

thread_data_t *setup_thread_data(int num_threads, int imax, int jmax, enum ALIGNMENT_MODE mode);

array_max_t find_array_max(int temp[], int size);

int similarity_score(char a, char b);

int initStart(char *v_string, char *w_string);

int initMatrix(enum GAP_TYPE v_type, enum GAP_TYPE w_type);

bool shouldFill(int i, int j);

void clear(enum GAP_TYPE v_type, enum GAP_TYPE w_type);

void init_k_band(enum GAP_TYPE v_type, enum GAP_TYPE w_type);

void printMatrix(int **matrix);

void printMatrixToFile(char *fileName, int **matrix);

int initMatricesForBlocks(enum GAP_TYPE v_type, enum GAP_TYPE w_type);

void getAlignment(enum GAP_TYPE v_type, enum GAP_TYPE w_type);

void freeStrings();

void freeResults();

void freeMatrix(int **matrix, int size_i);

void freeThreadData(thread_data_t *data);

void writeAlignmentToFile();

void writeTimesToFile(float times[], int threads, char *fileName);

void plotWithGnu();

void plotWithGnuPath();

void getAlignmentBlock(enum GAP_TYPE v_type, enum GAP_TYPE w_type);

void plotTimesKBand();

void plotTimes();

void plotWithGnuB();

void plotWithGnuPathB();

void plotWithGnuC();

void plotWithGnuPathC();

void plotAlignment();

#endif //ALIGNMENT_UTILS_H
