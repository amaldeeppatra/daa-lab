#include <stdio.h>
#include <stdlib.h>

// Structure to represent an edge
struct Edge {
    int src, dest, weight;
};

// Structure to represent a subset for union-find
struct Subset {
    int parent;
    int rank;
};

// Function to find the subset of an element `i` using path compression
int find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = find(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

// Function to perform union of two subsets `x` and `y` using union by rank
void Union(struct Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // Attach smaller rank tree under root of higher rank tree
    if (subsets[xroot].rank < subsets[yroot].rank) {
        subsets[xroot].parent = yroot;
    } else if (subsets[xroot].rank > subsets[yroot].rank) {
        subsets[yroot].parent = xroot;
    } else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Comparison function to sort edges by weight
int compare(const void* a, const void* b) {
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    return a1->weight > b1->weight;
}

// Function to implement Kruskal's algorithm
void KruskalMST(struct Edge edges[], int V, int E) {
    struct Edge result[V];  // To store the resulting MST
    int e = 0;  // Index for result[]
    int i = 0;  // Index for sorted edges

    // Sort all the edges in non-decreasing order of their weight
    qsort(edges, E, sizeof(edges[0]), compare);

    // Allocate memory for creating V subsets
    struct Subset *subsets = (struct Subset*) malloc(V * sizeof(struct Subset));

    // Create V subsets with single elements
    for (int v = 0; v < V; v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Number of edges to be taken is equal to V-1
    while (e < V - 1 && i < E) {
        // Pick the smallest edge. And increment the index for next iteration
        struct Edge nextEdge = edges[i++];

        int x = find(subsets, nextEdge.src);
        int y = find(subsets, nextEdge.dest);

        // If including this edge doesn't cause a cycle, include it in the result
        if (x != y) {
            result[e++] = nextEdge;
            Union(subsets, x, y);
        }
    }

    // Print the contents of result[] to display the MST
    printf("Edge   Cost\n");
    int totalWeight = 0;
    for (i = 0; i < e; i++) {
        printf("%d--%d  %d\n", result[i].src + 1, result[i].dest + 1, result[i].weight);
        totalWeight += result[i].weight;
    }
    printf("Total Weight of the Spanning Tree: %d\n", totalWeight);

    free(subsets);
}

int main() {
    int V, E; // Number of vertices and edges

    // Input the number of vertices and edges
    printf("Enter the number of vertices and edges: ");
    scanf("%d %d", &V, &E);

    struct Edge edges[E]; // Array to store all edges

    printf("Enter each edge (u, v) and its weight w:\n");
    for (int i = 0; i < E; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        edges[i].src = u - 1;  // Convert to 0-based index
        edges[i].dest = v - 1; // Convert to 0-based index
        edges[i].weight = w;
    }

    // Apply Kruskal's algorithm
    KruskalMST(edges, V, E);

    return 0;
}
