#include "alignments.h"

void *p_SmithWaterman(void *ptr_to_tdata) {
    thread_data_t *td = (thread_data_t *) ptr_to_tdata;
    int temp[4];
    int i, j, wave, tStart, tEnd;
    array_max_t arraymax;

    tStart = ((td->jmax / td->numThreads) * td->thread_id) + 1;
    tEnd = tStart + (td->jmax / td->numThreads);
    for (wave = 1; wave <= td->imax + td->numThreads - 1; wave++) {
        i = wave - td->thread_id;
        if (i >= 1 && i <= td->imax) {
            for (j = tStart; j <= tEnd; j++) {
                temp[0] = H[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]);
                temp[1] = H[i - 1][j] - 2;
                temp[2] = H[i][j - 1] - 2;
                temp[3] = 0;
                arraymax = find_array_max(temp, 4);
                H[i][j] = arraymax.max;
                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        I_i[i][j] = i - 1;
                        I_j[i][j] = j - 1;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        I_i[i][j] = i - 1;
                        I_j[i][j] = j;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        I_i[i][j] = i;
                        I_j[i][j] = j - 1;
                        break;
                    case 3:                                  // (i,j) is the beginning of a subsequence
                        I_i[i][j] = i;
                        I_j[i][j] = j;
                        break;
                }
            }
            pthread_mutex_lock(&mutexWait);
            waitingThreads--;
            if (waitingThreads > 0) {
                pthread_cond_wait(&condWait, &mutexWait);
            } else {
                waitingThreads = td->numThreads;
                pthread_cond_broadcast(&condWait);
            }
            pthread_mutex_unlock(&mutexWait);
        }
    }
    return ((void *) 0);
}

void *p_NeedlemanWunsch(void *ptr_to_tdata) {
    thread_data_t *td = (thread_data_t *) ptr_to_tdata;
    int temp[4];
    int i, j, wave, tStart, tEnd;
    array_max_t arraymax;

    tStart = ((td->jmax / td->numThreads) * td->thread_id) + 1;
    tEnd = tStart + (td->jmax / td->numThreads);
    for (wave = 1; wave <= td->imax + td->numThreads - 1; wave++) {
        i = wave - td->thread_id;
        if (i >= 1 && i <= td->imax) {
            for (j = tStart; j <= tEnd; j++) {
                temp[0] = H[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]);
                temp[1] = H[i - 1][j] - 2;
                temp[2] = H[i][j - 1] - 2;
                arraymax = find_array_max(temp, 3);
                H[i][j] = arraymax.max;
                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        I_i[i][j] = i - 1;
                        I_j[i][j] = j - 1;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        I_i[i][j] = i - 1;
                        I_j[i][j] = j;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        I_i[i][j] = i;
                        I_j[i][j] = j - 1;
                        break;
                    case 3:                                  // (i,j) is the beginning of a subsequence
                        I_i[i][j] = i;
                        I_j[i][j] = j;
                        break;
                }
            }
            pthread_mutex_lock(&mutexWait);
            waitingThreads--;
            if (waitingThreads > 0) {
                pthread_cond_wait(&condWait, &mutexWait);
            } else {
                waitingThreads = td->numThreads;
                pthread_cond_broadcast(&condWait);
            }
            pthread_mutex_unlock(&mutexWait);
        }
    }
    return ((void *) 0);
}