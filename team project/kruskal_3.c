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

// 간선 구조체
typedef struct {
    int src;
    int dest;
    double weight;
} Edge;

// 그래프 구조체
typedef struct {
    int V;
    Vertex *vertices;
    Edge *edges;
} Graph;

// Union-Find 자료구조를 위한 함수들
int find(int parent[], int i);
void Union(int parent[], int x, int y);

// Euclidean 거리 계산 함수
double euclideanDistance(Vertex a, Vertex b) {
    return sqrt((a.x - b.x) * (a.x - b.y) + (a.y - b.y) * (a.y - b.y));
}

// 그래프 초기화 함수
Graph *initializeGraph(int V) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->vertices = (Vertex *)malloc(V * sizeof(Vertex));
    graph->edges = (Edge *)malloc(V * (V - 1) / 2 * sizeof(Edge));
    
    int edgeIndex = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            graph->edges[edgeIndex].src = i;
            graph->edges[edgeIndex].dest = j;
            graph->edges[edgeIndex].weight = euclideanDistance(graph->vertices[i], graph->vertices[j]);
            edgeIndex++;
        }
    }
    
    return graph;
}

// Kruskal 알고리즘을 사용하여 Euclidean MST 구하기
void kruskalMST(Graph *graph) {
    // 간선을 가중치에 따라 오름차순 정렬
    for (int i = 0; i < graph->V * (graph->V - 1) / 2; i++) {
        for (int j = 0; j < graph->V * (graph->V - 1) / 2 - i - 1; j++) {
            if (graph->edges[j].weight > graph->edges[j + 1].weight) {
                Edge temp = graph->edges[j];
                graph->edges[j] = graph->edges[j + 1];
                graph->edges[j + 1] = temp;
            }
        }
    }

    // MST 구성을 위한 Union-Find 자료구조 초기화
    int *parent = (int *)malloc(graph->V * sizeof(int));
    for (int i = 0; i < graph->V; i++) {
        parent[i] = i;
    }

    // Euclidean MST 출력
    printf("Euclidean MST:\n");
    for (int i = 0; i < graph->V * (graph->V - 1) / 2; i++) {
        int srcParent = find(parent, graph->edges[i].src);
        int destParent = find(parent, graph->edges[i].dest);

        if (srcParent != destParent) {
            printf("Edge: %d - %d, Weight: %lf\n", graph->edges[i].src, graph->edges[i].dest, graph->edges[i].weight);
            Union(parent, srcParent, destParent);
        }
    }

    free(parent);
}

// 센터 찾기 함수
void findCenters(Graph *graph, int c) {
    printf("\nCenters:\n");
    for (int i = 0; i < graph->V; i++) {
        double minDistance = INF;
        int center = -1;
        for (int j = 0; j < graph->V; j++) {
            if (i != j && euclideanDistance(graph->vertices[i], graph->vertices[j]) < minDistance) {
                minDistance = euclideanDistance(graph->vertices[i], graph->vertices[j]);
                center = j;
            }
        }
        printf("Vertex %d: Center %d, Distance: %lf\n", i, center, minDistance);
    }
}

// Union-Find 자료구조 함수 구현
int find(int parent[], int i) {
    if (parent[i] == i) {
        return i;
    }
    return find(parent, parent[i]);
}

void Union(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    parent[xset] = yset;
}

int main() {
    Graph *graph = initializeGraph(SIZE);

    // 그리드에 무작위로 정점 생성
    for (int i = 0; i < SIZE; i++) {
        graph->vertices[i].x = rand() % SIZE;
        graph->vertices[i].y = rand() % SIZE;
    }

    // Kruskal 알고리즘을 사용하여 Euclidean MST 계산 및 출력
    kruskalMST(graph);

    // 센터 찾기 및 출력
    int c = 5;  // 센터 개수 설정
    findCenters(graph, c);

    free(graph->vertices);
    free(graph->edges);
    free(graph);

    return 0;
}


