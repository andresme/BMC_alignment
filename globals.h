//
// Created by andres on 5/30/15.
//

#ifndef ALIGNMENT_STRUCTS_H
#define ALIGNMENT_STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/*
 * type:
 *  0 = custom
 *  1 = file (table)
 */
struct table_score {
    int type;
    int match;
    int missmatch;
    int gap;
    int **table;
} typedef table_score;

/*
 * gaps_free:
 *  0 = no free gaps
 *  1 = free gaps left
 *  2 = free gaps right
 *  3 = free gaps both
 * gap_start_penalty:
 *  0 = no start penalty
 *  1 = start penalty
 */
struct thread_data_t {
    int thread_id, numThreads, imax, jmax, gaps_free, gap_start_penalty;
} typedef thread_data_t;

struct array_max_t {
    int ind;
    int max;
} typedef array_max_t;

pthread_mutex_t mutexWait;
pthread_cond_t condWait;
int waitingThreads;

int **I_i;
int **I_j;
int **H;

int threads;
int seq_v_size;
int seq_w_size;

char *seq_v;
char *seq_w;

table_score score_table;

#endif //ALIGNMENT_STRUCTS_H
