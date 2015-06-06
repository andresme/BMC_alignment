#include "utils.h"

thread_data_t *setup_thread_data(int num_threads, int imax, int jmax) {

    thread_data_t *tdata;
    tdata = (thread_data_t *) malloc(num_threads * sizeof(thread_data_t));

    for (int i = 0; i < num_threads; i++) {
        tdata[i].thread_id = i;
        tdata[i].numThreads = num_threads;
        tdata[i].imax = imax;
        tdata[i].jmax = jmax;
    }

    return tdata;
}


array_max_t find_array_max(int temp[], int size) {
    array_max_t max = {0, temp[0]};
    for (int i = 1; i < size; i++) {
        if (temp[i] > max.max) {
            max.ind = i;
            max.max = temp[i];
        }
    }
    return max;
}


int similarity_score(char a, char b) {
    if (a == b) {
        return score_table.match;
    } else {
        return score_table.missmatch;
    }
}

int initStart(int numThreads, char *v_string, char *w_string){
    seq_v_size = (int) strlen(v_string);
    seq_w_size = (int) strlen(w_string);

    seq_v = (char *) malloc(seq_v_size + 1 * sizeof(char));
    seq_w = (char *) malloc(seq_w_size + 1 * sizeof(char));

    if(seq_v == NULL || seq_w == NULL){
      printf("Could not alloc strings\n");
      exit(-1);
    }

    strcpy(seq_v, v_string) ;
    strcpy(seq_w, w_string);

    pthread_mutex_init(&mutexWait, NULL);
    pthread_mutex_init(&mutexStart, NULL);
    pthread_cond_init(&condWait, NULL);
    return 1;
}

int initMatrix(enum GAP_TYPE v_type, enum GAP_TYPE w_type){
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;
    H = (int **) malloc(size_w * sizeof(int *));
    if(H == NULL){
        printf("could not allocate memory\n");
        return -1;
    }
    for(int i = 0; i < size_w; i++){
        H[i] = (int *) malloc(size_v * sizeof(int));
        if(H[i] == NULL){
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for(int i = 0; i < size_w; i++){
        for(int j = 0; j < size_v; j++){
            H[i][j] = 0;
            if(i == 0){
                switch(w_type){
                       case free_left_free_right:
                       case free_left_penalty_right:
                           H[i][j] = 0;
                        break;
                       default:
                           H[i][j] = j * score_table.gap;
                        break;
                }
            }
            if(j == 0){
                switch(v_type){
                    case penalty_left_free_right:
                    case penalty_left_penalty_right:
                        H[i][j] = i * score_table.gap;
                        break;
                    default:
                        H[i][j] = 0;
                        break;
                }
            }

        }
    }

//    for(int i = 0; i < seq_w_size; i++){
//        for(int j = 0; j < seq_v_size; j++){
//            printf("%d\t", H[i][j]);
//        }
//        printf("\n");
//    }
//
//    printf("----------------------\n");

    I_direction = (int **) malloc(size_w * sizeof(int *));
    if(I_direction == NULL){
        printf("could not allocate memory\n");
        return -1;
    }
    for(int i = 0; i < size_w; i++){
        I_direction[i] = (int *) malloc(size_v * sizeof(int));
        if(I_direction[i] == NULL){
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for(int i = 0; i < size_w; i++){
        for(int j = 0; j < size_v; j++){
            if(j == 0){
                I_direction[i][j] = TOP;
            } else if(i == 0){
                I_direction[i][j] = LEFT;
            } else {
                I_direction[i][j] = NONE;
            }

        }
    }
    return 1;
}

bool shouldFill(int i, int j, int k){
  int left_side, center, right_side;
  if(seq_w_size > seq_v_size){
    left_side = seq_v_size - seq_w_size - k;
    center = j - i;
    return left_side <= center && center <= k;
  } else {
    left_side = seq_v_size - seq_w_size - k;
    center = i - j;
    return left_side <= center && center <= k;
  }
}