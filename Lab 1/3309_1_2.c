#include <stdio.h>
#include <stdlib.h>

// Function to calculate the prefix sum of the array
void calculatePrefixSum_309(int *arr_309, int *prefixSum_309, int n_309) {
    prefixSum_309[0] = arr_309[0];
    for (int i = 1; i < n_309; i++) {
        prefixSum_309[i] = prefixSum_309[i - 1] + arr_309[i];
    }
}

int main() {
    int n_309;

    printf("Enter the size of the array: ");
    scanf("%d", &n_309);

    if (n_309 <= 0) {
        printf("Array size must be positive.\n");
        return EXIT_FAILURE;
    }

    int *arr_309 = (int *)malloc(n_309 * sizeof(int));
    int *prefixSum_309 = (int *)malloc(n_309 * sizeof(int));

    if (arr_309 == NULL || prefixSum_309 == NULL) {
        perror("Error allocating memory");
        return EXIT_FAILURE;
    }

    printf("Enter the elements of the array: ");
    for (int i = 0; i < n_309; i++) {
        scanf("%d", &arr_309[i]);
    }

    calculatePrefixSum_309(arr_309, prefixSum_309, n_309);

    printf("Prefix sum array: ");
    for (int i = 0; i < n_309; i++) {
        printf("%d ", prefixSum_309[i]);
    }
    printf("\n");

    free(arr_309);
    free(prefixSum_309);

    return 0;
}
