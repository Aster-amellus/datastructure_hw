#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100   // 最大景点数量
#define INF 99999 // 无穷大表示不可达

// 定义景点结构
typedef struct {
    char name[50]; // 景点名称
    int id;        // 景点ID
    char description[200]; // 景点描述
} Spot;

// 定义图结构
typedef struct {
    Spot spots[MAX];  // 景点信息
    int adjMatrix[MAX][MAX]; // 邻接矩阵表示路径距离
    int spotCount;     // 景点数量
} CampusMap;

// 初始化校园图
void initMap(CampusMap *map) {
    map->spotCount = 0;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            map->adjMatrix[i][j] = (i == j) ? 0 : INF;
        }
    }
}

// 添加景点
void addSpot(CampusMap *map, int id, const char *name, const char *description) {
    if (map->spotCount >= MAX) {
        printf("景点数量已达最大限制！\n");
        return;
    }
    Spot *newSpot = &map->spots[map->spotCount++];
    newSpot->id = id;
    strcpy(newSpot->name, name);
    strcpy(newSpot->description, description);
}

// 添加路径
void addPath(CampusMap *map, int from, int to, int distance) {
    if (from >= MAX || to >= MAX) {
        printf("无效的景点ID！\n");
        return;
    }
    map->adjMatrix[from][to] = distance;
    map->adjMatrix[to][from] = distance; // 双向边
}

// Dijkstra算法计算最短路径
void dijkstra(CampusMap *map, int start, int end) {
    int dist[MAX], visited[MAX], prev[MAX];
    for (int i = 0; i < MAX; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (int i = 0; i < map->spotCount; i++) {
        int minDist = INF, u = -1;
        for (int j = 0; j < map->spotCount; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < map->spotCount; v++) {
            if (!visited[v] && map->adjMatrix[u][v] != INF) {
                int newDist = dist[u] + map->adjMatrix[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                }
            }
        }
    }

    if (dist[end] == INF) {
        printf("从%s到%s无路径！\n", map->spots[start].name, map->spots[end].name);
    } else {
        printf("从%s到%s的最短路径长度为：%d\n", map->spots[start].name, map->spots[end].name, dist[end]);
        printf("路径：");
        int path[MAX], count = 0;
        for (int at = end; at != -1; at = prev[at]) {
            path[count++] = at;
        }
        for (int i = count - 1; i >= 0; i--) {
            printf("%s", map->spots[path[i]].name);
            if (i > 0) printf(" -> ");
        }
        printf("\n");
    }
}

// 主函数
int main() {
    CampusMap map;
    initMap(&map);

    // 添加景点
    addSpot(&map, 0, "正门", "校园的正门入口");
    addSpot(&map, 1, "图书馆", "丰富的书籍和学习空间");
    addSpot(&map, 2, "操场", "学生运动的地方");
    addSpot(&map, 3, "教学楼", "上课的主要场所");

    // 添加路径
    addPath(&map, 0, 1, 5);
    addPath(&map, 1, 2, 3);
    addPath(&map, 2, 3, 4);
    addPath(&map, 0, 3, 10);

    // 查询两点最短路径
    int start, end;
    printf("输入起点ID和终点ID（例如 0 3）：");
    scanf("%d %d", &start, &end);
    dijkstra(&map, start, end);

    return 0;
}
