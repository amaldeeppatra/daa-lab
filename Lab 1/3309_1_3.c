#include <stdio.h>
#include <stdlib.h>

// Function to read the array from a file
void readArrayFromFile(int *arr, int n, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%d", &arr[i]);
    }

    fclose(file);
}

// Function to find the total number of duplicate elements
int countDuplicates(int *arr, int n) {
    int count = 0;
    int *visited = (int *)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        if (visited[i] == 1) continue;
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                if (visited[j] == 0) {
                    count++;
                    visited[j] = 1;
                }
            }
        }
    }

    free(visited);
    return count;
}

// Function to find the most repeating element in the array
int findMostRepeatingElement(int *arr, int n) {
    int maxCount = 0, mostRepeatingElement = arr[0];
    for (int i = 0; i < n; i++) {
        int count = 1;
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            mostRepeatingElement = arr[i];
        }
    }
    return mostRepeatingElement;
}

int main() {
    int n;
    const char *filename = "3309_1_3.txt";

    printf("Enter how many numbers you want to read from file: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Number of elements must be positive.\n");
        return EXIT_FAILURE;
    }

    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        perror("Error allocating memory");
        return EXIT_FAILURE;
    }

    readArrayFromFile(arr, n, filename);

    printf("The content of the array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int totalDuplicates = countDuplicates(arr, n);
    int mostRepeatingElement = findMostRepeatingElement(arr, n);

    printf("Total number of duplicate values = %d\n", totalDuplicates);
    printf("The most repeating element in the array = %d\n", mostRepeatingElement);

    free(arr);

    return 0;
}
