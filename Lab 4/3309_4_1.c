#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person {
    int id;
    char name[100];
    int age;
    int height;
    int weight;
};

// Function prototypes
void readData(struct person **people, int *n, const char *filename);
void minHeapify(struct person heap[], int n, int i);
void maxHeapify(struct person heap[], int n, int i);
void createMinHeap(struct person heap[], int n);
void createMaxHeap(struct person heap[], int n);
void displayWeightOfYoungest(struct person heap[], int n);
void insertIntoMinHeap(struct person **heap, int *n, struct person newPerson);
void deleteOldestPerson(struct person *heap, int *n);

int main() {
    int option;
    struct person *people = NULL;
    int n = 0;
    struct person *minHeap = NULL;
    int minHeapSize = 0;
    struct person *maxHeap = NULL;
    int maxHeapSize = 0;
    
    while (1) {
        printf("MAIN MENU (HEAP)\n");
        printf("1. Read Data\n");
        printf("2. Create a Min-heap based on the age\n");
        printf("3. Create a Max-heap based on the weight\n");
        printf("4. Display weight of the youngest person\n");
        printf("5. Insert a new person into the Min-heap\n");
        printf("6. Delete the oldest person\n");
        printf("7. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                readData(&people, &n, "students.txt");
                printf("Data read successfully.\n");
                break;
            case 2:
                minHeap = (struct person *)malloc(n * sizeof(struct person));
                memcpy(minHeap, people, n * sizeof(struct person));
                minHeapSize = n;
                createMinHeap(minHeap, minHeapSize);
                printf("Min-heap created based on age.\n");
                break;
            case 3:
                maxHeap = (struct person *)malloc(n * sizeof(struct person));
                memcpy(maxHeap, people, n * sizeof(struct person));
                maxHeapSize = n;
                createMaxHeap(maxHeap, maxHeapSize);
                printf("Max-heap created based on weight.\n");
                break;
            case 4:
                if (minHeapSize > 0)
                    displayWeightOfYoungest(minHeap, minHeapSize);
                else
                    printf("Min-heap is empty. Please create it first.\n");
                break;
            case 5: {
                struct person newPerson;
                printf("Enter Id, Name, Age, Height, Weight (separated by spaces):\n");
                scanf("%d %s %d %d %d", &newPerson.id, newPerson.name, &newPerson.age, &newPerson.height, &newPerson.weight);
                insertIntoMinHeap(&minHeap, &minHeapSize, newPerson);
                printf("Person inserted into Min-heap.\n");
                break;
            }
            case 6:
                if (minHeapSize > 0) {
                    deleteOldestPerson(minHeap, &minHeapSize);
                    printf("Oldest person deleted from Min-heap.\n");
                } else {
                    printf("Min-heap is empty. Please create it first.\n");
                }
                break;
            case 7:
                free(people);
                free(minHeap);
                free(maxHeap);
                return 0;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}

void readData(struct person **people, int *n, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", n);
    *people = (struct person *)malloc(*n * sizeof(struct person));

    for (int i = 0; i < *n; i++) {
        fscanf(file, "%d %s %d %d %d", &(*people)[i].id, (*people)[i].name, &(*people)[i].age, &(*people)[i].height, &(*people)[i].weight);
    }

    fclose(file);
}

void minHeapify(struct person heap[], int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && heap[left].age < heap[smallest].age)
        smallest = left;
    if (right < n && heap[right].age < heap[smallest].age)
        smallest = right;
    if (smallest != i) {
        struct person temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;
        minHeapify(heap, n, smallest);
    }
}

void maxHeapify(struct person heap[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && heap[left].weight > heap[largest].weight)
        largest = left;
    if (right < n && heap[right].weight > heap[largest].weight)
        largest = right;
    if (largest != i) {
        struct person temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;
        maxHeapify(heap, n, largest);
    }
}

void createMinHeap(struct person heap[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        minHeapify(heap, n, i);
}

void createMaxHeap(struct person heap[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        maxHeapify(heap, n, i);
}

void displayWeightOfYoungest(struct person heap[], int n) {
    printf("Weight of youngest person: %.2f kg\n", heap[0].weight * 0.453592); // converting pounds to kg
}

void insertIntoMinHeap(struct person **heap, int *n, struct person newPerson) {
    *n += 1;
    *heap = (struct person *)realloc(*heap, *n * sizeof(struct person));
    int i = *n - 1;
    (*heap)[i] = newPerson;

    while (i != 0 && (*heap)[(i - 1) / 2].age > (*heap)[i].age) {
        struct person temp = (*heap)[i];
        (*heap)[i] = (*heap)[(i - 1) / 2];
        (*heap)[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

void deleteOldestPerson(struct person *heap, int *n) {
    if (*n <= 0) return;

    heap[0] = heap[*n - 1];
    *n -= 1;
    minHeapify(heap, *n, 0);
}
