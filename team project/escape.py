import numpy as np
from scipy.sparse import csr_matrix
from scipy.sparse.csgraph import breadth_first_order

def path(arr, s, t):
    source_vertex = s
    temp = [t]
    i = t
    while arr[i] != -9999:
        temp.append(arr[i])
        i = arr[i]
    return temp[::-1]

def find_bottleneck(G, arr):
    min_val = np.inf
    for i in range(1, len(arr)):
        start, end = arr[i - 1], arr[i]
        if G[start][end] < min_val:
            min_val = G[start][end]
    return min_val

def augment(G1, short_path, bottleneck_edge):
    for i in range(1, len(short_path)):
        start, end = short_path[i - 1], short_path[i]
        G1[start][end] -= bottleneck_edge
        G1[end][start] += bottleneck_edge
    return G1

def edmonds_karp(G, s, t):
    flow = 0
    source, sink = s, t

    nodes, predecessor = breadth_first_order(csr_matrix(G), 0, directed=True, return_predecessors=True)
    shortest_path = path(predecessor, source, sink)

    while source in shortest_path:
        bottleneck_edge = find_bottleneck(G, shortest_path)
        flow += bottleneck_edge
        G = augment(G, shortest_path, bottleneck_edge)
        nodes, predecessor = breadth_first_order(csr_matrix(G), 0, directed=True, return_predecessors=True)
        shortest_path = path(predecessor, source, sink)

    return flow

def escape(d, st):
    g = d * d * 2 + 2
    x1 = np.zeros((g, g), dtype=np.int)

    for j in range(1, d * d * 2, 2):
        x1[j][j + 1] = 1

    for j in range(1, d + 1):
        x1[2 * j][g - 1] = 1
        x1[2 * (d - 1) * d + 2 * j][g - 1] = 1

    for j in range(d):
        x1[j * 2 * d + 2][g - 1] = 1
        x1[2 * d * (j + 1)][g - 1] = 1

    for l in st:
        x1[0][(l[0] - 1) * 2 * d + (2 * (l[1] - 1)) + 1] = 1

    for l in range(2, d * d * 2 + 1, 2):
        if l % (2 * d) != 0:
            x1[l][l + 1] = 1

    for l in range(2, d * d * 2 + 1, 2):
        if (l - 2) % (2 * d) != 0:
            x1[l][l - 3] = 1

    for l in range(2, d * d * 2 + 1, 2):
        if l - (2 * d) > 0:
            x1[l][l - (2 * d) - 1] = 1

    for l in range(2, d * d * 2 + 1, 2):
        if l + (2 * d) < d * d * 2 + 1:
            x1[l][l + (2 * d) - 1] = 1

    max_flow = edmonds_karp(x1, 0, g - 1)

    return max_flow

if __name__ == '__main__':
    # 사용자로부터 격자 크기 입력 받기
    grid_size = int(input("Enter the grid size (n): "))

    # 사용자로부터 탈출 좌표 입력 받기
    num_escape_points = int(input("Enter the number of escape points: "))
    escape_points = []
    for _ in range(num_escape_points):
        x, y = map(int, input("Enter escape point coordinates (x y): ").split())
        escape_points.append((x, y))

    # Escape 함수 실행
    final_flow = escape(grid_size, escape_points)
    
    # 결과 출력
    print("Maximum Flow:", final_flow)
    print("Starting Escape vertices:", num_escape_points)
    print("Possible" if final_flow == num_escape_points else "Not Possible")
