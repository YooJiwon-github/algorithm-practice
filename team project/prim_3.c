#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define SIZE 10


// 정점 구조체
typedef struct {
    int x;
    int y;
} Vertex;

// 그래프 구조체
typedef struct {
    int V;           // 정점의 개수
    Vertex *vertices; // 정점 배열
    double **adjMatrix; // 인접 행렬
} Graph;

// Euclidean 거리 계산 함수
double euclideanDistance(Vertex a, Vertex b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
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
            graph->adjMatrix[i][j] = INFINITY; // 초기화: 무한대로 설정
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

// MST 생성을 위한 Prim 알고리즘 함수
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

// 센터 찾기 함수
void findCenters(Graph *graph, int c) {
    printf("\nCenters:\n");
    printf("| Vertex | Closest Centers | Distances |\n");
    printf("|--------|------------------|-----------|\n");

    for (int i = 1; i < graph->V; i++) {
        // 배열을 사용하여 가장 가까운 센터들을 저장
        int *closestCenters = (int *)malloc(c * sizeof(int));
        // 배열을 사용하여 각 센터까지의 거리를 저장
        double *distances = (double *)malloc(c * sizeof(double));

        // 초기화
        for (int k = 0; k < c; k++) {
            closestCenters[k] = -1;
            distances[k] = INFINITY;
        }

        // 각 정점에서 모든 센터까지의 거리 계산
        for (int j = 1; j < graph->V; j++) {
            for (int k = 0; k < c; k++) {
                if (graph->adjMatrix[i][j] < distances[k]) {
                    // 현재 정점에서 j 정점까지의 거리가 저장된 거리보다 작으면 업데이트
                    distances[k] = graph->adjMatrix[i][j];
                    closestCenters[k] = j;
                    // break;  // 한 번 업데이트하면 더 이상 비교할 필요가 없음
                }
            }
        }

        // 출력
        printf("|   %2d   |", i);
        if(closestCenters[0] != -1) {
            printf("        %2d       |  %6.2lf  |", closestCenters[0], distances[0]);
        }
        else {
            printf("         N/A      |    N/A    |");
        }
        printf("\n");

        // 메모리 해제
        free(closestCenters);
        free(distances);
    }
}

int main() {
    Graph *graph = initializeGraph(SIZE);
    
    // 그리드에 무작위로 정점 생성
    for (int i = 0; i < SIZE; i++) {
        graph->vertices[i].x = rand() % SIZE;
        graph->vertices[i].y = rand() % SIZE;
    }

    // 모든 정점들 사이의 간선 추가
    for (int i = 0; i < SIZE; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            addEdge(graph, i, j);
        }
    }

    primMST(graph);
    findCenters(graph, 3);  // 센터 개수를 3으로 설정

    // 그래프 메모리 해제
    for (int i = 0; i < SIZE; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph->vertices);
    free(graph);

    return 0;
}
