#include "huffmanT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
