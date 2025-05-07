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

    void generateCodesRecursive(HuffmanNode* node, std::string code, xMap<char, std::string>& table, const char base16[]);
    void deleteNode(HuffmanNode* node) {
        if (node == nullptr) return;
        for (int i = 0; i < node->children.size(); ++i) {
            deleteNode(node->children.get(i));
        }
        delete node;
    }
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
inline HuffmanTree<treeOrder>::HuffmanTree()
{
}

template <int treeOrder>
inline HuffmanTree<treeOrder>::~HuffmanTree()
{
    //TODO
    deleteNode(root);
}

template <int treeOrder>
inline void HuffmanTree<treeOrder>::build(XArrayList<pair<char, int>>& symbolsFreqs)
{
    //TODO
    //(a) Create a heap from the given list.
    Heap<HuffmanNode*> heap([](HuffmanNode*& a, HuffmanNode*& b)->int {
        if (a->freq < b->freq) return -1;
        else if (a->freq > b->freq) return 1;
        else {
            bool aIsLeaf = a->children.empty();
            bool bIsLeaf = b->children.empty();

            if (aIsLeaf && !bIsLeaf) return -1;
            if (!aIsLeaf && bIsLeaf) return 1;
            
            bool aIsDummy = (a->symbol == '\0');
            bool bIsDummy = (b->symbol == '\0');

            if (aIsDummy && !bIsDummy) return 1;
            if (!aIsDummy && bIsDummy) return -1;

            if (a->symbol < b->symbol) return -1;
            else if (a->symbol > b->symbol) return 1;
            else return 0;
        }
    });

    int heapSize = symbolsFreqs.size(), dummyNodes = 0;
    if ((heapSize - 1) % (treeOrder - 1) != 0) {
        dummyNodes = (treeOrder - 1) - ((heapSize - 1) % (treeOrder - 1));
    }
    const int ARRAY_SIZE = heapSize + dummyNodes;
    HuffmanNode* array[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (i < heapSize) {
            pair<char, int> pPair = symbolsFreqs.get(i);
            array[i] = new HuffmanNode(pPair.first, pPair.second);
        } else {
            array[i] = new HuffmanNode('\0', 0);
        }
    }
    heap.heapify(array, ARRAY_SIZE);

    XArrayList<HuffmanNode*> sortedFreqs;
    while (!heap.empty()) {
        sortedFreqs.add(heap.pop());
    }
    heap.heapsortNoPrint(sortedFreqs);

    //(b) While the heap has more than one node, select up to treeOrder nodes with the
    //lowest frequencies.
    while (sortedFreqs.size() > 1) {
        //(c) Compute the total frequency and group selected nodes into a list.
        XArrayList<HuffmanNode*> children;
        int totalFreq = 0;
        for (int i = 0; i < treeOrder && sortedFreqs.size() >= 0; ++i) {
            HuffmanNode* child = sortedFreqs.removeAt(0);
            children.add(child);
            totalFreq += child->freq;
        }

        //(d) Create a new internal node with the computed frequency and list of children.
        HuffmanNode* parent = new HuffmanNode(totalFreq, children);

        //(e) Push the new internal node into the heap.
        sortedFreqs.add(0, parent);
        heap.heapsortNoPrint(sortedFreqs);
    }

    //(f) After the loop, the last remaining node becomes the root of the tree
    root = sortedFreqs.get(0);
}

template <int treeOrder>
inline void HuffmanTree<treeOrder>::generateCodes(xMap<char, std::string> &table) {
    // TODO
    // Base 16 for encoding
    char base16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    generateCodesRecursive(root, "", table, base16);
}

// Helper recursive function
template <int treeOrder>
inline void HuffmanTree<treeOrder>::generateCodesRecursive(HuffmanNode* node, std::string code, 
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
inline std::string HuffmanTree<treeOrder>::decode(const std::string &huffmanCode)
{
    //TODO
    // Get codes
    xMap<char, std::string> table([](char& symbol, int capacity)->int {
        return (int)symbol % capacity;
    });
    generateCodes(table);

    // Find the symbol corresponding to the given huffman code
    DLinkedList<char> symbols = table.keys();
    DLinkedList<std::string> codes = table.values();

    std::string decodedStr = "";
    int i = 0;
    while (i < huffmanCode.length()) {
        bool matched = false;
        // Try all codes to find a match at position i
        for (int j = 0; j < codes.size(); ++j) {
            const std::string& code = codes.get(j);
            if (huffmanCode.substr(i, code.length()) == code) {
                decodedStr += symbols.get(j);
                i += code.length();
                matched = true;
                break;
            }
        }
        if (!matched) {
            // No matching code found, break or handle error
            break;
        }
    }
    return decodedStr;
}

template <int treeOrder>
inline InventoryCompressor<treeOrder>::InventoryCompressor(InventoryManager *manager)
{
    //TODO
    invManager = manager;
    huffmanTable = new xMap<char, std::string>([](char& symbol, int capacity)->int {
        return (int)symbol % capacity;
    });
    tree = new HuffmanTree<treeOrder>();
}

template <int treeOrder>
inline InventoryCompressor<treeOrder>::~InventoryCompressor()
{
    //TODO
    delete huffmanTable;
    delete tree;
}

template <int treeOrder>
inline void InventoryCompressor<treeOrder>::buildHuffman()
{
    //TODO
    // (a) Traverse all products to build a frequency table for each character in the string
    // representation of a product using productToString.
    XArrayList<pair<char, int>> symbolsFreqs(0, [](pair<char, int>& a, pair<char, int>& b)->bool {
        return a.first == b.first;
    });

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
inline void InventoryCompressor<treeOrder>::printHuffmanTable() {
    DLinkedList<char> keys = huffmanTable->keys();
    for (char ch : keys) {
        std::cout << "'" << ch << "' : " << huffmanTable->get(ch) << std::endl;
    }
}

template <int treeOrder>
inline std::string InventoryCompressor<treeOrder>::productToString(const List1D<InventoryAttribute> &attributes, const std::string &name)
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
            << std::fixed << std::setprecision(6) << attributes.get(i).value
            << ")";
    }

    return ss.str();
}

template <int treeOrder>
inline std::string InventoryCompressor<treeOrder>::encodeHuffman(const List1D<InventoryAttribute> &attributes, const std::string &name)
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
inline std::string InventoryCompressor<treeOrder>::decodeHuffman(const std::string &huffmanCode, List1D<InventoryAttribute> &attributesOutput, std::string &nameOutput)
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