#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define N 7
#define ARRAY_LENGTH 8

// weight, cost, root 초기화 함수
void wc_init(double weight[ARRAY_LENGTH][ARRAY_LENGTH], double cost[ARRAY_LENGTH][ARRAY_LENGTH], double q[N + 1]);
// weight table 출력 함수
void print_weight(double weight[ARRAY_LENGTH][ARRAY_LENGTH]);
// cost table 출력 함수
void print_cost(double cost[ARRAY_LENGTH][ARRAY_LENGTH]);
// root table 출력 함수
void print_root(int root[ARRAY_LENGTH][ARRAY_LENGTH]);

int main(void) {
    double weight[ARRAY_LENGTH][ARRAY_LENGTH] = {0};
    double cost[ARRAY_LENGTH][ARRAY_LENGTH] = {0};
    int root[ARRAY_LENGTH][ARRAY_LENGTH] = {0};
    double p[N + 1] = {0, 0.03, 0.04, 0.06, 0.05, 0.05, 0.09, 0.1};
    double q[N + 1] = {0.07, 0.08, 0.07, 0.05, 0.06, 0.07, 0.12, 0.06};

    wc_init(weight, cost, q);

    for(int x = 1; x <= N; x++) {
        for(int i = 0; i <= (N-x); i++) {
            int j = i + x;
            weight[i][j] = weight[i][j-1] + p[j] + q[j];
            for(int k = i + 1; k <= j; k++) {
                if(k == (i + 1)) {
                    cost[i][j] = cost[i][k-1] + cost[k][j] + weight[i][j];
                    root[i][j] = k;
                }
                else {
                    if(cost[i][j] >= cost[i][k-1] + cost[k][j] + weight[i][j]) {
                        cost[i][j] = cost[i][k-1] + cost[k][j] + weight[i][j];
                        root[i][j] = k;
                    }
                }
            }
        }
    }

    print_weight(weight);
    print_cost(cost);
    print_root(root);

    return 0;
}

void wc_init(double weight[ARRAY_LENGTH][ARRAY_LENGTH], double cost[ARRAY_LENGTH][ARRAY_LENGTH], double q[N + 1]) {
    for (int i = 0; i <= N; i++) {
        weight[i][i] = q[i];
        cost[i][i] = 0.0;
    }
}

void print_weight(double weight[ARRAY_LENGTH][ARRAY_LENGTH]) {
    fputs("Weight table\n", stdout);
    printf("|------+------+------+------+------+------+------+------|\n");
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        for(int k = 0; k < i; k++) {
            printf("|      ");
        }
        for (int j = 0; j < ARRAY_LENGTH - i; j++) {
            printf("|%6.2lf", weight[i][j + i]);
        }
        printf("|\n");
		printf("|------+------+------+------+------+------+------+------|\n");
    }
}

void print_cost(double cost[ARRAY_LENGTH][ARRAY_LENGTH]) {
    fputs("\nCost Table\n", stdout);
    printf("|------+------+------+------+------+------+------+------|\n");
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        for(int k = 0; k < i; k++) {
            printf("|      ");
        }
        for (int j = 0; j < ARRAY_LENGTH - i; j++) {
            printf("|%6.2lf", cost[i][j + i]);
        }
        printf("|\n");
		printf("|------+------+------+------+------+------+------+------|\n");
    }
}

void print_root(int root[ARRAY_LENGTH][ARRAY_LENGTH]) {
    fputs("\nRoot Table\n", stdout);
    printf("|------+------+------+------+------+------+------+------|\n");
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        for(int k = 0; k < i; k++) {
            printf("|      ");
        }
        for (int j = 0; j < ARRAY_LENGTH - i; j++) {
            printf("|%6d", root[i][j + i]);
        }
        printf("|\n");
		printf("|------+------+------+------+------+------+------+------|\n");
    }
}
