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
