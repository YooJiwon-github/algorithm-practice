#include <stdio.h>
#include <stdlib.h>

#define MAX_GRID_SIZE 100

int path(int *arr, int s, int t, int *result, int *result_size) {
    int source_vertex = s;
    int temp[MAX_GRID_SIZE];
    int i = t, count = 0;

    while (arr[i] != -9999) {
        temp[count++] = i;
        i = arr[i];
    }

    // Copy the result in reverse order
    *result_size = count;
    for (i = 0; i < count; ++i) {
        result[i] = temp[count - 1 - i];
    }

    return 1;
}

int find_bottleneck(int **G, int *arr, int size) {
    int min_val = __INT_MAX__;
    for (int i = 1; i < size; ++i) {
        int start = arr[i - 1];
        int end = arr[i];
        if (G[start][end] < min_val) {
            min_val = G[start][end];
        }
    }
    return min_val;
}

int augment(int **G1, int *short_path, int bottleneck_edge) {
    for (int i = 1; i < short_path[0]; ++i) {
        int start = short_path[i - 1];
        int end = short_path[i];
        G1[start][end] -= bottleneck_edge;
        G1[end][start] += bottleneck_edge;
    }
    return 1;
}

int edmonds_karp(int **G, int size, int s, int t) {
    int flow = 0;
    int source = s;
    int sink = t;

    int *nodes = (int *)malloc(size * sizeof(int));
    int *predecessor = (int *)malloc(size * sizeof(int));

    int result[MAX_GRID_SIZE];
    int result_size;

    // Initialization
    for (int i = 0; i < size; ++i) {
        nodes[i] = -1;
        predecessor[i] = -1;
    }

    int *shortest_path;

    while (1) {
        int current = source;
        int count = 0;

        nodes[count++] = current;

        // Breadth-first search
        while (nodes[0] != -1) {
            current = nodes[0];
            nodes[0] = -1;

            for (int i = 0; i < size; ++i) {
                if (G[current][i] > 0 && predecessor[i] == -1) {
                    nodes[count++] = i;
                    predecessor[i] = current;
                }
            }
        }

        // Check if we reached the sink
        if (predecessor[sink] == -1) {
            free(nodes);
            free(predecessor);
            break;
        }

        // Find bottleneck edge
        path(predecessor, source, sink, result, &result_size);
        int bottleneck_edge = find_bottleneck(G, result, result_size);

        // Augment the graph
        augment(G, result, bottleneck_edge);

        // Update flow
        flow += bottleneck_edge;

        // Clear predecessor array
        for (int i = 0; i < size; ++i) {
            predecessor[i] = -1;
        }
    }

    return flow;
}

int escape(int d, int st_size, int *st) {
    int g = d * d * 2 + 2;

    int **x1 = (int **)malloc(g * sizeof(int *));
    for (int i = 0; i < g; ++i) {
        x1[i] = (int *)malloc(g * sizeof(int));
    }

    // Initialize x1 matrix
    for (int i = 0; i < g; ++i) {
        for (int j = 0; j < g; ++j) {
            x1[i][j] = 0;
        }
    }

    // Vin-Vout edges changed to 1
    for (int j = 1; j < d * d * 2; j += 2) {
        x1[j][j + 1] = 1;
    }

    // Sides to Sink
    // Upper + Lower
    for (int j = 1; j <= d; ++j) {
        x1[2 * j][g - 1] = 1;
        x1[2 * (d - 1) * d + 2 * j][g - 1] = 1;
    }

    // Sides to Sink
    // left + right
    for (int j = 0; j < d; ++j) {
        x1[j * 2 * d + 2][g - 1] = 1;
        x1[2 * d * (j + 1)][g - 1] = 1;
    }

    // Source to given vertices,
    // S to Vin
    for (int l = 0; l < st_size; ++l) {
        x1[0][(st[l * 2] - 1) * 2 * d + (2 * (st[l * 2 + 1] - 1)) + 1] = 1;
    }

    // From each vertex to its neighbors
    // Right
    for (int l = 2; l < d * d * 2 + 1; l += 2) {
        if (l % (2 * d) != 0) {
            x1[l][l + 1] = 1;
        }
    }

    // Left
    for (int l = 2; l < d * d * 2 + 1; l += 2) {
        if ((l - 2) % (2 * d) != 0) {
            x1[l][l - 3] = 1;
        }
    }

    // Up
    for (int l = 2; l < d * d * 2 + 1; l += 2) {
        if (l - (2 * d) > 0) {
            x1[l][l - (2 * d) - 1] = 1;
        }
    }

    // Down
    for (int l = 2; l < d * d * 2 + 1; l += 2) {
        if (l + (2 * d) < d * d * 2 + 1) {
            x1[l][l + (2 * d) - 1] = 1;
        }
    }

    int max_flow = edmonds_karp(x1, g, 0, g - 1);

    // Free dynamically allocated memory
    for (int i = 0; i < g; ++i) {
        free(x1[i]);
    }
    free(x1);

    return max_flow;
}

int main() {
    int grid_dimension;

    // Get grid size from user
    printf("Enter the grid size (n): ");
    scanf("%d", &grid_dimension);

    int num_start_vertices;
    // Get the number of starting escape vertices
    printf("Enter the number of escape points: ");
    scanf("%d", &num_start_vertices);

    int start_vertices[num_start_vertices * 2];
    // Get starting escape vertices from user
    for (int i = 0; i < num_start_vertices; ++i) {
        printf("Enter escape point coordinates (x y): ");
        scanf("%d %d", &start_vertices[i * 2], &start_vertices[i * 2 + 1]);
    }

    // Escape function execution
    int final_flow = escape(grid_dimension, num_start_vertices, start_vertices);

    // Print the result
    printf("Maximum Flow: %d\n", final_flow);
    printf("Starting Escape vertices: %d\n", num_start_vertices);
    printf("%s\n", final_flow == num_start_vertices ? "Possible" : "Not Possible");

    return 0;
}
