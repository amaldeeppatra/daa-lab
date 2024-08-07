#include <stdio.h>
#include <stdlib.h>

// Function to read the array from a file
void readArrayFromFile_309(int *arr_309, int n_309, const char *filename_309) {
    FILE *file_309 = fopen(filename_309, "r");
    if (file_309 == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n_309; i++) {
        fscanf(file_309, "%d", &arr_309[i]);
    }

    fclose(file_309);
}

// Function to find the second smallest and second largest elements
void findSecondSmallestAndLargest_309(int *arr_309, int n_309, int *secondSmallest_309, int *secondLargest_309) {
    int firstSmallest_309 = arr_309[0], firstLargest_309 = arr_309[0];
    *secondSmallest_309 = *secondLargest_309 = arr_309[0];

    for (int i = 1; i < n_309; i++) {
        if (arr_309[i] < firstSmallest_309) {
            *secondSmallest_309 = firstSmallest_309;
            firstSmallest_309 = arr_309[i];
        } else if (arr_309[i] > firstSmallest_309 && (arr_309[i] < *secondSmallest_309 || *secondSmallest_309 == firstSmallest_309)) {
            *secondSmallest_309 = arr_309[i];
        }

        if (arr_309[i] > firstLargest_309) {
            *secondLargest_309 = firstLargest_309;
            firstLargest_309 = arr_309[i];
        } else if (arr_309[i] < firstLargest_309 && (arr_309[i] > *secondLargest_309 || *secondLargest_309 == firstLargest_309)) {
            *secondLargest_309 = arr_309[i];
        }
    }
}

int main() {
    int n_309;
    const char *filename_309 = "3309_1_1.txt";

    printf("Enter the size of the array: ");
    scanf("%d", &n_309);

    if (n_309 < 2) {
        printf("Array size must be at least 2.\n");
        return EXIT_FAILURE;
    }

    int *arr_309 = (int *)malloc(n_309 * sizeof(int));
    if (arr_309 == NULL) {
        perror("Error allocating memory");
        return EXIT_FAILURE;
    }

    readArrayFromFile_309(arr_309, n_309, filename_309);

    int secondSmallest_309, secondLargest_309;
    findSecondSmallestAndLargest_309(arr_309, n_309, &secondSmallest_309, &secondLargest_309);

    printf("Second smallest element: %d\n", secondSmallest_309);
    printf("Second largest element: %d\n", secondLargest_309);

    free(arr_309);

    return 0;
}
