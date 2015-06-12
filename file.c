#include "file.h"


void readMatrixFromFile(char *fileName) {
    FILE *file;
    char content[2048], *pch;
    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error reading file \n");
        return;
    }
    score_table.type = 1;
    score_table.table = (int **) malloc(20 * sizeof(int *));
    for (int i = 0; i < 20; i++) {
        score_table.table[i] = (int *) malloc(20 * sizeof(int));
    }
    int i = 0;
    int j = 0;
    while (fgets(content, 2048, file)) {
        pch = strtok(content, " ");
        while (pch != NULL && j < 20) {
            score_table.table[i][j] = atoi(pch);
            pch = strtok(NULL, " ");
            j++;
        }
        j = 0;
        i++;
    }
    fclose(file);
}

//remember to free result somewhere
char *readStringFromFile(char *fileName) {
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen(fileName, "rb");
    if (!fp) {
        printf("Error reading file \n");
        return NULL;
    }

    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    /* allocate memory for entire content */
    buffer = malloc(lSize * sizeof(char));
    if (!buffer) {
        fclose(fp);
        printf("memory alloc fails");
        return NULL;
    }

    /* copy the file into the buffer */
    if (1 != fread(buffer, lSize, 1, fp)) {
        fclose(fp);
        free(buffer);
        printf("entire read fails");
        return NULL;
    }

    buffer[lSize-1] = '\0';

    /* do your work here, buffer is a string contains the whole text */
    fclose(fp);
    return buffer;
}
