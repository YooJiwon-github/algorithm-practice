#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define SIZE 100
#define INF INT_MAX

// 정점 구조체
typedef struct {
    int x;
    int y;
} Vertex;

// 그래프 구조체
typedef struct {
    int V;
    Vertex *vertices;
    double **adjMatrix;
} Graph;

// Euclidean 거리 계산 함수
double euclideanDistance(Vertex a, Vertex b) {
    return sqrt((a.x - b.x) * (a.x - b.y) + (a.y - b.y) * (a.y - b.y));
}

// 그래프 초기화 함수
Graph *initializeGraph(int V) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->vertices = (Vertex *)malloc(V * sizeof(Vertex));
    graph->adjMatrix = (double **)malloc(V * sizeof(double *));
    
    for (int i = 0; i < V; i++) {
        graph->adjMatrix[i] = (double *)malloc(V * sizeof(double));
        for (int j = 0; j < V; j++) {
            graph->adjMatrix[i][j] = INF;  // 초기화: 무한대로 설정
        }
    }
    
    return graph;
}

// 그래프에 간선 추가 함수
void addEdge(Graph *graph, int src, int dest) {
    double distance = euclideanDistance(graph->vertices[src], graph->vertices[dest]);
    graph->adjMatrix[src][dest] = distance;
    graph->adjMatrix[dest][src] = distance;
}

// Prim의 알고리즘을 사용하여 Euclidean MST 구하기
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

    // Euclidean MST 출력
    printf("Euclidean MST:\n");
    for (int i = 1; i < graph->V; i++) {
        printf("Edge: %d - %d, Weight: %lf\n", parent[i], i, key[i]);
    }

    free(parent);
    free(key);
    free(inMST);
}

// 센터 찾기 함수
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
    
    // 그리드에 무작위로 정점 생성
    for (int i = 0; i < SIZE; i++) {
        graph->vertices[i].x = rand() % SIZE;
        graph->vertices[i].y = rand() % SIZE;
    }

    // 간선 추가
    for (int i = 0; i < SIZE; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            addEdge(graph, i, j);
        }
    }

    // Prim 알고리즘을 사용하여 Euclidean MST 계산 및 출력
    primMST(graph);

    // 센터 찾기 및 출력
    int c = 5;  // 센터 개수 설정
    findCenters(graph, c);

    free(graph->vertices);
    free(graph->adjMatrix);
    free(graph);

    return 0;
}
