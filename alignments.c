#include "alignments.h"

void getAlignment(enum GAP_TYPE v_type, enum GAP_TYPE w_type) {
    int max_i = seq_w_size - 1;
    int max_j = seq_v_size - 1;

    bool v_type_value = v_type == free_left_free_right || v_type == penalty_left_free_right;
    bool w_type_value = w_type == free_left_free_right || w_type == penalty_left_free_right;
    int temp = H[seq_w_size - 1][seq_v_size - 1];
    if (v_type_value && w_type_value) {
        for (int i = seq_w_size - 2; i > 0; i--) {
            for (int j = seq_v_size - 2; j > 0; j--) {
                if (H[seq_w_size - 1][j] > temp) {
                    temp = H[seq_w_size - 1][j];
                    max_i = seq_w_size - 1;
                    max_j = j;
                }
            }
            if (H[i][seq_v_size - 1] > temp) {
                temp = H[i][seq_v_size - 1];
                max_i = i;
                max_j = seq_v_size - 1;
            }
        }
    } else if (v_type_value) {
        for (int j = seq_v_size - 2; j > 0; j--) {
            if (H[seq_w_size - 1][j] > temp) {
                temp = H[seq_w_size - 1][j];
                max_j = j;
            }
        }
    } else if (w_type_value) {
        for (int i = seq_w_size - 1; i > 0; i--) {
            if (H[i][seq_v_size - 1] > temp) {
                temp = H[i][seq_v_size - 1];
                max_i = i;
            }
        }
    }
    printf("%d,%d\n", max_i, max_j);

    int i = max_i;
    int j = max_j;
    int count = 0;
    while(i > 0 || j > 0){
        if(I_direction[i][j] == TOP_LEFT){
            i = i-1;
            j = j-1;
        } else if(I_direction[i][j] == TOP){
            i = i-1;
        } else if(I_direction[i][j] == LEFT){
            j = j-1;
        } else {
            break;
        }
        count++;
    }
    string_alignment.v_string = (char *) malloc((count+1) * sizeof(char));
    string_alignment.w_string = (char *) malloc((count+1) * sizeof(char));

    i = max_i;
    j = max_j;
    int v_index = count-1;
    int w_index = count-1;

    while(i > 0 || j > 0){
        if(I_direction[i][j] == TOP_LEFT){
            string_alignment.v_string[v_index] = seq_v[j];
            string_alignment.w_string[w_index] = seq_w[i];
            i = i-1;
            j = j-1;
        } else if(I_direction[i][j] == TOP){
            string_alignment.v_string[v_index] = '-';
            string_alignment.w_string[w_index] = seq_w[i];
            i = i-1;
        } else if(I_direction[i][j] == LEFT){
            string_alignment.v_string[v_index] = seq_v[j];
            string_alignment.w_string[w_index] = '-';
            j = j-1;
        } else {
            break;
        }
        v_index--;
        w_index--;
    }
    string_alignment.v_string[count] = '\0';
    string_alignment.w_string[count] = '\0';
    printf("%s\n%s\n", string_alignment.v_string, string_alignment.w_string);
}


void runThreads(void *(*__start_routine)(void *), int threads, enum ALIGNMENT_MODE mode) {
    clock_t t1, t2;

    void *status;
    pthread_t callThd[threads];
    thread_data_t *data = setup_thread_data(threads, seq_w_size, seq_v_size, mode);

    waitingThreads = threads;
    pthread_mutex_lock(&mutexStart);
    startedThreads = 0;
    for (int i = 0; i < threads; i++) {
        pthread_create(&callThd[i], NULL, __start_routine, (void *) &data[i]);
        startedThreads++;
    }
    pthread_mutex_unlock(&mutexStart);
    t1 = clock();
    for (int i = 0; i < threads; i++) {
        pthread_join(callThd[i], &status);
    }
    t2 = clock();

    float diff = (((float) t2 - (float) t1) / 1000000.0F) * 1000;
    printf("it took: %f ms for %d threads\n", diff, threads);
}

