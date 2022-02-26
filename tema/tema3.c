#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./bmp_header.h"

#define MAXBUFF 5001
#define MAXOFF 5000
typedef struct {
    bmp_fileheader header;
    bmp_infoheader info;
    char* imageMatrix;
}struct_bmp;

int main() {
    struct_bmp ind;
    ind.imageMatrix = NULL;
    char buffer[MAXBUFF];
    char zero[MAXOFF] = {0};
    char* name = NULL;
    char* var = NULL;
    while (fgets(buffer, MAXBUFF - 1, stdin)) {
        char* buf_aux = buffer;
        name = strtok_r(buffer, " \n", &buf_aux);
        if (strcmp(name, "edit") == 0) {
            var = strtok_r(NULL, " \n", &buf_aux);
            FILE* read = fopen(var, "rb");
            fread(&ind.header, sizeof(bmp_fileheader), 1, read);
            fread(&ind.info, sizeof(bmp_infoheader), 1, read);
            fseek(read, ind.header.imageDataOffset, SEEK_SET);
            // ne dam mai la dreapta ca sa ne putem poz la inceputul matricii^^^
            if (ind.imageMatrix == NULL) {
                ind.imageMatrix = malloc(sizeof(char) * ind.info.biSizeImage);
            }
            fread(ind.imageMatrix, sizeof(char), ind.info.biSizeImage, read);
            // citim cate 1 byte odata cat de mare e img^^^
            fclose(read);
        } else if (strcmp(name, "save") == 0) {
            unsigned int headerSize = 0, infoSize = 0;
            unsigned int actualOffeset = 0, ocupiedOffset = 0;
            var = strtok_r(NULL, " \n", &buf_aux);
            FILE* write = fopen(var, "wb");
            fwrite(&ind.header, sizeof(bmp_fileheader), 1, write);
            fwrite(&ind.info, sizeof(bmp_infoheader), 1, write);
            headerSize = sizeof(bmp_fileheader);
            infoSize = sizeof(bmp_infoheader);
            ocupiedOffset = headerSize + infoSize;
            actualOffeset = ind.header.imageDataOffset - ocupiedOffset;
            fwrite(zero, 1, actualOffeset, write);
            fwrite(ind.imageMatrix, sizeof(char), ind.info.biSizeImage, write);
            fclose(write);
        } else if (strcmp(name, "quit") == 0) {
            break;  // efectiv iesim din program
        } else {
            printf("nu am reusit\n");
        }
    }
    return 0;
}
