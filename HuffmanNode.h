#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

class HuffmanNode {
public:
    char data;
    unsigned freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

#endif // HUFFMANNODE_H
