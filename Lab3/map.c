#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000    // 最大景点数量
#define INF 99999   // 无穷大表示不可达

// 定义景点结构(顶点)
typedef struct {
    char name[50];         // 景点名称
    char description[200]; // 景点描述
} Spot;

// 定义图结构
typedef struct {
    Spot spots[MAX];           // 景点信息
    int adjMatrix[MAX][MAX];   // 邻接矩阵表示路径距离
    int spotCount;             // 景点数量
} CampusMap;

// 函数声明
void initMap(CampusMap *map);
void addSpot(CampusMap *map, const char *name, const char *description);
void addPath(CampusMap *map, int from, int to, int distance);
void dijkstra(CampusMap *map, int start, int end); // Dijkstra算法计算最短路径
void findAllPaths(CampusMap *map, int start, int end);
void findArticulationPoints(CampusMap *map);
int loadDataFromFile(CampusMap *map, const char *filename);
void inputDataManually(CampusMap *map);

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
void addSpot(CampusMap *map, const char *name, const char *description) {
    if (map->spotCount >= MAX) {
        printf("景点数量已达最大限制！\n");
        return;
    }
    Spot *newSpot = &map->spots[map->spotCount++];
    strcpy(newSpot->name, name);
    strcpy(newSpot->description, description);
}

// 添加路径
void addPath(CampusMap *map, int from, int to, int distance) {
    if (from < 0 || from >= map->spotCount || to < 0 || to >= map->spotCount) {
        printf("无效的景点索引！\n");
        return;
    }
    map->adjMatrix[from][to] = distance;
    map->adjMatrix[to][from] = distance; // 双向边
}

