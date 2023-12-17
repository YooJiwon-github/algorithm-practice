#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define SIZE 10


// ���� ����ü
typedef struct {
    int x;
    int y;
} Vertex;

// �׷��� ����ü
typedef struct {
    int V;           // ������ ����
    Vertex *vertices; // ���� �迭
    double **adjMatrix; // ���� ���
} Graph;

// Euclidean �Ÿ� ��� �Լ�
double euclideanDistance(Vertex a, Vertex b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
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
            graph->adjMatrix[i][j] = INFINITY; // �ʱ�ȭ: ���Ѵ�� ����
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

// MST ������ ���� Prim �˰��� �Լ�
void primMST(Graph *graph) {
    int *parent = (int *)malloc(graph->V * sizeof(int));
    double *key = (double *)malloc(graph->V * sizeof(double));
    int *inMST = (int *)malloc(graph->V * sizeof(int));
    
    for (int i = 0; i < graph->V; i++) {
        key[i] = INFINITY;
        inMST[i] = 0;
    }

    key[0] = 0.0;
    parent[0] = -1;

    for (int count = 0; count < graph->V - 1; count++) {
        int u = -1;
        for (int v = 0; v < graph->V; v++) {
            if (!inMST[v] && (u == -1 || key[v] < key[u]))
                u = v;
        }

        inMST[u] = 1;

        for (int v = 0; v < graph->V; v++) {
            if (!inMST[v] && graph->adjMatrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph->adjMatrix[u][v];
            }
        }
    }

    printf("Euclidean MST edges:\n");
    for (int i = 1; i < graph->V; i++) {
        printf("%d - %d\n", parent[i], i);
    }

    free(parent);
    free(key);
    free(inMST);
}

// ���� ã�� �Լ�
void findCenters(Graph *graph, int c) {
    printf("\nCenters:\n");
    printf("| Vertex | Closest Centers | Distances |\n");
    printf("|--------|------------------|-----------|\n");

    for (int i = 1; i < graph->V; i++) {
        // �迭�� ����Ͽ� ���� ����� ���͵��� ����
        int *closestCenters = (int *)malloc(c * sizeof(int));
        // �迭�� ����Ͽ� �� ���ͱ����� �Ÿ��� ����
        double *distances = (double *)malloc(c * sizeof(double));

        // �ʱ�ȭ
        for (int k = 0; k < c; k++) {
            closestCenters[k] = -1;
            distances[k] = INFINITY;
        }

        // �� �������� ��� ���ͱ����� �Ÿ� ���
        for (int j = 1; j < graph->V; j++) {
            for (int k = 0; k < c; k++) {
                if (graph->adjMatrix[i][j] < distances[k]) {
                    // ���� �������� j ���������� �Ÿ��� ����� �Ÿ����� ������ ������Ʈ
                    distances[k] = graph->adjMatrix[i][j];
                    closestCenters[k] = j;
                    // break;  // �� �� ������Ʈ�ϸ� �� �̻� ���� �ʿ䰡 ����
                }
            }
        }

        // ���
        printf("|   %2d   |", i);
        if(closestCenters[0] != -1) {
            printf("        %2d       |  %6.2lf  |", closestCenters[0], distances[0]);
        }
        else {
            printf("         N/A      |    N/A    |");
        }
        printf("\n");

        // �޸� ����
        free(closestCenters);
        free(distances);
    }
}

int main() {
    Graph *graph = initializeGraph(SIZE);
    
    // �׸��忡 �������� ���� ����
    for (int i = 0; i < SIZE; i++) {
        graph->vertices[i].x = rand() % SIZE;
        graph->vertices[i].y = rand() % SIZE;
    }

    // ��� ������ ������ ���� �߰�
    for (int i = 0; i < SIZE; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            addEdge(graph, i, j);
        }
    }

    primMST(graph);
    findCenters(graph, 3);  // ���� ������ 3���� ����

    // �׷��� �޸� ����
    for (int i = 0; i < SIZE; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph->vertices);
    free(graph);

    return 0;
}
