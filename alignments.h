//
// Created by andres on 5/30/15.
//

#ifndef ALIGNMENT_ALIGNMENTS_H
#define ALIGNMENT_ALIGNMENTS_H

#include "globals.h"
#include "utils.h"

void *p_SmithWaterman(void *ptr_to_tdata);
void *p_NeedlemanWunsch(void *ptr_to_tdata);
void *p_KBand(void *ptr_to_tdata);

#endif //ALIGNMENT_ALIGNMENTS_H