// Dijkstra算法计算最短路径
void dijkstra(CampusMap *map, int start, int end) {
    //边界检查
    if (start < 0 || start >= map->spotCount || end < 0 || end >= map->spotCount) {
        printf("无效的起点或终点索引！\n");
        return;
    }

    //是否可达/已经访问/有无到达邻居的路径
    int dist[MAX], visited[MAX], prev[MAX];
    for (int i = 0; i < map->spotCount; i++) {
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
        //选择当前最短路径的点，更新其邻居的路径
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

// 辅助函数：查找所有路径
void findAllPathsUtil(CampusMap *map, int u, int d, int visited[], int path[], int pathIndex) {
    visited[u] = 1;
    path[pathIndex] = u;
    pathIndex++;

    if (u == d) {
        // 打印一条路径
        for (int i = 0; i < pathIndex; i++) {
            printf("%s", map->spots[path[i]].name);
            if (i < pathIndex - 1) printf(" -> ");
        }
        printf("\n");
    } else {
        for (int v = 0; v < map->spotCount; v++) {
            if (!visited[v] && map->adjMatrix[u][v] != INF) {
                findAllPathsUtil(map, v, d, visited, path, pathIndex);
            }
        }
    }

    // Backtrack
    visited[u] = 0;
}

// 查找所有路径
void findAllPaths(CampusMap *map, int start, int end) {
    if (start < 0 || start >= map->spotCount || end < 0 || end >= map->spotCount) {
        printf("无效的起点或终点索引！\n");
        return;
    }

    int visited[MAX] = {0};
    int path[MAX];
    int pathIndex = 0;
    printf("从%s到%s的所有路径：\n", map->spots[start].name, map->spots[end].name);
    findAllPathsUtil(map, start, end, visited, path, pathIndex);
}

// 辅助函数：查找关节点
void APUtil(CampusMap *map, int u, int visited[], int disc[], int low[], int parent[], int ap[], int *timeCounter) {
    int children = 0; // 记录子节点数量
    visited[u] = 1; // 标记当前节点为已访问
    disc[u] = low[u] = ++(*timeCounter); // 设置发现的timestamp和最低可达的timestamp（最早能回到根节点的深度）
    
    for (int v = 0; v < map->spotCount; v++) {
        if (map->adjMatrix[u][v] != INF && u != v) {
            if (!visited[v]) {
                children++; 
                parent[v] = u; // 设置节点v的父节点为u
                APUtil(map, v, visited, disc, low, parent, ap, timeCounter); // 递归访问节点v
                // 更新节点u的低值
                low[u] = (low[u] < low[v]) ? low[u] : low[v];
                
                // 如果u是根节点并且有多个子节点，则u是关节点
                if (parent[u] == -1 && children > 1)
                    ap[u] = 1;
                
                // 如果u不是根节点且v的低值大于等于u的发现时间，则u是关节点
                if (parent[u] != -1 && low[v] >= disc[u])
                    ap[u] = 1;
            }
            else if (v != parent[u]) { // 如果节点v已经被访问过且不是父节点
                // 更新节点u的低值为节点v的发现时间中的较小者
                low[u] = (low[u] < disc[v]) ? low[u] : disc[v];
            }
        }
    }
}

// 查找关节点
void findArticulationPoints(CampusMap *map) {
    int visited[MAX] = {0};
    int disc[MAX] = {0};
    int low[MAX] = {0};
    int parent[MAX];
    int ap[MAX] = {0};
    int timeCounter = 0;

    for (int i = 0; i < map->spotCount; i++) {
        parent[i] = -1;
    }

    for (int i = 0; i < map->spotCount; i++) {
        if (!visited[i]) {
            APUtil(map, i, visited, disc, low, parent, ap, &timeCounter);
        }
    }

    printf("图的关节点为：\n");
    int found = 0;
    for (int i = 0; i < map->spotCount; i++) {
        if (ap[i] == 1) {
            printf("%s\n", map->spots[i].name);
            found = 1;
        }
    }
    if (!found) {
        printf("没有关节点。\n");
    }
}

// 将函数返回类型改为 int，表示加载是否成功
int loadDataFromFile(CampusMap *map, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("无法打开文件：%s\n", filename);
        return 0; // 返回 0 表示加载失败
    }

    initMap(map); // 加载前初始化

    int id, from, to, distance;
    char name[50], description[200];
    int spotCount;

    if (fscanf(file, "%d\n", &spotCount) != 1) {
        printf("读取景点数量失败。\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < spotCount; i++) {
        if (fscanf(file, "%d\n", &id) != 1) {
            printf("读取景点ID失败。\n");
            fclose(file);
            return 0;
        }
        if (!fgets(name, sizeof(name), file)) {
            printf("读取景点名称失败。\n");
            fclose(file);
            return 0;
        }
        name[strcspn(name, "\n")] = '\0'; // 去除换行符
        if (!fgets(description, sizeof(description), file)) {
            printf("读取景点描述失败。\n");
            fclose(file);
            return 0;
        }
        description[strcspn(description, "\n")] = '\0';
        addSpot(map, name, description);
    }

    int pathCount;
    if (fscanf(file, "%d", &pathCount) != 1) {
        printf("读取路径数量失败。\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < pathCount; i++) {
        if (fscanf(file, "%d %d %d", &from, &to, &distance) != 3) {
            printf("读取路径信息失败。\n");
            fclose(file);
            return 0;
        }
        addPath(map, from, to, distance);
    }

    fclose(file);
    return 1; // 返回 1 表示加载成功
}

// 手动输入数据
void inputDataManually(CampusMap *map) {
    initMap(map); // 加载前初始化

    int spotCount;
    printf("输入景点数量：");
    if (scanf("%d", &spotCount) != 1 || spotCount < 0 || spotCount > MAX) {
        printf("无效的景点数量！\n");
        while (getchar() != '\n'); // 清理输入缓冲区
        return;
    }

    for (int i = 0; i < spotCount; i++) {
        char name[50], description[200];
        printf("输入景点名称：");
        getchar(); // 吸收换行符
        if (!fgets(name, sizeof(name), stdin)) {
            printf("读取景点名称失败。\n");
            return;
        }
        name[strcspn(name, "\n")] = '\0';
        printf("输入景点描述：");
        if (!fgets(description, sizeof(description), stdin)) {
            printf("读取景点描述失败。\n");
            return;
        }
        description[strcspn(description, "\n")] = '\0';
        addSpot(map, name, description);
    }

    int pathCount;
    printf("输入路径数量：");
    if (scanf("%d", &pathCount) != 1 || pathCount < 0 || pathCount > MAX * (MAX - 1) / 2) {
        printf("无效的路径数量！\n");
        while (getchar() != '\n'); // 清理输入缓冲区
        return;
    }

    for (int i = 0; i < pathCount; i++) {
        int from, to, distance;
        printf("输入路径的起点索引、终点索引和距离（例如 0 1 5）：");
        if (scanf("%d %d %d", &from, &to, &distance) != 3) {
            printf("输入格式错误！\n");
            while (getchar() != '\n'); // 清理输入缓冲区
            return;
        }
        addPath(map, from, to, distance);
    }
}

int main() {
    CampusMap map;
    initMap(&map);

    int choice;
    printf("请选择输入方式（1. 从文件读取  2. 手动输入）：");
    if (scanf("%d", &choice) != 1) {
        printf("输入格式错误！\n");
        return 0;
    }

    if (choice == 1) {
        char filename[100];
        printf("请输入文件名：");
        if (scanf("%s", filename) != 1) {
            printf("输入文件名失败！\n");
            return 0;
        }
        // 检查数据是否成功加载
        if (!loadDataFromFile(&map, filename)) {
            // 如果加载失败，退出程序或提示重新输入
            return 0;
        }
    } else if (choice == 2) {
        inputDataManually(&map);
    } else {
        printf("无效的选择！\n");
        return 0;
    }

    int option;
    printf("请选择功能（1. 查询最短路径  2. 查询所有路径  3. 查找关节点）：");
    if (scanf("%d", &option) != 1) {
        printf("输入格式错误！\n");
        return 0;
    }

    if (option == 1) {
        int start, end;
        printf("输入起点索引和终点索引（例如 0 3）：");
        if (scanf("%d %d", &start, &end) != 2) {
            printf("输入格式错误！\n");
            return 0;
        }
        dijkstra(&map, start, end);
    } else if (option == 2) {
        int start, end;
        printf("输入起点索引和终点索引（例如 0 3）：");
        if (scanf("%d %d", &start, &end) != 2) {
            printf("输入格式错误！\n");
            return 0;
        }
        if (start < 0 || start >= map.spotCount || end < 0 || end >= map.spotCount) {
            printf("无效的景点索引！\n");
            return 0;
        }
        findAllPaths(&map, start, end);
    } else if (option == 3) {
        findArticulationPoints(&map);
    } else {
        printf("无效的选择！\n");
    }

    return 0;
}
