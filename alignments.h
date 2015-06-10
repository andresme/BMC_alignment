#ifndef ALIGNMENT_ALIGNMENTS_H
#define ALIGNMENT_ALIGNMENTS_H

#include "globals.h"
#include "utils.h"

void runNeedlemanWunsch(enum GAP_TYPE v_type, enum GAP_TYPE w_type, char *v_string, char *w_string, enum ALIGNMENT_MODE mode, int threads, int inital_k, int adjust_k);
void runSmithWaterman(char *v_string, char *w_string, enum ALIGNMENT_MODE mode, int threads);

void *p_SmithWaterman(void *ptr_to_tdata);
void *p_NeedlemanWunsch(void *ptr_to_tdata);
void *p_NeedlemanWunschBlock(void *ptr_to_tdata);
void *p_SmithWatermanBlocks(void *ptr_to_tdata);

#endif //ALIGNMENT_ALIGNMENTS_H
