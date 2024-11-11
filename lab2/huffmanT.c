#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define MAX_CODETABLE_SIZE 256  // 最大编码表大小

// 定义字符与哈夫曼编码的映射结构
typedef struct {
    char character;
    char code[100];  // 增大编码长度以防止溢出
} HuffmanCode;

// 哈夫曼树节点结构
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// 最小堆结构
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// 创建一个哈夫曼编码映射表
HuffmanCode codeTable[MAX_CODETABLE_SIZE];
int codeTableSize = 0;

// 创建一个新节点
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    if (!temp) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// 创建一个最小堆
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    if (!minHeap) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    if (!minHeap->array) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    return minHeap;
}

// 交换两个最小堆节点
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// 堆化
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// 提取最小值节点
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    if (minHeap->size == 0)
        return NULL;

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// 插入节点到最小堆
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    if (minHeap->size == minHeap->capacity) {
        fprintf(stderr, "最小堆已满，无法插入新节点\n");
        return;
    }

    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// 检查节点是否为叶节点
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// 构建最小堆
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// 创建并构建最小堆
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// 构建哈夫曼树
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (minHeap->size != 1) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    struct MinHeapNode* root = extractMin(minHeap);
    free(minHeap->array);
    free(minHeap);
    return root;
}

// 打印并保存哈夫曼编码
void printCodes(struct MinHeapNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (isLeaf(root)) {
        printf("%c: ", root->data);
        for (int i = 0; i < top; ++i)
            printf("%d", arr[i]);
        printf("\n");

        // 保存到codeTable
        if (codeTableSize >= MAX_CODETABLE_SIZE) {
            fprintf(stderr, "编码表已满，无法保存更多编码\n");
            return;
        }

        codeTable[codeTableSize].character = root->data;
        for (int i = 0; i < top && i < (int)(sizeof(codeTable[0].code) - 1); ++i) {
            codeTable[codeTableSize].code[i] = '0' + arr[i];
        }
        codeTable[codeTableSize].code[top] = '\0';
        codeTableSize++;
    }
}

// 辅助函数：读取一个字符，包括空格
int readCharacter(char *c) {
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    *c = buffer[0];
    return 1;
}

// 初始化哈夫曼编码
void initializeHuffman() {
    int n;
    printf("输入字符集大小: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_CODETABLE_SIZE) {
        printf("无效的字符集大小。\n");
        while (getchar() != '\n'); // 清除输入缓冲区
        return;
    }
    getchar();  // 消费掉换行符

    char* characters = (char*)malloc(n * sizeof(char));
    int* frequencies = (int*)malloc(n * sizeof(int));

    if (!characters || !frequencies) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }

    printf("输入 %d 个字符和对应的权值:\n", n);
    for (int i = 0; i < n; i++) {
        printf("字符 %d: ", i + 1);
        if (!readCharacter(&characters[i])) {
            printf("输入错误。\n");
            free(characters);
            free(frequencies);
            return;
        }
        printf("读取的字符: '%c'\n", characters[i]); // 调试信息
        printf("权值 %d: ", i + 1);
        if (scanf("%d", &frequencies[i]) != 1) {
            printf("输入错误。\n");
            free(characters);
            free(frequencies);
            while (getchar() != '\n'); // 清除输入缓冲区
            return;
        }
        getchar(); // 消费掉换行符
    }

    struct MinHeapNode* root = buildHuffmanTree(characters, frequencies, n);
    int arr[MAX_TREE_HT], top = 0;
    codeTableSize = 0; // 重置编码表大小
    printf("生成的哈夫曼编码如下:\n");
    printCodes(root, arr, top);

    // 释放哈夫曼树
    // 这里可以递归释放所有节点，但为了简化，暂时不实现
    // 在实际应用中，请确保释放所有动态分配的内存

    free(characters);
    free(frequencies);
}

// 编码函数：将文本转换为哈夫曼编码
void encodeText(const char *text, char *encodedText);
// 解码函数：将哈夫曼编码转换为文本
void decodeText(const char *encodedText, char *decodedText);
// 根据字符获取编码
const char* getCode(char character);
// 根据编码获取字符
char getCharacter(const char *code);

// 编码函数：将文本转换为哈夫曼编码
void encodeText(const char *text, char *encodedText) {
    encodedText[0] = '\0'; // 确保编码文本为空
    for (int i = 0; text[i] != '\0'; i++) {
        const char *code = getCode(text[i]);
        if (code) {
            strcat(encodedText, code);
        } else {
            printf("字符 '%c' 未找到对应编码。\n", text[i]);
        }
    }
}

