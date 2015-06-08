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
  if(score_table.type == 0){
    if (a == b) {
        return score_table.match;
    } else {
        return score_table.missmatch;
    }
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
    I_direction[0][0] = NONE;
    return 1;
}

void printMatrix(int **matrix){
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;

    for(int i = 0; i < size_w; i++){
        for(int j = 0; j < size_v; j++){
           printf("%d\t", matrix[i][j]);
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
                           B[i][j] = j * score_table.continue_block_cost + score_table.new_block_cost;
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
                        C[i][j] = i * score_table.continue_block_cost + score_table.new_block_cost;
                        break;
                    default:
                        C[i][j] = 0;
                        break;
                }
            }
        }
    }

    return initDirectionsMatrix();
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

void plotWithGnu(){
  int status;
    // By calling fork(), a child process will be created as a exact duplicate of the calling process.
    // // Search for fork() (maybe "man fork" on Linux) for more information.
    // if(seq_w_size > 20 || seq_v_size > 20){
    //   if(fork() == 0){
    //     // Child process will return 0 from fork()
    //     status = system("./gnuplot_large.gp");
    //     exit(0);
    //   }
    // } else {
      if(fork() == 0){
        // Child process will return 0 from fork()
        status = system("./gnuplot_large.gp");
        exit(0);
      }
    // }
}

void printMatrixToFile(char *fileName, int **matrix){
  FILE *file = fopen(fileName, "w");
  if (file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }
  for(int i = seq_w_size; i >= 0 ; i--){
    for(int j = 0; j <= seq_v_size; j++){
      fprintf(file, "%d\t", matrix[i][j]);
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

void getAlignment(enum GAP_TYPE v_type, enum GAP_TYPE w_type) {
    int max_i = seq_w_size;
    int max_j = seq_v_size;
    int error_i = 0;
    int error_j = 0;

    //printMatrix(I_direction);

    bool v_type_value = v_type == free_left_free_right || v_type == penalty_left_free_right;
    bool w_type_value = w_type == free_left_free_right || w_type == penalty_left_free_right;
    int temp = H[seq_w_size][seq_v_size];
    if (v_type_value && w_type_value) {
        for (int i = seq_w_size; i > 0; i--) {
            for (int j = seq_v_size; j > 0; j--) {
                if (H[seq_w_size][j] > temp) {
                    temp = H[seq_w_size][j];
                    max_i = seq_w_size;
                    max_j = j;
                }
            }
            if (H[i][seq_v_size] > temp) {
                temp = H[i][seq_v_size];
                max_i = i;
                max_j = seq_v_size;
            }
        }
    } else if (v_type_value) {
        for (int j = seq_v_size; j > 0; j--) {
            if (H[seq_w_size][j] > temp) {
                temp = H[seq_w_size][j];
                max_j = j;
            }
        }
    } else if (w_type_value) {
        for (int i = seq_w_size; i > 0; i--) {
            if (H[i][seq_v_size] > temp) {
                temp = H[i][seq_v_size];
                max_i = i;
            }
        }
    }
    printf("%d,%d\n", max_i, max_j);

    int i = max_i;
    int j = max_j;
    int count = 0;
    while(i >= 0 && j >= 0){
        if(I_direction[i][j] == TOP_LEFT){
          //printf("Top left\n");
            i = i-1;
            j = j-1;
        } else if(I_direction[i][j] == TOP){
        //  printf("Top\n");
            i = i-1;
        } else if(I_direction[i][j] == LEFT){
        //  printf("Left\n");
            j = j-1;
        } else {
            break;
        }
        count++;
    }
    printf("count: %d\n", count);
    //7,13
    //13,13

    if(max_i < seq_w_size){
      error_i = seq_w_size-max_i;
      count += error_i;
    } else if(max_j < seq_v_size){
      error_j = seq_v_size-max_j;
      count += error_j;
    }

    printf("count: %d\n", count);

    //printf("count: %d\n", count);
    string_alignment.v_string = (char *) malloc(count * sizeof(char));
    string_alignment.w_string = (char *) malloc(count * sizeof(char));

    i = max_i;
    j = max_j;
    int str_index = count-1;

    temp = 0;
    while(seq_w_size - temp >= i+1){
      string_alignment.v_string[str_index] = '-';
      string_alignment.w_string[str_index] = seq_w[seq_w_size - temp - 1];
      printf("%c , %c : %d\n", '-', seq_w[seq_w_size-temp-1], str_index);
      temp++;
      str_index--;
    }


    temp = 0;
    while(seq_v_size - temp >= j+1){
      string_alignment.v_string[str_index] = seq_v[seq_v_size - temp - 1];
      string_alignment.w_string[str_index] = '-';
      printf("%c , %c : %d\n", seq_v[seq_v_size-temp-1], '-', str_index);
      temp++;
      str_index--;
    }


    printMatrixToFile("temp_matrix.dat", H);

    FILE *path = fopen("temp_lines.dat", "w");
    if (path == NULL) {
      printf("Error opening file!\n");
      exit(1);
    }

    while(i >= 0 && j >= 0){
      fprintf(path, "%d\t%d\n", j, max_i-i+error_i);
        if(I_direction[i][j] == TOP_LEFT){
            string_alignment.v_string[str_index] = seq_v[j-1];
            string_alignment.w_string[str_index] = seq_w[i-1];
            printf("%c , %c : %d\n", seq_v[j-1], seq_w[i-1], str_index);
            i = i-1;
            j = j-1;
        } else if(I_direction[i][j] == TOP){
            string_alignment.v_string[str_index] = '-';
            string_alignment.w_string[str_index] = seq_w[i-1];
            printf("%c , %c : %d\n", '-', seq_w[i-1], str_index);
            i = i-1;
        } else if(I_direction[i][j] == LEFT){
            string_alignment.v_string[str_index] = seq_v[j-1];
            string_alignment.w_string[str_index] = '-';
            printf("%c , %c : %d\n", seq_v[j-1], '-', str_index);
            j = j-1;
        } else {
            break;
        }
        str_index--;
    }
    fclose(path);

    string_alignment.v_string[count] = '\0';
    string_alignment.w_string[count] = '\0';

    plotWithGnu();
    printf("%s\n%s\n", string_alignment.v_string, string_alignment.w_string);
}
