#ifndef HUFFMANT_H
#define HUFFMANT_H

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

// 添加 extern 声明以供其他文件引用
extern HuffmanCode codeTable[MAX_CODETABLE_SIZE];
extern int codeTableSize;

// 编码函数：将文本转换为哈夫曼编码
void encodeText(const char *text, char *encodedText);
// 解码函数：将哈夫曼编码转换为文本
void decodeText(const char *encodedText, char *decodedText);
// 根据字符获取编码
const char* getCode(char character);
// 根据编码获取字符
char getCharacter(const char *code);

// 初始化哈夫曼编码
void initializeHuffman();
// 构建哈夫曼树
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size);
// 打印并保存哈夫曼编码
void printCodes(struct MinHeapNode* root, int arr[], int top);
// 保存内容到文件
void saveToFile(const char *filename, const char *content);
// 从文件中读取内容
void loadFromFile(const char *filename, char *content);

#endif // HUFFMANT_H

