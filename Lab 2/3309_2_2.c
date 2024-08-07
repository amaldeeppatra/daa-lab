#include <stdio.h>
#include <stdlib.h>

void decimalToBinary(int num_309, FILE *outputFile) {
    if (num_309 > 1) {
        decimalToBinary(num_309 / 2, outputFile);
    }
    fprintf(outputFile, "%d", num_309 % 2);
}

int main() {
    const char*inputFileName = "input-1.txt";
    const char*outputFileName = "output-1.txt";
    FILE *inputFile_309 = fopen(inputFileName, "r");
    FILE *outputFile_309 = fopen(outputFileName, "w");

    if (inputFile_309 == NULL || outputFile_309 == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    int num_309;
    while (fscanf(inputFile_309, "%d", &num_309) != EOF) {
        decimalToBinary(num_309, outputFile_309);
        fprintf(outputFile_309, "\n");
    }
fclose(inputFile_309);
fclose(outputFile_309);

    printf("Conversion completed. Check the output file.\n");

    return 0;
}