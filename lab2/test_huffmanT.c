#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "huffmanT.h"

extern HuffmanCode codeTable[MAX_CODETABLE_SIZE];
extern int codeTableSize;

void test_getCode() {
    // 设置测试编码表
    codeTableSize = 3;
    codeTable[0].character = 'a';
    strcpy(codeTable[0].code, "0");
    codeTable[1].character = 'b';
    strcpy(codeTable[1].code, "10");
    codeTable[2].character = 'c';
    strcpy(codeTable[2].code, "11");

    assert(strcmp(getCode('a'), "0") == 0);
    assert(strcmp(getCode('b'), "10") == 0);
    assert(strcmp(getCode('c'), "11") == 0);
    assert(getCode('d') == NULL);
    printf("test_getCode 通过。\n");
}

void test_getCharacter() {
    // 设置测试编码表
    codeTableSize = 3;
    codeTable[0].character = 'a';
    strcpy(codeTable[0].code, "0");
    codeTable[1].character = 'b';
    strcpy(codeTable[1].code, "10");
    codeTable[2].character = 'c';
    strcpy(codeTable[2].code, "11");

    assert(getCharacter("0") == 'a');
    assert(getCharacter("10") == 'b');
    assert(getCharacter("11") == 'c');
    assert(getCharacter("111") == '\0');
    printf("test_getCharacter 通过。\n");
}

void test_encodeText() {
    // 设置测试编码表
    codeTableSize = 3;
    codeTable[0].character = 'a';
    strcpy(codeTable[0].code, "0");
    codeTable[1].character = 'b';
    strcpy(codeTable[1].code, "10");
    codeTable[2].character = 'c';
    strcpy(codeTable[2].code, "11");

    char encoded[100];
    encodeText("abc", encoded);
    assert(strcmp(encoded, "01011") == 0);
    printf("test_encodeText 通过。\n");
}

void test_decodeText() {
    // 设置测试编码表
    codeTableSize = 3;
    codeTable[0].character = 'a';
    strcpy(codeTable[0].code, "0");
    codeTable[1].character = 'b';
    strcpy(codeTable[1].code, "10");
    codeTable[2].character = 'c';
    strcpy(codeTable[2].code, "11");

    char decoded[100];
    decodeText("01011", decoded);
    assert(strcmp(decoded, "abc") == 0);
    printf("test_decodeText 通过。\n");
}

int main() {
    test_getCode();
    test_getCharacter();
    test_encodeText();
    test_decodeText();
    printf("所有单元测试通过。\n");
    return 0;
}