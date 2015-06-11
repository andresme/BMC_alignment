#include "alignments.h"


float runThreads(void *(*__start_routine)(void *), int threads, enum ALIGNMENT_MODE mode) {
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
    // printf("it took: %f ms for %d threads\n", diff, threads);
    freeThreadData(data);
    return diff;
}

void runNeedlemanWunsch(enum GAP_TYPE v_type, enum GAP_TYPE w_type, char *v_string, char *w_string, enum ALIGNMENT_MODE mode, int threads, int initial_k, int adjust_k) {
  float times[threads];
  float times_k_band[threads];

  for(int i = 0; i < threads; i++){
    initStart(v_string, w_string);
    if (mode == gap_blocks){
        initMatricesForBlocks(v_type, w_type);
    } else {
        initMatrix(v_type, w_type);
    }

    void *(*__start_routine)(void *) = mode == gap_blocks ? p_NeedlemanWunschBlock : p_NeedlemanWunsch;

    times[i] = runThreads(__start_routine, i+1, mode);

    if(mode == k_band){
      clear(v_type, w_type);
      times_k_band[i] = 0;
      int best_k_1 = INT_MAX;
      current_k = initial_k;
      while(best_k_1 > H[seq_w_size][seq_v_size]){
        init_k_band(v_type, w_type);
        times_k_band[i] += runThreads(__start_routine, i+1, mode);

        current_k += adjust_k;

        best_k_1 = (2*current_k + seq_w_size - seq_v_size) * score_table.gap +
          (seq_v_size - current_k) * score_table.match;

          printf("%d > %d\n", best_k_1, H[seq_w_size][seq_v_size]);
      }
    }

    pthread_mutex_destroy(&mutexWait);
    pthread_cond_destroy(&condWait);

    if (mode == gap_blocks){
        getAlignmentBlock(v_type, w_type);
    } else {
        getAlignment(v_type, w_type);
    }
    freeMatrix(H, seq_w_size);
    if(mode == gap_blocks){
      freeMatrix(B, seq_w_size);
      freeMatrix(C, seq_w_size);
    }
    freeMatrix(I_direction, seq_w_size);
    freeStrings();
    freeResults();
  }

    for(int i = 0; i < threads; i++){
      printf("%d threads took: %f\n", i, times[i]);
      if(mode == k_band){
        printf("%d threads took: %f (k bands)\n", i, times_k_band[i]);
      }
    }
    plotWithGnuPath();

    writeTimesToFile(times, threads, "temp_times.dat");
    if(mode == k_band){
      writeTimesToFile(times_k_band, threads, "temp_times_k_band.dat");
      plotTimesKBand();
    } else {
      plotTimes();
    }
}

void runSmithWaterman(char *v_string, char *w_string, enum ALIGNMENT_MODE mode, int threads) {
  float times[threads];

  for(int i = 0; i < threads; i++){
    initStart(v_string, w_string);
    if (mode == gap_blocks){
        initMatricesForBlocks(free_left_free_right, free_left_free_right);
    } else {
        initMatrix(free_left_free_right, free_left_free_right);
    }

    void *(*__start_routine)(void *) = mode == gap_blocks ? p_SmithWatermanBlocks : p_SmithWaterman;

    times[i] = runThreads(__start_routine, i+1, mode);

    // printf("%d,%d = %d\n", seq_w_size - 1, seq_v_size - 1, H[seq_w_size - 1][seq_v_size - 1]);

    pthread_mutex_destroy(&mutexWait);
    pthread_cond_destroy(&condWait);

    getAlignment(free_left_free_right, free_left_free_right);
    freeMatrix(H, seq_w_size);
    if(mode == gap_blocks){
      freeMatrix(B, seq_w_size);
      freeMatrix(C, seq_w_size);
    }
    freeMatrix(I_direction, seq_w_size);
    freeStrings();
    freeResults();
  }
  for(int i = 0; i < threads; i++){
    printf("%d threads took: %f\n", i, times[i]);
  }

  plotWithGnu();
  writeTimesToFile(times, threads, "temp_times.dat");
  plotTimes();

}

