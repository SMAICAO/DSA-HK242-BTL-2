#ifndef INVENTORY_COMPRESSOR_H
#define INVENTORY_COMPRESSOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include "inventory.h"
#include "hash/xMap.h"
#include "heap/Heap.h"
#include "list/XArrayList.h"

template<int treeOrder>
class HuffmanTree {
public:
    struct HuffmanNode {
        char symbol;
        int freq;
        XArrayList<HuffmanNode*> children;

        HuffmanNode(char s, int f) : symbol(s), freq(f) {}; //Leaf node
        HuffmanNode(int f, const  XArrayList<HuffmanNode*>& childs) : freq(f), children(childs) {}; //Internal node
    };

    HuffmanTree();
    ~HuffmanTree();

    void build(XArrayList<pair<char, int>>& symbolsFreqs);
    void generateCodes(xMap<char, std::string>& table);
    std::string decode(const std::string& huffmanCode);

private:
    HuffmanNode* root;
};

template<int treeOrder>
class InventoryCompressor {
public:
    InventoryCompressor(InventoryManager* manager);
    ~InventoryCompressor();

    void buildHuffman();
    void printHuffmanTable();
    std::string productToString(const List1D<InventoryAttribute>& attributes, const std::string& name);
    std::string encodeHuffman(const List1D<InventoryAttribute>& attributes, const std::string& name);
    std::string decodeHuffman(const std::string& huffmanCode, List1D<InventoryAttribute>& attributesOutput, std::string& nameOutput);

private:
    xMap<char, std::string>* huffmanTable;
    InventoryManager* invManager;
    HuffmanTree<treeOrder>* tree;
};


#endif // INVENTORY_COMPRESSOR_H

template <int treeOrder>
HuffmanTree<treeOrder>::HuffmanTree()
{
}

template <int treeOrder>
HuffmanTree<treeOrder>::~HuffmanTree()
{
    //TODO
    root->children.clear();
    delete root;
}

template <int treeOrder>
void HuffmanTree<treeOrder>::build(XArrayList<pair<char, int>>& symbolsFreqs)
{
    //TODO
    //(a) Create a heap from the given list.
    Heap<HuffmanNode*> heap([](HuffmanNode* a, HuffmanNode* b) -> int {
        if (a->freq < b->freq) return -1;
        else if (a->freq > b->freq) return 1;
        else if (a->symbol < b->symbol) return -1;
        else if (a->symbol > b->symbol) return 1;
        else return 0;
    }, 0);

    for (auto it = symbolsFreqs.begin(); it != symbolsFreqs.end(); ++it) {
        HuffmanNode* node = new HuffmanNode((*it)->first, (*it)->second);
        heap.push(node);
    }

    if ((symbolsFreqs.size() - 1) % (treeOrder - 1) != 0) {
        for (int i = 0; i < (treeOrder - 1) - ((symbolsFreqs.size() - 1) % (treeOrder - 1)); ++i) {
            HuffmanNode* node = new HuffmanNode('\0', 0);
            heap.push(node);
        }
    }

    //(b) While the heap has more than one node, select up to treeOrder nodes with the
    //lowest frequencies.
    while (heap.size() > 1) {
        //(c) Compute the total frequency and group selected nodes into a list.
        XArrayList<HuffmanNode*> children;
        int totalFreq = 0;
        for (int i = 0; i < treeOrder && heap.size() > 0; ++i) {
            HuffmanNode* child = heap.pop();
            children.add(child);
            totalFreq += child->freq;
        }

        //(d) Create a new internal node with the computed frequency and list of children.
        HuffmanNode* parent = new HuffmanNode(totalFreq, children);

        //(e) Push the new internal node into the heap.
        heap.push(parent);
    }

    //(f) After the loop, the last remaining node becomes the root of the tree
    root = heap.pop();
}

template <int treeOrder>
void HuffmanTree<treeOrder>::generateCodes(xMap<char, std::string> &table)
{
    //TODO
    // Base 16 for encoding
    char base16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    
    // DFS to generate the codes
    std::string code = "";
    XArrayList<pair<HuffmanNode*, std::string>> stack;
    stack.push(make_pair(root, code));

    while (!stack.empty()) {
        auto nodePair = stack.pop();
        HuffmanNode* node = nodePair.first;
        std::string code = nodePair.second;

        // If node is leaf
        if (node->children.empty()) {
            table.put(node->symbol, code);
        
        // If node is parent
        } else {
            for (int i = 0; i < node->children.size(); ++i) {
                stack.push(make_pair(node->children.get(i), code + base16[i]));
            }
        }
    }
}

template <int treeOrder>
std::string HuffmanTree<treeOrder>::decode(const std::string &huffmanCode)
{
    //TODO
    // Get codes
    xMap<char, std::string> &table;
    generateCodes(table);

    // Find the symbol corresponding to the given huffman code
    DLinkedList<char> symbols = table.keys();
    DLinkedList<std::string> codes = table.values();

    int index = codes.indexOf(huffmanCode);
    // Code found
    if (index != -1) {
        return symbols.get(index);
    
    // Code not found
    } else {
        return std::string();
    }
}

template <int treeOrder>
InventoryCompressor<treeOrder>::InventoryCompressor(InventoryManager *manager)
{
    //TODO
}

template <int treeOrder>
InventoryCompressor<treeOrder>::~InventoryCompressor()
{
    //TODO
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::buildHuffman()
{
    //TODO
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::printHuffmanTable() {
    DLinkedList<char> keys = huffmanTable->keys();
    for (char ch : keys) {
        std::cout << "'" << ch << "' : " << huffmanTable->get(ch) << std::endl;
    }
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::productToString(const List1D<InventoryAttribute> &attributes, const std::string &name)
{
    //TODO
    return std::string();
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::encodeHuffman(const List1D<InventoryAttribute> &attributes, const std::string &name)
{
    //TODO
    return std::string();
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::decodeHuffman(const std::string &huffmanCode, List1D<InventoryAttribute> &attributesOutput, std::string &nameOutput)
{
    //TODO
    return std::string();
}