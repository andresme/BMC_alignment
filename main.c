
#include "globals.h"
#include "utils.h"
#include "alignments.h"

int main(int argc, const char* argv[]) {

    if(argc < 3){
        printf("must contain at least 3 arguments");
        exit(-1);
    }


    seq_v_size = (int) strlen(argv[2]);
    seq_w_size = (int) strlen(argv[3]);

    seq_v = (char *) malloc(seq_v_size + 1 * sizeof(char));
    seq_w = (char *) malloc(seq_w_size + 1 * sizeof(char));

    threads = atoi(argv[1]);
    strcpy(seq_v, argv[2]) ;
    strcpy(seq_w, argv[3]);


    void *status;
    pthread_t callThd[threads];

    pthread_mutex_init(&mutexWait, NULL);
    pthread_cond_init(&condWait, NULL);

    thread_data_t *data = setup_thread_data(threads, seq_w_size, seq_v_size);
    init();

    clock_t t1, t2;

    waitingThreads = threads;
    for (int i = 0; i < threads; i++) {
        pthread_create(&callThd[i], NULL, p_NeedlemanWunsch, (void *) &data[i]);
    }

    t1 = clock();
    for (int i = 0; i < threads; i++) {
        pthread_join(callThd[i], &status);
    }
    t2 = clock();

    float diff = (((float) t2 - (float) t1) / 1000000.0F) * 1000;
    printf("it took: %f ms for %d threads\n", diff, threads);

    printf("%d\n", H[seq_w_size][seq_v_size]);

    pthread_mutex_destroy(&mutexWait);
    pthread_cond_destroy(&condWait);
    pthread_exit(NULL);
}