#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "test/tc_xmap.h"
#include "test/tc_heap.h"
#include "test/tc_compressor.h"

using namespace std;

void (*func_ptr[18])() = {
    hashDemo1,
    hashDemo2,
    hashDemo3,
    hashDemo4,
    hashDemo5,
    hashDemo6,
    hashDemo7,
    heapDemo1,
    heapDemo2,
    heapDemo3,
    heapDemo4,
    tc_huffman1001,
    tc_huffman1002,
    tc_huffman1003,
    tc_huffman1004,
    tc_huffman1005,
    tc_compressor1001,
    tc_compressor1002
};

void run(int func_idx)
{
    cout << "----------------------------------------" << endl;
    cout << "Running test case " << func_idx << "..." << endl;
    func_ptr[func_idx]();
    cout << "Test case " << func_idx << " completed." << endl;
    cout << "----------------------------------------" << endl << endl;
}

int main(int argc, char **argv)
{
    run(10);
    
    return 0;
}