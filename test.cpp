#include "Huffman_alg.h"
#include <iostream>
#include <unordered_map>

int main(){
    std::unordered_map<char, int> chars = {
        {'a', 20},
        {'b', 13},
        {'c', 10},
        {'d', 19}
    };
    Huffman exmp;
    exmp.makeFrequencyTree(chars);
    exmp.PrintTree();
}