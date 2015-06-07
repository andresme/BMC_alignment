#include "globals.h"
#include "utils.h"
#include "alignments.h"

int main(int argc, const char* argv[]) {

    score_table.gap = -2;
    score_table.match = 1;
    score_table.missmatch = -1;
    score_table.new_block_cost = 2;
    score_table.continue_block_cost = 1;

//    for(int i = 1; i <= 4; i++){

    runNeedlemanWunsch(free_left_penalty_right, free_left_penalty_right, "ATTGTGATCC", "TATTGTGATCCACTGCTCAT", none, 1, 0, 1);
//        runSmithWaterman("TCTAACGGTA", "CGCATCTCGA", 0, 4);
//    }


    pthread_exit(NULL);
}
