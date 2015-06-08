#ifndef ALIGNMENT_STRUCTS_H
#define ALIGNMENT_STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#include <limits.h>

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
    int new_block_cost;
    int continue_block_cost;
    int **table;
} typedef table_score;

typedef struct alignment {
    char* v_string;
    char* w_string;
} ALIGNMENT;


enum DIRECTION {
    TOP,
    LEFT,
    TOP_LEFT,
    NONE,
    TOP_H,
    TOP_B,
    TOP_C,
    LEFT_H,
    LEFT_B,
    LEFT_C,
    TOP_LEFT_H,
    TOP_LEFT_B,
    TOP_LEFT_C
} direction;

enum GAP_TYPE {
    free_left_free_right,
    free_left_penalty_right,
    penalty_left_free_right,
    penalty_left_penalty_right,
} gap_type;

enum ALIGNMENT_MODE {
    k_band,
    gap_blocks,
    none,
} alignment_mode;

struct thread_data_t {
    int thread_id, numThreads, imax, jmax;
    enum ALIGNMENT_MODE mode;
} typedef thread_data_t;

struct array_max_t {
    int ind;
    int max;
} typedef array_max_t;

pthread_mutex_t mutexStart;
pthread_mutex_t mutexWait;
pthread_cond_t condWait;
int waitingThreads;
int startedThreads;

int **I_direction;
int **H;
int **B;
int **C;

int threads;
int seq_v_size;
int seq_w_size;

char *seq_v;
char *seq_w;

table_score score_table;
ALIGNMENT string_alignment;

int current_k;

#endif //ALIGNMENT_STRUCTS_H
