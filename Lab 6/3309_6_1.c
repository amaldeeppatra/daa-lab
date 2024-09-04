#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 100

// Function to find the vertex with the minimum key value
int findMinVertex(int key[], bool mstSet[], int V) {
    int min = INT_MAX, minIndex;

    for (int v = 0; v < V; v++)
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            minIndex = v;
        }

    return minIndex;
}

// Function to print the MST cost adjacency matrix and total weight
void printMST(int parent[], int graph[MAX][MAX], int V) {
    int mstCost[MAX][MAX] = {0};
    int totalCost = 0;

    printf("Cost Adjacency Matrix of the Minimum Spanning Tree:\n");

    for (int i = 1; i < V; i++) {
        int u = parent[i], v = i;
        int weight = graph[u][v];
        mstCost[u][v] = weight;
        mstCost[v][u] = weight;
        totalCost += weight;
    }

    // Print the cost adjacency matrix of MST
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%d ", mstCost[i][j]);
        }
        printf("\n");
    }

    printf("Total Weight of the Spanning Tree: %d\n", totalCost);
}

// Prim's algorithm to find the MST for a graph represented using adjacency matrix
void primMST(int graph[MAX][MAX], int V, int startVertex) {
    int parent[MAX];     // Array to store the constructed MST
    int key[MAX];        // Key values to pick the minimum weight edge in the cut
    bool mstSet[MAX];    // To represent set of vertices included in MST

    // Initialize all keys as INFINITE and mstSet[] as false
    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }

    // Start from the given starting vertex
    key[startVertex] = 0;
    parent[startVertex] = -1; // First node is always the root of MST

    // Construct the MST
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum key vertex not yet included in MST
        int u = findMinVertex(key, mstSet, V);
        mstSet[u] = true;

        // Update key value and parent index of the adjacent vertices of the picked vertex
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    printMST(parent, graph, V);
}

int main() {
    int V; // Number of vertices in the graph
    int startVertex;
    int graph[MAX][MAX];

    FILE *file = fopen("inUnAdjMat.dat", "r");

    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }

    printf("Enter the number of vertices: ");
    scanf("%d", &V);

    printf("Enter the starting vertex (1 to %d): ", V);
    scanf("%d", &startVertex);
    startVertex--; // Convert to 0-based index

    // Read the adjacency matrix from the file
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            fscanf(file, "%d", &graph[i][j]);
        }
    }

    fclose(file);

    // Run Prim's Algorithm
    primMST(graph, V, startVertex);

    return 0;
}
