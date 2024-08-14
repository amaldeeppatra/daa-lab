#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function prototypes
void mergeSort(int arr[], int l, int r, int* comparisons);
void merge(int arr[], int l, int m, int r, int* comparisons);
void readData(const char* filename, int** data, int* size);
void writeData(const char* filename, int* data, int size);
void sortAndMeasure(const char* input_file, const char* output_file);

int main() {
    int option;

    while (1) {
        printf("MAIN MENU (MERGE SORT)\n");
        printf("1. Ascending Data\n");
        printf("2. Descending Data\n");
        printf("3. Random Data\n");
        printf("4. ERROR (EXIT)\n");
        printf("Enter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                sortAndMeasure("inAsc.dat", "outMergeAsc.dat");
                break;
            case 2:
                sortAndMeasure("inDesc.dat", "outMergeDesc.dat");
                break;
            case 3:
                sortAndMeasure("inRand.dat", "outMergeRand.dat");
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }

    return 0;
}

void mergeSort(int arr[], int l, int r, int* comparisons) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m, comparisons);
        mergeSort(arr, m + 1, r, comparisons);

        merge(arr, l, m, r, comparisons);
    }
}

void merge(int arr[], int l, int m, int r, int* comparisons) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        (*comparisons)++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void readData(const char* filename, int** data, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    int count = 0;
    int capacity = 10;
    int* tempData = (int*)malloc(capacity * sizeof(int));

    while (fscanf(file, "%d", &tempData[count]) != EOF) {
        count++;
        if (count >= capacity) {
            capacity *= 2;
            tempData = (int*)realloc(tempData, capacity * sizeof(int));
        }
    }

    fclose(file);

    *data = tempData;
    *size = count;
}

void writeData(const char* filename, int* data, int size) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", data[i]);
    }
    fprintf(file, "\n");

    fclose(file);
}

void sortAndMeasure(const char* input_file, const char* output_file) {
    int* data;
    int size;

    readData(input_file, &data, &size);
    printf("Before Sorting: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    int comparisons = 0;
    clock_t start_time = clock();
    mergeSort(data, 0, size - 1, &comparisons);
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1e9;

    writeData(output_file, data, size);
    printf("After Sorting: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    printf("Number of Comparisons: %d\n", comparisons);
    printf("Execution Time: %.0f nanoseconds\n", elapsed_time);

    free(data);
}
