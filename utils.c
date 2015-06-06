#include "utils.h"

thread_data_t *setup_thread_data(int num_threads, int imax, int jmax, enum ALIGNMENT_MODE mode) {

    thread_data_t *tdata;
    tdata = (thread_data_t *) malloc(num_threads * sizeof(thread_data_t));

    for (int i = 0; i < num_threads; i++) {
        tdata[i].thread_id = i;
        tdata[i].numThreads = num_threads;
        tdata[i].imax = imax;
        tdata[i].jmax = jmax;
        tdata[i].mode = mode;
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

    strcpy(seq_v, v_string);
    strcpy(seq_w, w_string);

    printf("%s\n%s\n", v_string, w_string);
    printf("%s\n%s\n", seq_v, seq_w);

    pthread_mutex_init(&mutexWait, NULL);
    pthread_mutex_init(&mutexStart, NULL);
    pthread_cond_init(&condWait, NULL);
    return 1;
}

int initDirectionsMatrix() {
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;

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

void printMatrix(){
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;

    for(int i = 0; i < size_w; i++){
        for(int j = 0; j < size_v; j++){
           printf("%d\t", H[i][j]);
       }
       printf("\n");
   }

   printf("----------------------\n");
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
            H[i][j] = INT_MIN;
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

    //printMatrix(H);
    return initDirectionsMatrix();
}

int initMatricesForBlocks(enum GAP_TYPE v_type, enum GAP_TYPE w_type){
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

    B = (int **) malloc(size_w * sizeof(int *));
    if(B == NULL){
        printf("could not allocate memory\n");
        return -1;
    }
    for(int i = 0; i < size_w; i++){
        B[i] = (int *) malloc(size_v * sizeof(int));
        if(B[i] == NULL){
            printf("could not allocate memory\n");
            return -1;
        }
    }

    C = (int **) malloc(size_w * sizeof(int *));
    if(C == NULL){
        printf("could not allocate memory\n");
        return -1;
    }
    for(int i = 0; i < size_w; i++){
        C[i] = (int *) malloc(size_v * sizeof(int));
        if(C[i] == NULL){
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for(int i = 0; i < size_w; i++){
        for(int j = 0; j < size_v; j++){
            H[i][j] = INT_MIN;
            if (i==0 && j == 0) {
                H[i][j] = 0; 
            }
        }
    }

    for(int i = 0; i < size_w; i++){
        for(int j = 0; j < size_v; j++){
            B[i][j] = INT_MIN;
            if(i == 0 && j != 0){
                switch(w_type){
                       case free_left_free_right:
                       case free_left_penalty_right:
                           B[i][j] = 0;
                        break;
                       default:
                           B[i][j] = j * score_table.gap;
                        break;
                }
            }
        }
    }

    for(int i = 0; i < size_w; i++){
        for(int j = 0; j < size_v; j++){
            C[i][j] = INT_MIN;
            if(j == 0 && i != 0){
                switch(v_type){
                    case penalty_left_free_right:
                    case penalty_left_penalty_right:
                        C[i][j] = i * score_table.gap;
                        break;
                    default:
                        C[i][j] = 0;
                        break;
                }
            }
        }
    }

    return 1;
}

bool shouldFill(int i, int j){
  int center = i - j;
  int temp;
  if(seq_w_size > seq_v_size){
    temp = seq_w_size - seq_v_size + current_k;
    return current_k <= center && center <= temp;
  } else {
    temp = seq_w_size - seq_v_size - current_k;
    return temp <= center && center <= current_k;
  }
}


void init_k_band(enum GAP_TYPE v_type, enum GAP_TYPE w_type) {
  int size_w = seq_w_size + 1;
  int size_v = seq_v_size + 1;

  for(int i = 0; i < size_w; i++){
      for(int j = 0; j < size_v; j++){
          H[i][j] = INT_MIN;
          if(i == 0 && shouldFill(i, j)){
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
          if(j == 0 && shouldFill(i, j)){
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
}

void clear(enum GAP_TYPE v_type, enum GAP_TYPE w_type){
  int size_w = seq_w_size + 1;
  int size_v = seq_v_size + 1;

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
}
