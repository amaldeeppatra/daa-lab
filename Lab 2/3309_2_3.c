#include <stdio.h>
#include <stdlib.h>

// Recursive function to find GCD
int gcd_309(int a_309, int b_309) {
    if (b_309 == 0)
        return a_309;
    return gcd_309(b_309, a_309 % b_309);
}

int main(int argc_309, char *argv_309[]) {
    if (argc_309 != 3) {
        printf("Usage: %s inGcd.dat outGcd.dat\n", argv_309[0]);
        return 1;
    }

    FILE *inputFile_309, *outputFile_309;
    inputFile_309 = fopen(argv_309[1], "r");
    outputFile_309 = fopen(argv_309[2], "w");

    if (inputFile_309 == NULL) {
        printf("Error: Cannot open input file %s\n", argv_309[1]);
        return 1;
    }

    if (outputFile_309 == NULL) {
        printf("Error: Cannot open output file %s\n", argv_309[2]);
        fclose(inputFile_309);
        return 1;
    }

    int num1_309, num2_309;
    int count_309 = 0;
    while (fscanf(inputFile_309, "%d %d", &num1_309, &num2_309) == 2) {
        count_309++;
        int result_309 = gcd_309(num1_309, num2_309);
        fprintf(outputFile_309, "The GCD of %d and %d is %d\n", num1_309, num2_309, result_309);
    }

    if (count_309 < 20) {
        printf("Error: The input file must contain at least 20 pairs of numbers\n");
        fclose(inputFile_309);
        fclose(outputFile_309);
        // Remove the incomplete output file
        remove(argv_309[2]);
        return 1;
    }

    fclose(inputFile_309);
    fclose(outputFile_309);

    printf("GCD results have been written to %s\n", argv_309[2]);
    return 0;
}