// 解码函数：将哈夫曼编码转换为文本
void decodeText(const char *encodedText, char *decodedText) {
    int j = 0;
    char code[100];  // 增大编码长度
    int codeIndex = 0;

    while (*encodedText) {
        code[codeIndex++] = *encodedText++;
        code[codeIndex] = '\0';

        char character = getCharacter(code);
        if (character != '\0') {  // 找到匹配字符
            decodedText[j++] = character;
            codeIndex = 0;  // 重置codeIndex
        }
        // 防止codeIndex超出数组范围
        if (codeIndex >= (int)(sizeof(code) - 1)) {
            printf("编码过长，无法解码。\n");
            break;
        }
    }
    decodedText[j] = '\0';
}

// 根据字符获取编码
const char* getCode(char character) {
    for (int i = 0; i < codeTableSize; i++) {
        if (codeTable[i].character == character) {
            return codeTable[i].code;
        }
    }
    return NULL;  // 未找到编码
}

// 根据编码获取字符
char getCharacter(const char *code) {
    for (int i = 0; i < codeTableSize; i++) {
        if (strcmp(codeTable[i].code, code) == 0) {
            return codeTable[i].character;
        }
    }
    return '\0';  // 未找到字符
}

// 将内容保存到文件
void saveToFile(const char *filename, const char *content) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("无法打开文件 %s\n", filename);
        return;
    }
    fprintf(fp, "%s", content);
    fclose(fp);
}

// 从文件中读取内容
void loadFromFile(const char *filename, char *content) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("无法打开文件 %s\n", filename);
        content[0] = '\0';  // 确保内容为空
        return;
    }
    fgets(content, 500, fp);
    fclose(fp);
}

int main() {
    int choice;
    char text[100];
    char encodedText[500];
    char decodedText[100];

    while (1) {
        printf("\n菜单:\n");
        printf("1. 初始化哈夫曼编码 (Initialize Huffman Encoding)\n");
        printf("2. 编码 (Encoding)\n");
        printf("3. 解码 (Decoding)\n");
        printf("4. 打印编码文件 (Print Code File)\n");
        printf("5. 打印哈夫曼编码表 (Print Huffman Codes)\n");
        printf("Q. 退出\n");
        printf("选择操作: ");
        choice = getchar();
        getchar();  // 消费掉换行符

        switch (choice) {
            case '1':
                initializeHuffman();
                break;

            case '2':
                if (codeTableSize == 0) {
                    printf("请先初始化哈夫曼编码 (选择选项 1)\n");
                    break;
                }
                printf("输入要编码的文本: ");
                if (fgets(text, sizeof(text), stdin) == NULL) {
                    printf("输入错误。\n");
                    break;
                }
                text[strcspn(text, "\n")] = '\0';  // 去掉换行符
                encodeText(text, encodedText);
                saveToFile("CodeFile.txt", encodedText);
                printf("编码结果已保存到 CodeFile.txt\n");
                break;

            case '3':
                if (codeTableSize == 0) {
                    printf("请先初始化哈夫曼编码 (选择选项 1)\n");
                    break;
                }
                loadFromFile("CodeFile.txt", encodedText);
                if (encodedText[0] == '\0') {
                    printf("编码文件为空或无法读取。\n");
                    break;
                }
                decodeText(encodedText, decodedText);
                saveToFile("TextFile.txt", decodedText);
                printf("解码结果: %s\n", decodedText);
                printf("解码结果已保存到 TextFile.txt\n");
                break;

            case '4':
                loadFromFile("CodeFile.txt", encodedText);
                if (encodedText[0] == '\0') {
                    printf("编码文件为空或无法读取。\n");
                    break;
                }
                printf("CodeFile.txt 内容:\n%s\n", encodedText);
                break;

            case '5':
                if (codeTableSize == 0) {
                    printf("哈夫曼编码尚未初始化。\n");
                } else {
                    printf("哈夫曼编码表:\n");
                    for (int i = 0; i < codeTableSize; i++) {
                        if (codeTable[i].character == ' ') {
                            printf("字符: '空格', 编码: %s\n", codeTable[i].code);
                        } else {
                            printf("字符: %c, 编码: %s\n", codeTable[i].character, codeTable[i].code);
                        }
                    }
                }
                break;

            case 'Q':
            case 'q':
                printf("退出程序。\n");
                exit(0);
                break;

            default:
                printf("无效选项，请重新选择。\n");
        }
    }
    return 0;
}
