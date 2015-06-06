#include "file.h"


void readMatrixFromFile(char* fileName){
	FILE *file;
    char content[2048], *pch;
	file = fopen(fileName, "r");
	if(file == NULL){
		printf("Error reading file \n");
		exit(1);
    }
    score_table.type = 1;
    score_table.table = (int**) malloc(20*sizeof(int*));
    for(int i=0; i<20; i++){
        score_table.table[i] = (int*) malloc(20*sizeof(int)); 
    }
    int i = 0;
    int j = 0;
    while(fgets(content, 2048, file)){
        pch = strtok (content, ",");
        while (pch != NULL){
			score_table.table[i][j] = atoi(pch);
            pch = strtok(NULL, ",");
            j++;
        }
        i++;
    }
}
