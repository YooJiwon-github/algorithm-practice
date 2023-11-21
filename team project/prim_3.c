#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define SIZE 100
#define INF INT_MAX

// ���� ����ü
typedef struct {
    int x;
    int y;
} Vertex;

// �׷��� ����ü
typedef struct {
    int V;
    Vertex *vertices;
    double **adjMatrix;
} Graph;

// Euclidean �Ÿ� ��� �Լ�
double euclideanDistance(Vertex a, Vertex b) {
    return sqrt((a.x - b.x) * (a.x - b.y) + (a.y - b.y) * (a.y - b.y));
}

// �׷��� �ʱ�ȭ �Լ�
Graph *initializeGraph(int V) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->vertices = (Vertex *)malloc(V * sizeof(Vertex));
    graph->adjMatrix = (double **)malloc(V * sizeof(double *));
    
    for (int i = 0; i < V; i++) {
        graph->adjMatrix[i] = (double *)malloc(V * sizeof(double));
        for (int j = 0; j < V; j++) {
            graph->adjMatrix[i][j] = INF;  // �ʱ�ȭ: ���Ѵ�� ����
        }
    }
    
    return graph;
}

// �׷����� ���� �߰� �Լ�
void addEdge(Graph *graph, int src, int dest) {
    double distance = euclideanDistance(graph->vertices[src], graph->vertices[dest]);
    graph->adjMatrix[src][dest] = distance;
    graph->adjMatrix[dest][src] = distance;
}

// Prim�� �˰����� ����Ͽ� Euclidean MST ���ϱ�
void primMST(Graph *graph) {
    int *parent = (int *)malloc(graph->V * sizeof(int));
    double *key = (double *)malloc(graph->V * sizeof(double));
    int *inMST = (int *)malloc(graph->V * sizeof(int));
    
    for (int i = 0; i < graph->V; i++) {
        key[i] = INF;
        inMST[i] = 0;
    }
    
    key[0] = 0;
    parent[0] = -1;
    
    for (int count = 0; count < graph->V - 1; count++) {
        int u = -1;
        for (int v = 0; v < graph->V; v++) {
            if (!inMST[v] && (u == -1 || key[v] < key[u])) {
                u = v;
            }
        }
        
        inMST[u] = 1;
        
        for (int v = 0; v < graph->V; v++) {
            if (!inMST[v] && graph->adjMatrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph->adjMatrix[u][v];
            }
        }
    }

    // Euclidean MST ���
    printf("Euclidean MST:\n");
    for (int i = 1; i < graph->V; i++) {
        printf("Edge: %d - %d, Weight: %lf\n", parent[i], i, key[i]);
    }

    free(parent);
    free(key);
    free(inMST);
}

// ���� ã�� �Լ�
void findCenters(Graph *graph, int c) {
    printf("\nCenters:\n");
    printf("---------+-----------------+-----------\n");
    printf("| Vertex | Closest Center  | Distance |\n");
    printf("|--------+-----------------+----------|\n");
    for (int i = 0; i < graph->V; i++) {
        double minDistance = INF;
        int center = -1;
        for (int j = 0; j < graph->V; j++) {
            if (i != j && graph->adjMatrix[i][j] < minDistance) {
                minDistance = graph->adjMatrix[i][j];
                center = j;
            }
        }
        printf("|   %2d   |        %2d       |  %6.2lf  |\n", i, center, minDistance);
        printf("|--------+-----------------+----------|\n");
    }
}

int main() {
    Graph *graph = initializeGraph(SIZE);
    
    // �׸��忡 �������� ���� ����
    for (int i = 0; i < SIZE; i++) {
        graph->vertices[i].x = rand() % SIZE;
        graph->vertices[i].y = rand() % SIZE;
    }

    // ���� �߰�
    for (int i = 0; i < SIZE; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            addEdge(graph, i, j);
        }
    }

    // Prim �˰����� ����Ͽ� Euclidean MST ��� �� ���
    primMST(graph);

    // ���� ã�� �� ���
    int c = 5;  // ���� ���� ����
    findCenters(graph, c);

    free(graph->vertices);
    free(graph->adjMatrix);
    free(graph);

    return 0;
}
