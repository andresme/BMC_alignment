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
        return 1;
    } else {
        return -1;
    }
}

int init(){
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
        }
    }

    I_i = (int **) malloc(size_w * sizeof(int *));
    if(I_i == NULL){
        printf("could not allocate memory\n");
        return -1;
    }
    for(int i = 0; i < size_w; i++){
        I_i[i] = (int *) malloc(size_v * sizeof(int));
        if(I_i[i] == NULL){
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for(int i = 0; i < size_w; i++){
        for(int j = 0; j < size_v; j++){
            I_i[i][j] = 0;
        }
    }

    I_j = (int **) malloc(size_w * sizeof(int *));
    if(I_j == NULL){
        printf("could not allocate memory\n");
        return -1;
    }
    for(int i = 0; i < size_w; i++){
        I_j[i] = (int *) malloc(size_v * sizeof(int));
        if(I_j[i] == NULL){
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for(int i = 0; i < size_w; i++){
        for(int j = 0; j < size_v; j++){
            I_j[i][j] = 0;
        }
    }
    return 1;
}