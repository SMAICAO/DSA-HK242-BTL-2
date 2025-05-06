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

ostream& operator<<(ostream& os, pair<char, int>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

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

    void generateCodesRecursive(HuffmanNode* node, std::string code, xMap<char, std::string>& table, const char base16[]);
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
    Heap<HuffmanNode*> heap([](HuffmanNode*& a, HuffmanNode*& b)->int {
        if (a->freq < b->freq) return -1;
        else if (a->freq > b->freq) return 1;
        else if (a->symbol < b->symbol) return -1;
        else if (a->symbol > b->symbol) return 1;
        else return 0;
    }, 0);

    for (auto it = symbolsFreqs.begin(); it != symbolsFreqs.end(); ++it) {
        HuffmanNode* node = new HuffmanNode((*it).first, (*it).second);
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
void HuffmanTree<treeOrder>::generateCodes(xMap<char, std::string> &table) {
    // TODO
    // Base 16 for encoding
    char base16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    generateCodesRecursive(root, "", table, base16);
}

// Helper recursive function
template <int treeOrder>
void HuffmanTree<treeOrder>::generateCodesRecursive(HuffmanNode* node, std::string code, 
                                                   xMap<char, std::string> &table, const char base16[]) {
    // If leaf node, store the code
    if (node->children.empty()) {
        table.put(node->symbol, code);
        return;
    }
    
    // If internal node, recursively process all children
    for (int i = 0; i < node->children.size(); i++) {
        generateCodesRecursive(node->children.get(i), code + base16[i], table, base16);
    }
}

template <int treeOrder>
std::string HuffmanTree<treeOrder>::decode(const std::string &huffmanCode)
{
    //TODO
    // Get codes
    xMap<char, std::string> table([](char& symbol, int capacity)->int {
        return 0;
    }, 0.75, 0, 0, [](char& a, char& b)->bool {
        return a == b;
    });
    generateCodes(table);

    // Find the symbol corresponding to the given huffman code
    DLinkedList<char> symbols = table.keys();
    DLinkedList<std::string> codes = table.values();

    string decodedStr = "";
    for (int i = 0; i < huffmanCode.length(); ++i) {
        std::string code = huffmanCode.substr(i, 1);
        for (int j = i; j < huffmanCode.length(); ++j) {
            if (codes.contains(code)) {
                int index = codes.indexOf(code);
                decodedStr += symbols.get(index);
                i += code.length() - 1;
                break;
            } else {
                code += huffmanCode[j];
            }
        }
    }
    return decodedStr;
}

template <int treeOrder>
InventoryCompressor<treeOrder>::InventoryCompressor(InventoryManager *manager)
{
    //TODO
    invManager = manager;
    huffmanTable = new xMap<char, std::string>([](char& symbol, int capacity)->int {
        return 0;
    });
    tree = new HuffmanTree<treeOrder>();
}

template <int treeOrder>
InventoryCompressor<treeOrder>::~InventoryCompressor()
{
    //TODO
    delete huffmanTable;
    delete invManager;
    delete tree;
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::buildHuffman()
{
    //TODO
    // (a) Traverse all products to build a frequency table for each character in the string
    // representation of a product using productToString.
    XArrayList<pair<char, int>> symbolsFreqs(0, [](pair<char, int>& a, pair<char, int>& b)->bool {
        return a.first == b.first;
    }, 256);

    // Get product strings
    std::string productStr = "";
    for (int i = 0; i < invManager->size(); ++i) {
        List1D<InventoryAttribute> attributes = invManager->getProductAttributes(i);
        std::string name = invManager->getProductName(i);
        productStr += productToString(attributes, name);
    }

    // Count frequencies of each character in the product string
    for (int i = 0; i < productStr.length(); ++i) {
        char ch = productStr[i];
        int freq = 1;
        if (symbolsFreqs.contains(make_pair(ch, freq))) {
            symbolsFreqs.get(symbolsFreqs.indexOf(make_pair(ch, freq))).second++;
        } else {
            symbolsFreqs.add(make_pair(ch, freq));
        }
    }

    // (b) Create the huffmanTable by building a Huffman tree using the frequency table.
    tree->build(symbolsFreqs);
    tree->generateCodes(*huffmanTable);
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
    stringstream ss;

    ss << name << ":";
    for (int i = 0; i < attributes.size(); ++i) {
        if (i > 0) {
            ss << ", ";
        }
        
        ss << "("
            << attributes.get(i).name
            << ": "
            << attributes.get(i).value
            << ")";
    }

    return ss.str();
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::encodeHuffman(const List1D<InventoryAttribute> &attributes, const std::string &name)
{
    //TODO
    // Convert product data to a string using productToString,
    // then generate the code using huffmanTable.
    std::string productStr = productToString(attributes, name);
    std::string huffmanCode = "";
    for (int i = 0; i < productStr.length(); ++i) {
        char ch = productStr[i];
        huffmanCode += huffmanTable->get(ch);
    }
    return huffmanCode;
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::decodeHuffman(const std::string &huffmanCode, List1D<InventoryAttribute> &attributesOutput, std::string &nameOutput)
{
    //TODO
    // (a) Decode the string using the Huffman tree.
    std::string productString = tree->decode(huffmanCode);

    // (b) Parse the result string to extract the product name and attribute list. The
    // expected format is: "product name: (attribute1: value1), (attribute2: value2),
    // ...".
    std::string productName = productString.substr(0, productString.find(":"));
    std::string attributesStr = productString.substr(productString.find(":") + 1);
    List1D<InventoryAttribute> attributeList;
    while (attributesStr.length() > 0) {
        int start = attributesStr.find("(");
        int end = attributesStr.find(")");
        if (start == std::string::npos || end == std::string::npos) {
            break;
        }
        std::string attributeStr = attributesStr.substr(start + 1, end - start - 1);
        std::string name = attributeStr.substr(0, attributeStr.find(":"));
        double value = std::stod(attributeStr.substr(attributeStr.find(":") + 1));
        InventoryAttribute attribute(name, value);
        attributeList.add(attribute);
        attributesStr = attributesStr.substr(end + 1);
    }

    // (c) The method returns the decoded string. At the same time, the product name and
    // attribute list are assigned to nameOutput and attributesOutput, respectively.
    nameOutput = productName;
    attributesOutput = attributeList;
    return productString;
}