#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function prototypes
void quickSort(int arr[], int low, int high, int* comparisons, int* best_case, int* worst_case);
int partition(int arr[], int low, int high, int* comparisons, int* best_case, int* worst_case);
void readData(const char* filename, int** data, int* size);
void writeData(const char* filename, int* data, int size);
void sortAndMeasure(const char* input_file, const char* output_file);

int main() {
    int option_309;

    while (1) {
        printf("MAIN MENU (QUICK SORT)\n");
        printf("1. Ascending Data\n");
        printf("2. Descending Data\n");
        printf("3. Random Data\n");
        printf("4. ERROR (EXIT)\n");
        printf("Enter option: ");
        scanf("%d", &option_309);

        switch (option_309) {
            case 1:
                sortAndMeasure("inAsc.dat", "outQuickAsc.dat");
                break;
            case 2:
                sortAndMeasure("inDesc.dat", "outQuickDesc.dat");
                break;
            case 3:
                sortAndMeasure("inRand.dat", "outQuickRand.dat");
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

void quickSort(int arr[], int low, int high, int* comparisons, int* best_case, int* worst_case) {
    if (low < high) {
        int pi = partition(arr, low, high, comparisons, best_case, worst_case);

        quickSort(arr, low, pi - 1, comparisons, best_case, worst_case);
        quickSort(arr, pi + 1, high, comparisons, best_case, worst_case);
    }
}

int partition(int arr[], int low, int high, int* comparisons, int* best_case, int* worst_case) {
    int pivot_309 = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        (*comparisons)++;
        if (arr[j] < pivot_309) {
            i++;
            int temp_309 = arr[i];
            arr[i] = arr[j];
            arr[j] = temp_309;
        }
    }
    int temp_309 = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp_309;

    int partition_size_left_309 = i - low + 1;
    int partition_size_right_309 = high - (i + 1);

    if (partition_size_left_309 == 0 || partition_size_right_309 == 0) {
        (*worst_case)++;
    }
    if (partition_size_left_309 <= (high - low) / 2 && partition_size_right_309 <= (high - low) / 2) {
        (*best_case)++;
    }

    return (i + 1);
}

void readData(const char* filename, int** data, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    int count_309 = 0;
    int capacity_309 = 10;
    int* tempData_309 = (int*)malloc(capacity_309 * sizeof(int));

    while (fscanf(file, "%d", &tempData_309[count_309]) != EOF) {
        count_309++;
        if (count_309 >= capacity_309) {
            capacity_309 *= 2;
            tempData_309 = (int*)realloc(tempData_309, capacity_309 * sizeof(int));
        }
    }

    fclose(file);

    *data = tempData_309;
    *size = count_309;
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
    int best_case = 0;
    int worst_case = 0;
    clock_t start_time = clock();
    quickSort(data, 0, size - 1, &comparisons, &best_case, &worst_case);
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

    if (worst_case > 0) {
        printf("Scenario: Worst-case partitioning\n");
    } else if (best_case > 0) {
        printf("Scenario: Best-case partitioning\n");
    } else {
        printf("Scenario: Neither best nor worst-case partitioning\n");
    }

    free(data);
}
