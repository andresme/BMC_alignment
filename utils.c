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


int getIndexFromChar(char c) {
    if (c == 'A') {
        return 0;
    } else if (c == 'R') {
        return 1;
    } else if (c == 'D') {
        return 2;
    } else if (c == 'N') {
        return 3;
    } else if (c == 'C') {
        return 4;
    } else if (c == 'E') {
        return 5;
    } else if (c == 'Q') {
        return 6;
    } else if (c == 'G') {
        return 7;
    } else if (c == 'H') {
        return 8;
    } else if (c == 'I') {
        return 9;
    } else if (c == 'L') {
        return 10;
    } else if (c == 'K') {
        return 11;
    } else if (c == 'M') {
        return 12;
    } else if (c == 'F') {
        return 13;
    } else if (c == 'P') {
        return 14;
    } else if (c == 'S') {
        return 15;
    } else if (c == 'T') {
        return 16;
    } else if (c == 'W') {
        return 17;
    } else if (c == 'Y') {
        return 18;
    } else if (c == 'V') {
        return 19;
    }
}

int similarity_score(char a, char b) {
    if (score_table.type == 0) {
        if (a == b) {
            return score_table.match;
        } else {
            return score_table.missmatch;
        }
    } else {
        int i = getIndexFromChar(a);
        int j = getIndexFromChar(b);
        return score_table.table[i][j];
    }
}

