#include <stdio.h>
#include <stdlib.h>
int gcd(int a_309, int b_309) {
    if (a_309 == 0) {
        return b_309;
    }
    if (b_309 == 0) {
        return a_309;
    }
    if (a_309 > b_309) {
        return gcd(a_309 % b_309, b_309);
    } else {
        return gcd(b_309 % a_309, a_309);
    }
}

int main() {
    const char *inputFileName = "input.txt";
    const char *outputFileName = "output.txt";

    FILE *inputFile_309 = fopen(inputFileName, "r");
    FILE *outputFile_309 = fopen(outputFileName, "w");

    if (inputFile_309 == NULL || outputFile_309 == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    int num1_309, num2_309;
    if (fscanf(inputFile_309, "%d %d", &num1_309, &num2_309) != 2) {
        printf("Error reading numbers from file!\n");
        fclose(inputFile_309);
        fclose(outputFile_309);
        exit(1);
    }
int divisor_309 = gcd(num1_309, num2_309);
fprintf(outputFile_309, "The gcd of %d and %d is %d\n", num1_309, num2_309, divisor_309);
fclose(inputFile_309);
fclose(outputFile_309);
printf("GCD calculation completed\n");

    return 0;
}