void *p_SmithWaterman(void *ptr_to_tdata) {
    thread_data_t *td = (thread_data_t *) ptr_to_tdata;
    int temp[4];
    int i, j, wave, tStart, tEnd;
    array_max_t arraymax;

    tStart = (ceil(((float)td->jmax / (float)td->numThreads)) * td->thread_id)+1;
    tEnd = (ceil(((float)td->jmax / (float)td->numThreads)) * (td->thread_id+1));

    //printf("Hello from thread: %d Numthreads:%d - %d - %d -!\n",td->thread_id,td->numThreads,tStart,tEnd);

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
            for (j = tStart; j <= tEnd; j++) {
              if(H[i][j] != INT_MIN){
                continue;
              }
                temp[0] = H[i - 1][j - 1] != INT_MIN ? H[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]) : INT_MIN;
                temp[1] = H[i - 1][j] != INT_MIN ? H[i - 1][j] + score_table.gap : INT_MIN;
                temp[2] = H[i][j - 1] != INT_MIN ? H[i][j - 1] + score_table.gap : INT_MIN;
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
    return ((void *) 0);
}

void *p_SmithWatermanBlocks(void *ptr_to_tdata) {
    thread_data_t *td = (thread_data_t *) ptr_to_tdata;
    int tempH[4];
    int tempB[4];
    int tempC[4];
    int i, j, wave, tStart, tEnd;
    array_max_t arraymax;

    tStart = (ceil(((float)td->jmax / (float)td->numThreads)) * td->thread_id)+1;
    tEnd = (ceil(((float)td->jmax / (float)td->numThreads)) * (td->thread_id+1));

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
            for (j = tStart; j <= tEnd; j++) {

                tempH[0] = H[i - 1][j - 1] != INT_MIN ? H[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]) : INT_MIN;
                tempH[1] = B[i - 1][j - 1] != INT_MIN ? B[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]) : INT_MIN;
                tempH[2] = C[i - 1][j - 1] != INT_MIN ? C[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]) : INT_MIN;
                tempH[3] = 0;
                arraymax = find_array_max(tempH, 4);
                H[i][j] = arraymax.max;
                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        I_direction[i][j] = TOP_LEFT_H;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        I_direction[i][j] = TOP_LEFT_B;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        I_direction[i][j] = TOP_LEFT_C;
                        break;
                    case 3:                                  // (i,j) is the beginning of a subsequence
                        I_direction[i][j] = NONE;
                        break;
                }

                tempB[0] = H[i - 1][j] != INT_MIN ? H[i - 1][j] + score_table.continue_block_cost + score_table.new_block_cost : INT_MIN;
                tempB[1] = B[i - 1][j] != INT_MIN ? B[i - 1][j] + score_table.continue_block_cost : INT_MIN;
                tempB[2] = C[i - 1][j] != INT_MIN ? C[i - 1][j]  + score_table.continue_block_cost + score_table.new_block_cost : INT_MIN;
                tempB[3] = 0;
                arraymax = find_array_max(tempB, 4);
                B[i][j] = arraymax.max;
                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        I_direction[i][j] = TOP_H;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        I_direction[i][j] = TOP_B;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        I_direction[i][j] = TOP_C;
                        break;
                    case 3:                                  // (i,j) is the beginning of a subsequence
                        I_direction[i][j] = NONE;
                        break;
                }

                tempC[0] = H[i][j - 1] != INT_MIN ? H[i][j - 1] + score_table.continue_block_cost + score_table.new_block_cost : INT_MIN;
                tempC[1] = B[i][j - 1] != INT_MIN ? B[i][j - 1] + score_table.continue_block_cost + score_table.new_block_cost : INT_MIN;
                tempC[2] = C[i][j - 1] != INT_MIN ? C[i][j - 1] + score_table.continue_block_cost : INT_MIN;
                tempC[3] = 0;
                arraymax = find_array_max(tempC, 4);
                C[i][j] = arraymax.max;
                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        I_direction[i][j] = LEFT_H;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        I_direction[i][j] = LEFT_B;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        I_direction[i][j] = LEFT_C;
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

    tStart = (ceil(((float)td->jmax / (float)td->numThreads)) * td->thread_id)+1;
    tEnd = (ceil(((float)td->jmax / (float)td->numThreads)) * (td->thread_id+1));

    // printf("Hello from thread: %d Numthreads:%d - %d - %d -!\n",td->thread_id,td->numThreads,tStart,tEnd);

    for (wave = 1; wave <= td->imax + td->numThreads - 1; wave++) {
        i = wave - td->thread_id;
        if (i >= 1 && i <= td->imax) {
            for (j = tStart; j <= tEnd; j++) {
              if(H[i][j] != INT_MIN){
                continue;
              }
                if(td->mode == k_band && !shouldFill(i, j)){
                  continue;
                }
                temp[0] = H[i - 1][j - 1] != INT_MIN ? H[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]) : INT_MIN;
                temp[1] = H[i - 1][j] != INT_MIN ? H[i - 1][j] + score_table.gap : INT_MIN;
                temp[2] = H[i][j - 1] != INT_MIN ? H[i][j - 1] + score_table.gap : INT_MIN;
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
    return ((void *) 0);
}

void *p_NeedlemanWunschBlock(void *ptr_to_tdata) {
    thread_data_t *td = (thread_data_t *) ptr_to_tdata;
    int tempH[3];
    int tempB[3];
    int tempC[3];
    int i, j, wave, tStart, tEnd;
    array_max_t arraymax;

    tStart = (ceil(((float)td->jmax / (float)td->numThreads)) * td->thread_id)+1;
    tEnd = (ceil(((float)td->jmax / (float)td->numThreads)) * (td->thread_id+1));

    for (wave = 1; wave <= td->imax + td->numThreads - 1; wave++) {
        i = wave - td->thread_id;
        if (i >= 1 && i <= td->imax) {
            for (j = tStart; j <= tEnd; j++) {
                if(!shouldFill(i, j) && td->mode == k_band){
                  continue;
                }

                tempH[0] = H[i - 1][j - 1] != INT_MIN ? H[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]) : INT_MIN;
                tempH[1] = B[i - 1][j - 1] != INT_MIN ? B[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]) : INT_MIN;
                tempH[2] = C[i - 1][j - 1] != INT_MIN ? C[i - 1][j - 1] + similarity_score(seq_w[i - 1], seq_v[j - 1]) : INT_MIN;
                arraymax = find_array_max(tempH, 3);
                H[i][j] = arraymax.max;

                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        I_direction[i][j] = TOP_LEFT_H;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        I_direction[i][j] = TOP_LEFT_B;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        I_direction[i][j] = TOP_LEFT_C;
                        break;
                }

                tempB[0] = H[i - 1][j] != INT_MIN ? H[i - 1][j] + score_table.continue_block_cost + score_table.new_block_cost : INT_MIN;
                tempB[1] = B[i - 1][j] != INT_MIN ? B[i - 1][j] + score_table.continue_block_cost : INT_MIN;
                tempB[2] = C[i - 1][j] != INT_MIN ? C[i - 1][j] + score_table.continue_block_cost + score_table.new_block_cost : INT_MIN;
                arraymax = find_array_max(tempB, 3);
                B[i][j] = arraymax.max;

                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        B_direction[i][j] = TOP_H;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        B_direction[i][j] = TOP_B;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        B_direction[i][j] = TOP_C;
                        break;
                }

                tempC[0] = H[i][j - 1] != INT_MIN ? H[i][j - 1] + score_table.continue_block_cost + score_table.new_block_cost : INT_MIN;
                tempC[1] = B[i][j - 1] != INT_MIN ? B[i][j - 1] + score_table.continue_block_cost + score_table.new_block_cost : INT_MIN;
                tempC[2] = C[i][j - 1] != INT_MIN ? C[i][j - 1] + score_table.continue_block_cost : INT_MIN;
                arraymax = find_array_max(tempC, 3);
                C[i][j] = arraymax.max;
                switch (arraymax.ind) {
                    case 0:                                  // score in (i,j) stems from a match/mismatch
                        C_direction[i][j] = LEFT_H;
                        break;
                    case 1:                                  // score in (i,j) stems from a deletion in sequence V
                        C_direction[i][j] = LEFT_B;
                        break;
                    case 2:                                  // score in (i,j) stems from a deletion in sequence W
                        C_direction[i][j] = LEFT_C;
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
