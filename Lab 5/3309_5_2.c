#include <stdio.h>
#include <stdlib.h>

// Define a structure for the symbol
struct SYMBOL {
    char alphabet;
    int frequency;
    struct SYMBOL *left, *right;
};

// Function to create a new symbol node
struct SYMBOL* createNode(char alphabet, int frequency) {
    struct SYMBOL* newNode = (struct SYMBOL*)malloc(sizeof(struct SYMBOL));
    newNode->alphabet = alphabet;
    newNode->frequency = frequency;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to swap two symbol nodes
void swapNodes(struct SYMBOL** a, struct SYMBOL** b) {
    struct SYMBOL* temp = *a;
    *a = *b;
    *b = temp;
}

// Min-Heapify function to maintain the min-heap property
void minHeapify(struct SYMBOL* heap[], int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left]->frequency < heap[smallest]->frequency)
        smallest = left;

    if (right < size && heap[right]->frequency < heap[smallest]->frequency)
        smallest = right;

    if (smallest != i) {
        swapNodes(&heap[i], &heap[smallest]);
        minHeapify(heap, size, smallest);
    }
}

// Function to extract the minimum node from the min-heap
struct SYMBOL* extractMin(struct SYMBOL* heap[], int* size) {
    struct SYMBOL* minNode = heap[0];
    heap[0] = heap[--(*size)];
    minHeapify(heap, *size, 0);
    return minNode;
}

// Function to insert a new node into the min-heap
void insertMinHeap(struct SYMBOL* heap[], int* size, struct SYMBOL* node) {
    heap[(*size)++] = node;
    int i = *size - 1;
    while (i && heap[i]->frequency < heap[(i - 1) / 2]->frequency) {
        swapNodes(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Function to build the Huffman tree
struct SYMBOL* buildHuffmanTree(char alphabets[], int frequencies[], int n) {
    struct SYMBOL* heap[n];
    int size = 0;

    for (int i = 0; i < n; i++) {
        heap[i] = createNode(alphabets[i], frequencies[i]);
        size++;
    }

    // Build the min-heap
    for (int i = (size - 2) / 2; i >= 0; i--)
        minHeapify(heap, size, i);

    // Build the Huffman tree
    while (size > 1) {
        struct SYMBOL* left = extractMin(heap, &size);
        struct SYMBOL* right = extractMin(heap, &size);

        // Create a new internal node with the sum of the frequencies
        struct SYMBOL* newNode = createNode('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        insertMinHeap(heap, &size, newNode);
    }

    // Return the root of the Huffman tree
    return extractMin(heap, &size);
}

// Function for in-order traversal of the Huffman tree
void inOrderTraversal(struct SYMBOL* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        if (root->alphabet != '$') // Ignore internal nodes marked with '$'
            printf("%c ", root->alphabet);
        inOrderTraversal(root->right);
    }
}

int main() {
    int n;
    printf("Enter the number of distinct alphabets: ");
    scanf("%d", &n);

    char alphabets[n];
    int frequencies[n];

    printf("Enter the alphabets: ");
    for (int i = 0; i < n; i++)
        scanf(" %c", &alphabets[i]);

    printf("Enter its frequencies: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &frequencies[i]);

    // Build the Huffman tree
    struct SYMBOL* root = buildHuffmanTree(alphabets, frequencies, n);

    printf("In-order traversal of the tree (Huffman): ");
    inOrderTraversal(root);

    return 0;
}
