#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 100
#define INFINITY 999999
#define MAX_V 1002

int n;
int escape[MAX_N][2];
int capacity[MAX_N][MAX_N];
int flow[MAX_N][MAX_N];

struct NetworkEdge {
    int to, capacity, flow, residual_idx;
};

struct NetworkEdge adj[MAX_V][MAX_V];
bool chk[MAX_V];
int group[MAX_V];
int src, sink;

void add_edge(int from, int to, int capacity) {
    adj[from][to].to = to;
    adj[from][to].capacity = capacity;
    adj[from][to].flow = 0;
    adj[from][to].residual_idx = to;

    // 역방향 간선 추가
    adj[to][from].to = from;
    adj[to][from].capacity = 0; // 역방향 간선의 용량은 0으로 설정
    adj[to][from].flow = 0;
    adj[to][from].residual_idx = from;
}

int find_path(int cur, int addible_flow) {
    if (cur == sink)
        return addible_flow;

    chk[cur] = true;

    for (int i = 0; i < MAX_V; i++) {
        struct NetworkEdge edge = adj[cur][i];
        if (chk[edge.to] || edge.capacity - edge.flow == 0)
            continue;

        int added = find_path(edge.to, (addible_flow < edge.capacity - edge.flow) ? addible_flow : edge.capacity - edge.flow);
        if (0 < added) {
            edge.flow += added;

            // 역방향 간선도 고려하여 업데이트
            adj[edge.to][edge.residual_idx].flow -= added;

            return added;
        }
    }

    // 경로가 없을 때 chk 배열 초기화
    chk[cur] = false;

    return 0;
}


int main() {
    printf("Enter the size of the grid (n): ");
    scanf("%d", &n);
    int v = n * n;

    int grid[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = 0;
        }
    }

    int i = 0;
    int answer = 'n';
    while (i < n * n) {
        printf("탈출 지점을 입력하시겠습니까? (y/n)");
        scanf(" %c", &answer);
        if (answer == 'y') {
            printf("Escape Point %d (x y): ", i + 1);
            scanf("%d %d", &escape[i][0], &escape[i][1]);
            grid[escape[i][0]][escape[i][1]] = 1;
            group[i + 1] = 0; // group 배열 초기화
            i++;
        } else {
            printf("입력을 종료하겠습니다. . .\n");
            break;
        }
    }

    // 추가: group 배열 초기화
    for (int i = 1; i <= v; i++) {
        if (group[i] == 0) {
            group[i] = 2;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }

    src = 0;
    sink = v + 1;

    for (int i = 0; i <= v + 1; i++) {
        for (int j = 0; j <= v + 1; j++) {
            adj[i][j].to = 0;
            adj[i][j].capacity = 0;
            adj[i][j].flow = 0;
            adj[i][j].residual_idx = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 1) {
                if (i > 0 && grid[i - 1][j] == 0) {
                    add_edge(i * n + j + 1, (i - 1) * n + j + 1, 1);
                    group[i * n + j + 1] = 1;
                    group[(i - 1) * n + j + 1] = 2;
                }
                if (i < n - 1 && grid[i + 1][j] == 0) {
                    add_edge(i * n + j + 1, (i + 1) * n + j + 1, 1);
                    group[i * n + j + 1] = 1;
                    group[(i + 1) * n + j + 1] = 2;
                }
                if (j > 0 && grid[i][j - 1] == 0) {
                    add_edge(i * n + j + 1, i * n + j, 1);
                    group[i * n + j + 1] = 1;
                    group[i * n + j] = 2;
                }
                if (j < n - 1 && grid[i][j + 1] == 0) {
                    add_edge(i * n + j + 1, i * n + j + 2, 1);
                    group[i * n + j + 1] = 1;
                    group[i * n + j + 2] = 2;
                }
            }
        }
    }

    for (int i = 1; i <= v; i++) {
        if (group[i] == 1) {
            add_edge(src, i, 1);
        } else if (group[i] == 2) {
            add_edge(i, sink, 1);
        }
    }

    int ans = 0, added_flow;
    while (0 < (added_flow = find_path(src, 101))) {
        ans += added_flow;
        for (int i = 0; i <= v + 1; i++) {
            chk[i] = false;
        }
    }

    printf("%d\n", ans);

    return 0;
}
