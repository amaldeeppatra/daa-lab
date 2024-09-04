#include <stdio.h>
#include <stdlib.h>

// Structure to represent an item
struct ITEM {
    int item_id_309;
    double item_profit_309;
    double item_weight_309;
    double profit_weight_ratio_309;
};

// Function prototypes
void calculateProfitWeightRatio_309(struct ITEM items_309[], int n_309);
void heapify_309(struct ITEM arr_309[], int n_309, int i_309);
void heapSort_309(struct ITEM arr_309[], int n_309);
void fractionalKnapsack_309(struct ITEM items_309[], int n_309, double capacity_309);

int main() {
    int n_309;
    double capacity_309;

    printf("Enter the number of items: ");
    scanf("%d", &n_309);

    struct ITEM *items_309 = (struct ITEM *)malloc(n_309 * sizeof(struct ITEM));

    for (int i = 0; i < n_309; i++) {  // `i` is the loop variable, so no `_309` suffix
        printf("Enter the profit and weight of item no %d: ", i + 1);
        scanf("%lf %lf", &items_309[i].item_profit_309, &items_309[i].item_weight_309);
        items_309[i].item_id_309 = i + 1;
    }

    printf("Enter the capacity of knapsack: ");
    scanf("%lf", &capacity_309);

    // Calculate profit/weight ratio
    calculateProfitWeightRatio_309(items_309, n_309);

    // Sort items in non-increasing order of profit/weight ratio using heap sort
    heapSort_309(items_309, n_309);

    // Perform the fractional knapsack algorithm
    fractionalKnapsack_309(items_309, n_309, capacity_309);

    free(items_309);
    return 0;
}

// Function to calculate profit/weight ratio
void calculateProfitWeightRatio_309(struct ITEM items_309[], int n_309) {
    for (int i = 0; i < n_309; i++) {  // `i` is the loop variable, so no `_309` suffix
        items_309[i].profit_weight_ratio_309 = items_309[i].item_profit_309 / items_309[i].item_weight_309;
    }
}

// Heapify function for heap sort
void heapify_309(struct ITEM arr_309[], int n_309, int i_309) {
    int largest_309 = i_309;
    int left_309 = 2 * i_309 + 1;
    int right_309 = 2 * i_309 + 2;

    if (left_309 < n_309 && arr_309[left_309].profit_weight_ratio_309 > arr_309[largest_309].profit_weight_ratio_309)
        largest_309 = left_309;

    if (right_309 < n_309 && arr_309[right_309].profit_weight_ratio_309 > arr_309[largest_309].profit_weight_ratio_309)
        largest_309 = right_309;

    if (largest_309 != i_309) {
        struct ITEM temp_309 = arr_309[i_309];
        arr_309[i_309] = arr_309[largest_309];
        arr_309[largest_309] = temp_309;

        heapify_309(arr_309, n_309, largest_309);
    }
}

// Heap sort function
void heapSort_309(struct ITEM arr_309[], int n_309) {
    for (int i = n_309 / 2 - 1; i >= 0; i--)  // `i` is the loop variable, so no `_309` suffix
        heapify_309(arr_309, n_309, i);

    for (int i = n_309 - 1; i > 0; i--) {  // `i` is the loop variable, so no `_309` suffix
        struct ITEM temp_309 = arr_309[0];
        arr_309[0] = arr_309[i];
        arr_309[i] = temp_309;

        heapify_309(arr_309, i, 0);
    }
}

// Function to perform the Fractional Knapsack algorithm
void fractionalKnapsack_309(struct ITEM items_309[], int n_309, double capacity_309) {
    double total_profit_309 = 0.0;

    printf("Item No\tprofit\tWeight\tAmount to be taken\n");

    for (int i = 0; i < n_309; i++) {  // `i` is the loop variable, so no `_309` suffix
        if (items_309[i].item_weight_309 <= capacity_309) {
            // Take the whole item
            capacity_309 -= items_309[i].item_weight_309;
            total_profit_309 += items_309[i].item_profit_309;
            printf("%d\t%.6lf\t%.6lf\t1.000000\n", items_309[i].item_id_309, items_309[i].item_profit_309, items_309[i].item_weight_309);
        } else {
            // Take the fraction of the remaining capacity
            double fraction_309 = capacity_309 / items_309[i].item_weight_309;
            total_profit_309 += items_309[i].item_profit_309 * fraction_309;
            printf("%d\t%.6lf\t%.6lf\t%.6lf\n", items_309[i].item_id_309, items_309[i].item_profit_309, items_309[i].item_weight_309, fraction_309);
            break;
        }
    }

    printf("Maximum profit: %.6lf\n", total_profit_309);
}