void runNeedlemanWunsch(enum GAP_TYPE v_type, enum GAP_TYPE w_type, char *v_string, char *w_string, enum ALIGNMENT_MODE mode, int threads, int initial_k, int adjust_k) {
    initStart(threads, v_string, w_string);
    initMatrix(v_type, w_type);

    void *(*__start_routine)(void *) = p_NeedlemanWunsch;

    if(mode == k_band){
      int best_k_1 = INT_MAX;
      current_k = initial_k;
      while(best_k_1 > H[seq_w_size - 1][seq_v_size - 1]){
        init_k_band(v_type, w_type);
        runThreads(__start_routine, threads, mode);

        current_k += adjust_k;

        best_k_1 = (2*current_k + seq_w_size - seq_v_size) * score_table.gap +
          (seq_v_size - current_k) * score_table.match;

      }
    } else {
      runThreads(__start_routine, threads, mode);
    }

    // printf("%d,%d = %d\n", seq_w_size - 1, seq_v_size - 1, H[seq_w_size - 1][seq_v_size - 1]);

    pthread_mutex_destroy(&mutexWait);
    pthread_cond_destroy(&condWait);

    //printMatrix();

    getAlignment(v_type, w_type);
}

void runSmithWaterman(char *v_string, char *w_string, enum ALIGNMENT_MODE mode, int threads) {
    initStart(threads, v_string, w_string);
    initMatrix(free_left_free_right, free_left_free_right);

    void *(*__start_routine)(void *) = p_SmithWaterman;

    runThreads(__start_routine, threads, mode);

    printf("%d\n", H[seq_w_size][seq_v_size]);

    pthread_mutex_destroy(&mutexWait);
    pthread_cond_destroy(&condWait);

}

void *p_SmithWaterman(void *ptr_to_tdata) {
    thread_data_t *td = (thread_data_t *) ptr_to_tdata;
    int temp[4];
    int i, j, wave, tStart, tEnd;
    array_max_t arraymax;

    tStart = ((td->jmax / td->numThreads) * td->thread_id) + 1;
    tEnd = tStart + (td->jmax / td->numThreads);

    pthread_mutex_lock(&mutexStart);
    startedThreads--;
    while (startedThreads > 0) {
        pthread_mutex_unlock(&mutexStart);
        pthread_mutex_lock(&mutexStart);
    }
    pthread_mutex_unlock(&mutexStart);


    for (wave = 1; wave <= td->imax + td->numThreads - 1; wave++) {
        i = wave - td->thread_id;
        if (i >= 1 && i <= td->imax) {
            for (j = tStart; j < tEnd; j++) {
                temp[0] = H[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]);
                temp[1] = H[i - 1][j] - 2;
                temp[2] = H[i][j - 1] - 2;
                temp[3] = 0;
                arraymax = find_array_max(temp, 4);
                H[i][j] = arraymax.max;
                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        I_direction[i][j] = TOP_LEFT;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        I_direction[i][j] = TOP;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        I_direction[i][j] = LEFT;
                        break;
                    case 3:                                  // (i,j) is the beginning of a subsequence
                        I_direction[i][j] = NONE;
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
    int temp[3];
    int i, j, wave, tStart, tEnd;
    array_max_t arraymax;

    tStart = ((td->jmax / td->numThreads) * td->thread_id) + 1;
    tEnd = tStart + (td->jmax / td->numThreads);
    for (wave = 1; wave <= td->imax + td->numThreads - 1; wave++) {
        i = wave - td->thread_id;
        if (i >= 1 && i <= td->imax) {
            for (j = tStart; j < tEnd; j++) {
                if(!shouldFill(i, j) && td->mode == k_band){
                  continue;
                }
                temp[0] = H[i - 1][j - 1] != INT_MIN ? H[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]) : INT_MIN;
                temp[1] = H[i - 1][j] != INT_MIN ? H[i - 1][j] - 2 : INT_MIN;
                temp[2] = H[i][j - 1] != INT_MIN ? H[i][j - 1] - 2 : INT_MIN;
                arraymax = find_array_max(temp, 3);
                H[i][j] = arraymax.max;
                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        I_direction[i][j] = TOP_LEFT;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        I_direction[i][j] = TOP;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        I_direction[i][j] = LEFT;
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
