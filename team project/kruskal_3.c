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

// ���� ����ü
typedef struct {
    int src;
    int dest;
    double weight;
} Edge;

// �׷��� ����ü
typedef struct {
    int V;
    Vertex *vertices;
    Edge *edges;
} Graph;

// Union-Find �ڷᱸ���� ���� �Լ���
int find(int parent[], int i);
void Union(int parent[], int x, int y);

// Euclidean �Ÿ� ��� �Լ�
double euclideanDistance(Vertex a, Vertex b) {
    return sqrt((a.x - b.x) * (a.x - b.y) + (a.y - b.y) * (a.y - b.y));
}

// �׷��� �ʱ�ȭ �Լ�
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

// Kruskal �˰����� ����Ͽ� Euclidean MST ���ϱ�
void kruskalMST(Graph *graph) {
    // ������ ����ġ�� ���� �������� ����
    for (int i = 0; i < graph->V * (graph->V - 1) / 2; i++) {
        for (int j = 0; j < graph->V * (graph->V - 1) / 2 - i - 1; j++) {
            if (graph->edges[j].weight > graph->edges[j + 1].weight) {
                Edge temp = graph->edges[j];
                graph->edges[j] = graph->edges[j + 1];
                graph->edges[j + 1] = temp;
            }
        }
    }

    // MST ������ ���� Union-Find �ڷᱸ�� �ʱ�ȭ
    int *parent = (int *)malloc(graph->V * sizeof(int));
    for (int i = 0; i < graph->V; i++) {
        parent[i] = i;
    }

    // Euclidean MST ���
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

// ���� ã�� �Լ�
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

// Union-Find �ڷᱸ�� �Լ� ����
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

    // �׸��忡 �������� ���� ����
    for (int i = 0; i < SIZE; i++) {
        graph->vertices[i].x = rand() % SIZE;
        graph->vertices[i].y = rand() % SIZE;
    }

    // Kruskal �˰����� ����Ͽ� Euclidean MST ��� �� ���
    kruskalMST(graph);

    // ���� ã�� �� ���
    int c = 5;  // ���� ���� ����
    findCenters(graph, c);

    free(graph->vertices);
    free(graph->edges);
    free(graph);

    return 0;
}