int initStart(char *v_string, char *w_string) {
    seq_v_size = (int) strlen(v_string);
    seq_w_size = (int) strlen(w_string);

    seq_v = (char *) malloc(seq_v_size + 1 * sizeof(char));
    seq_w = (char *) malloc(seq_w_size + 1 * sizeof(char));

    if (seq_v == NULL || seq_w == NULL) {
        printf("Could not alloc strings\n");
        return -1;
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
    if (I_direction == NULL) {
        printf("could not allocate memory\n");
        return -1;
    }
    for (int i = 0; i < size_w; i++) {
        I_direction[i] = (int *) malloc(size_v * sizeof(int));
        if (I_direction[i] == NULL) {
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            if (j == 0) {
                I_direction[i][j] = TOP;
            } else if (i == 0) {
                I_direction[i][j] = LEFT;
            } else {
                I_direction[i][j] = NONE;
            }
        }
    }
    I_direction[0][0] = NONE;
    return 1;
}

int initDirectionsMatrixB() {
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;

    B_direction = (int **) malloc(size_w * sizeof(int *));
    if (B_direction == NULL) {
        printf("could not allocate memory\n");
        return -1;
    }
    for (int i = 0; i < size_w; i++) {
        B_direction[i] = (int *) malloc(size_v * sizeof(int));
        if (B_direction[i] == NULL) {
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            if (j == 0) {
                B_direction[i][j] = TOP;
            } else if (i == 0) {
                B_direction[i][j] = LEFT;
            } else {
                B_direction[i][j] = NONE;
            }
        }
    }
    B_direction[0][0] = NONE;
    return 1;
}

int initDirectionsMatrixC() {
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;

    C_direction = (int **) malloc(size_w * sizeof(int *));
    if (C_direction == NULL) {
        printf("could not allocate memory\n");
        return -1;
    }
    for (int i = 0; i < size_w; i++) {
        C_direction[i] = (int *) malloc(size_v * sizeof(int));
        if (C_direction[i] == NULL) {
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            if (j == 0) {
                C_direction[i][j] = TOP;
            } else if (i == 0) {
                C_direction[i][j] = LEFT;
            } else {
                C_direction[i][j] = NONE;
            }
        }
    }
    C_direction[0][0] = NONE;
    return 1;
}

void printMatrix(int **matrix) {
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    printf("----------------------\n");
}

int initMatrix(enum GAP_TYPE v_type, enum GAP_TYPE w_type) {
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;

    H = (int **) malloc(size_w * sizeof(int *));
    if (H == NULL) {
        printf("could not allocate memory\n");
        return -1;
    }
    for (int i = 0; i < size_w; i++) {
        H[i] = (int *) malloc(size_v * sizeof(int));
        if (H[i] == NULL) {
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            H[i][j] = INT_MIN;
            if (i == 0) {
                switch (w_type) {
                    case free_left_free_right:
                    case free_left_penalty_right:
                        H[i][j] = 0;
                        break;
                    default:
                        H[i][j] = j * score_table.gap;
                        break;
                }
            }
            if (j == 0) {
                switch (v_type) {
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

int initMatricesForBlocks(enum GAP_TYPE v_type, enum GAP_TYPE w_type) {
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;

    H = (int **) malloc(size_w * sizeof(int *));
    if (H == NULL) {
        printf("could not allocate memory\n");
        return -1;
    }
    for (int i = 0; i < size_w; i++) {
        H[i] = (int *) malloc(size_v * sizeof(int));
        if (H[i] == NULL) {
            printf("could not allocate memory\n");
            return -1;
        }
    }

    B = (int **) malloc(size_w * sizeof(int *));
    if (B == NULL) {
        printf("could not allocate memory\n");
        return -1;
    }
    for (int i = 0; i < size_w; i++) {
        B[i] = (int *) malloc(size_v * sizeof(int));
        if (B[i] == NULL) {
            printf("could not allocate memory\n");
            return -1;
        }
    }

    C = (int **) malloc(size_w * sizeof(int *));
    if (C == NULL) {
        printf("could not allocate memory\n");
        return -1;
    }
    for (int i = 0; i < size_w; i++) {
        C[i] = (int *) malloc(size_v * sizeof(int));
        if (C[i] == NULL) {
            printf("could not allocate memory\n");
            return -1;
        }
    }

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            H[i][j] = INT_MIN;
            if (i == 0 && j == 0) {
                H[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            B[i][j] = INT_MIN;
            if (i == 0 && j != 0) {
                switch (w_type) {
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

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            C[i][j] = INT_MIN;
            if (j == 0 && i != 0) {
                switch (v_type) {
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

    if (!initDirectionsMatrix())
        return 0;
    if (!initDirectionsMatrixB())
        return 0;

    return initDirectionsMatrixC();
}

bool shouldFill(int i, int j) {
    int center = i - j;
    int temp;
    if (seq_w_size > seq_v_size) {
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

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            H[i][j] = INT_MIN;
            if (i == 0 && shouldFill(i, j)) {
                switch (w_type) {
                    case free_left_free_right:
                    case free_left_penalty_right:
                        H[i][j] = 0;
                        break;
                    default:
                        H[i][j] = j * score_table.gap;
                        break;
                }
            }
            if (j == 0 && shouldFill(i, j)) {
                switch (v_type) {
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

void clear(enum GAP_TYPE v_type, enum GAP_TYPE w_type) {
    int size_w = seq_w_size + 1;
    int size_v = seq_v_size + 1;

    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            if (j == 0) {
                I_direction[i][j] = TOP;
            } else if (i == 0) {
                I_direction[i][j] = LEFT;
            } else {
                I_direction[i][j] = NONE;
            }

        }
    }
    for (int i = 0; i < size_w; i++) {
        for (int j = 0; j < size_v; j++) {
            H[i][j] = 0;
            if (i == 0) {
                switch (w_type) {
                    case free_left_free_right:
                    case free_left_penalty_right:
                        H[i][j] = 0;
                        break;
                    default:
                        H[i][j] = j * score_table.gap;
                        break;
                }
            }
            if (j == 0) {
                switch (v_type) {
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

void plotWithGnu() {
    int status;
    if (fork() == 0) {
        status = system("./gnuplot.gp");
        exit(0);
    }
}

void plotWithGnuPath() {
    int status;
    if (fork() == 0) {
        status = system("./gnuplot_path.gp");
        exit(0);
    }
}

void plotWithGnuB() {
    int status;
    if (fork() == 0) {
        status = system("./gnuplot_B.gp");
        exit(0);
    }
}

void plotWithGnuPathB() {
    int status;
    if (fork() == 0) {
        status = system("./gnuplot_path_B.gp");
        exit(0);
    }
}

void plotWithGnuC() {
    int status;
    if (fork() == 0) {
        status = system("./gnuplot_C.gp");
        exit(0);
    }
}

void plotWithGnuPathC() {
    int status;
    if (fork() == 0) {
        status = system("./gnuplot_path_C.gp");
        exit(0);
    }
}

void plotTimesKBand() {
    int status;
    if (fork() == 0) {
        status = system("./gnuplot_times_k_band.gp");
        exit(0);
    }
}


void plotTimes() {
    int status;
    if (fork() == 0) {
        status = system("./gnuplot_times.gp");
        exit(0);
    }
}


void printMatrixToFile(char *fileName, int **matrix) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(file, "%c\t", 'e');

    for (int i = 0; i < seq_v_size; i++) {
        fprintf(file, "%c\t", seq_v[i]);
    }
    fprintf(file, "\n%c\t", seq_w[seq_w_size - 1]);

    for (int i = seq_w_size; i >= 0; i--) {
        for (int j = 0; j <= seq_v_size; j++) {
            if (matrix[i][j] == INT_MIN) {
                fprintf(file, "%f\t", NAN);
            } else {
                fprintf(file, "%d\t", matrix[i][j]);
            }
        }
        if (i > 1) {
            fprintf(file, "\n%c\t", seq_w[i - 2]);
        } else if (i == 1) {
            fprintf(file, "\ne\t");
        } else {
            fprintf(file, "\n");
        }

    }
    fclose(file);
}

void writeTimesToFile(float times[], int threads, char *fileName) {
    printf("here\n");
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < threads; i++) {
        fprintf(file, "%d\t%f\n", i + 1, times[i]);
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
    // printf("%d,%d\n", max_i, max_j);

    int i = max_i;
    int j = max_j;
    int count = 0;
    while (i >= 0 && j >= 0) {
        if (I_direction[i][j] == TOP_LEFT) {
            //printf("Top left\n");
            i = i - 1;
            j = j - 1;
        } else if (I_direction[i][j] == TOP) {
            //  printf("Top\n");
            i = i - 1;
        } else if (I_direction[i][j] == LEFT) {
            //  printf("Left\n");
            j = j - 1;
        } else {
            break;
        }
        count++;
    }
    // printf("count: %d\n", count);
    //7,13
    //13,13

    if (max_i < seq_w_size) {
        error_i = seq_w_size - max_i;
        count += error_i;
    } else if (max_j < seq_v_size) {
        error_j = seq_v_size - max_j;
        count += error_j;
    }

    // printf("count: %d\n", count);

    //printf("count: %d\n", count);
    string_alignment.v_string = (char *) malloc((count + 1) * sizeof(char));
    string_alignment.w_string = (char *) malloc((count + 1) * sizeof(char));

    i = max_i;
    j = max_j;
    int str_index = count - 1;

    temp = 0;
    while (seq_w_size - temp >= i + 1) {
        string_alignment.v_string[str_index] = '-';
        string_alignment.w_string[str_index] = seq_w[seq_w_size - temp - 1];
        // printf("%c , %c : %d\n", '-', seq_w[seq_w_size-temp-1], str_index);
        temp++;
        str_index--;
    }


    temp = 0;
    while (seq_v_size - temp >= j + 1) {
        string_alignment.v_string[str_index] = seq_v[seq_v_size - temp - 1];
        string_alignment.w_string[str_index] = '-';
        // printf("%c , %c : %d\n", seq_v[seq_v_size-temp-1], '-', str_index);
        temp++;
        str_index--;
    }


    printMatrixToFile("temp_matrix.dat", H);
    string_alignment.best_score = H[seq_w_size][seq_v_size];

    FILE *path = fopen("temp_lines.dat", "w");
    if (path == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    while (i >= 0 && j >= 0) {
        fprintf(path, "%d\t%d\n", j, max_i - i + error_i);
        if (I_direction[i][j] == TOP_LEFT) {
            string_alignment.v_string[str_index] = seq_v[j - 1] = seq_v[j - 1];
            string_alignment.w_string[str_index] = seq_w[i - 1];
            // printf("%c , %c : %d\n", string_alignment.v_string[str_index], string_alignment.w_string[str_index], str_index);
            i = i - 1;
            j = j - 1;
        } else if (I_direction[i][j] == TOP) {
            string_alignment.v_string[str_index] = '-';
            string_alignment.w_string[str_index] = seq_w[i - 1];
            // printf("%c , %c : %d\n", string_alignment.v_string[str_index], string_alignment.w_string[str_index], str_index);
            i = i - 1;
        } else if (I_direction[i][j] == LEFT) {
            string_alignment.v_string[str_index] = seq_v[j - 1];
            string_alignment.w_string[str_index] = '-';
            // printf("%c , %c : %d\n", string_alignment.v_string[str_index], string_alignment.w_string[str_index], str_index);
            j = j - 1;
        } else {
            break;
        }
        str_index--;
    }
    fclose(path);

    string_alignment.v_string[count] = '\0';
    string_alignment.w_string[count] = '\0';
}

void getAlignmentBlock(enum GAP_TYPE v_type, enum GAP_TYPE w_type) {
    printf("GET ALIGNMENT BLOCK\n");
    int max_i_H = seq_w_size;
    int max_j_H = seq_v_size;
    int max_i_B = seq_w_size;
    int max_j_B = seq_v_size;
    int max_i_C = seq_w_size;
    int max_j_C = seq_v_size;
    int error_i = 0;
    int error_j = 0;

    bool v_type_value = v_type == free_left_free_right || v_type == penalty_left_free_right;
    bool w_type_value = w_type == free_left_free_right || w_type == penalty_left_free_right;
    int temp = H[seq_w_size][seq_v_size];
    if (v_type_value && w_type_value) {
        for (int i = seq_w_size; i > 0; i--) {
            for (int j = seq_v_size; j > 0; j--) {
                if (H[seq_w_size][j] > temp) {
                    temp = H[seq_w_size][j];
                    max_i_H = seq_w_size;
                    max_j_H = j;
                }
            }
            if (H[i][seq_v_size] > temp) {
                temp = H[i][seq_v_size];
                max_i_H = i;
                max_j_H = seq_v_size;
            }
        }
    } else if (v_type_value) {
        for (int j = seq_v_size; j > 0; j--) {
            if (H[seq_w_size][j] > temp) {
                temp = H[seq_w_size][j];
                max_j_H = j;
            }
        }
    } else if (w_type_value) {
        for (int i = seq_w_size; i > 0; i--) {
            if (H[i][seq_v_size] > temp) {
                temp = H[i][seq_v_size];
                max_i_H = i;
            }
        }
    }

    temp = B[seq_w_size][seq_v_size];
    if (v_type_value && w_type_value) {
        for (int i = seq_w_size; i > 0; i--) {
            for (int j = seq_v_size; j > 0; j--) {
                if (B[seq_w_size][j] > temp) {
                    temp = H[seq_w_size][j];
                    max_i_B = seq_w_size;
                    max_j_B = j;
                }
            }
            if (B[i][seq_v_size] > temp) {
                temp = B[i][seq_v_size];
                max_i_B = i;
                max_j_B = seq_v_size;
            }
        }
    } else if (v_type_value) {
        for (int j = seq_v_size; j > 0; j--) {
            if (B[seq_w_size][j] > temp) {
                temp = B[seq_w_size][j];
                max_j_B = j;
            }
        }
    } else if (w_type_value) {
        for (int i = seq_w_size; i > 0; i--) {
            if (B[i][seq_v_size] > temp) {
                temp = B[i][seq_v_size];
                max_i_B = i;
            }
        }
    }

    temp = C[seq_w_size][seq_v_size];
    if (v_type_value && w_type_value) {
        for (int i = seq_w_size; i > 0; i--) {
            for (int j = seq_v_size; j > 0; j--) {
                if (C[seq_w_size][j] > temp) {
                    temp = C[seq_w_size][j];
                    max_i_C = seq_w_size;
                    max_j_C = j;
                }
            }
            if (C[i][seq_v_size] > temp) {
                temp = C[i][seq_v_size];
                max_i_C = i;
                max_j_C = seq_v_size;
            }
        }
    } else if (v_type_value) {
        for (int j = seq_v_size; j > 0; j--) {
            if (C[seq_w_size][j] > temp) {
                temp = C[seq_w_size][j];
                max_j_C = j;
            }
        }
    } else if (w_type_value) {
        for (int i = seq_w_size; i > 0; i--) {
            if (C[i][seq_v_size] > temp) {
                temp = C[i][seq_v_size];
                max_i_C = i;
            }
        }
    }

    int **currentMatrix;
    int i, aux_i;
    int j, aux_j;

    int maxPerTable[3] = {H[max_i_H][max_j_H], B[max_i_B][max_j_B], C[max_i_C][max_j_C]};

    array_max_t arraymax = find_array_max(maxPerTable, 3);

    switch (arraymax.ind) {
        case 0:
            currentMatrix = H;
            i = aux_i = max_i_H;
            j = aux_j = max_j_H;
            break;
        case 1:
            currentMatrix = B;
            i = aux_i = max_i_B;
            j = aux_j = max_j_B;
            break;
        case 2:
            currentMatrix = C;
            i = aux_i = max_i_C;
            j = aux_j = max_j_C;
            break;
    }

    printMatrix(I_direction);
    printMatrix(B_direction);
    printMatrix(C_direction);

    int count = 0;
    while (aux_i >= 0 && aux_j >= 0) {
        switch (I_direction[aux_i][aux_j]) {
            case TOP_LEFT:
            case TOP_LEFT_H:
            case TOP_LEFT_B:
            case TOP_LEFT_C:
                aux_i--;
                aux_j--;
                break;
            case TOP:
            case TOP_H:
            case TOP_B:
            case TOP_C:
                aux_i--;
                break;
            case LEFT:
            case LEFT_H:
            case LEFT_B:
            case LEFT_C:
                aux_j--;
                break;
            case NONE:
                aux_i--;
                aux_j--;
                break;
        }
        count++;
    }

    if (i < seq_w_size) {
        error_i = seq_w_size - i;
        count += error_i;
    } else if (j < seq_v_size) {
        error_j = seq_v_size - j;
        count += error_j;
    }

    string_alignment.v_string = (char *) malloc((count + 1) * sizeof(char));
    string_alignment.w_string = (char *) malloc((count + 1) * sizeof(char));

    int str_index = count - 1;
    temp = 0;
    while (seq_w_size - temp >= i + 1) {
        string_alignment.v_string[str_index] = '-';
        string_alignment.w_string[str_index] = seq_w[seq_w_size - temp - 1];
        temp++;
        str_index--;
    }

    temp = 0;
    while (seq_v_size - temp >= j + 1) {
        string_alignment.v_string[str_index] = seq_v[seq_v_size - temp - 1];
        string_alignment.w_string[str_index] = '-';
        temp++;
        str_index--;
    }

    printf("Filled gaps\n");

    string_alignment.best_score = arraymax.max;

    FILE *path = fopen("temp_lines.dat", "w");
    if (path == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    while (i >= 0 && j >= 0) {
        switch (I_direction[aux_i][aux_j]) {
            case TOP_LEFT_H:
            case TOP_LEFT_B:
            case TOP_LEFT_C:
                string_alignment.v_string[str_index] = seq_v[j - 1] = seq_v[j - 1];
                string_alignment.w_string[str_index] = seq_w[i - 1];
                i--;
                j--;
                break;
            case TOP_H:
            case TOP_B:
            case TOP_C:
                string_alignment.v_string[str_index] = '-';
                string_alignment.w_string[str_index] = seq_w[i - 1];
                i--;
                break;
            case LEFT_H:
            case LEFT_B:
            case LEFT_C:
                string_alignment.v_string[str_index] = seq_v[j - 1];
                string_alignment.w_string[str_index] = '-';
                j--;
                break;
        }
        str_index--;
    }
    fclose(path);

    string_alignment.v_string[count] = '\0';
    string_alignment.w_string[count] = '\0';
}

void freeThreadData(thread_data_t *data) {
    free(data);
}

void freeStrings() {
    free(seq_v);
    free(seq_w);
}

void freeResults() {
    free(string_alignment.v_string);
    free(string_alignment.w_string);
}

void freeMatrix(int **matrix, int size_i) {
    for (int i = 0; i < size_i; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
