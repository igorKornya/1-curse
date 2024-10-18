#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "HuffmanNode.h"
#include <iostream>
#include <map>
#include <queue>

using namespace std;

class compare {
public:
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return (left->freq > right->freq);
    }
};

class Huffman {
public:
    map<char, string> huffmanCodes;
    HuffmanNode* root;

    Huffman() : root(nullptr) {}
    ~Huffman();

    void buildHuffmanTree(const string& text);
    void generateCodes(HuffmanNode* root, string str);
    string getHuffmanEncoding(const string& text);
    string decodeHuffmanEncoding(const string& encodedString);

private:
    void clear(HuffmanNode* node);
    string decodeHuffmanHelper(const string& encodedString);
};

#endif // HUFFMAN_H
