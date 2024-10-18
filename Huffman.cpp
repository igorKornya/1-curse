#include "Huffman.h"

void Huffman::buildHuffmanTree(const string& text) {
    map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, compare> minHeap;

    for (auto pair : freqMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() != 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();

        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        HuffmanNode* top = new HuffmanNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }

    root = minHeap.top();
    generateCodes(root, "");
}

void Huffman::generateCodes(HuffmanNode* root, string str) {
    if (!root) return;

    if (root->data != '$') {
        huffmanCodes[root->data] = str;
    }

    generateCodes(root->left, str + "0");
    generateCodes(root->right, str + "1");
}

string Huffman::getHuffmanEncoding(const string& text) {
    string encodedString = "";
    for (char ch : text) {
        encodedString += huffmanCodes[ch];
    }
    return encodedString;
}

string Huffman::decodeHuffmanEncoding(const string& encodedString) {
    return decodeHuffmanHelper(encodedString);
}

string Huffman::decodeHuffmanHelper(const string& encodedString) {
    string decodedString = "";
    HuffmanNode* currentNode = root;
    for (char bit : encodedString) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        if (!currentNode->left && !currentNode->right) {
            decodedString += currentNode->data;
            currentNode = root;
        }
    }
    return decodedString;
}

void Huffman::clear(HuffmanNode* node) {
    if (!node) return;

    clear(node->left);
    clear(node->right);
    delete node;
}

Huffman::~Huffman() {
    clear(root);
}
