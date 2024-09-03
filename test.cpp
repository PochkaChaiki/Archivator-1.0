#include "Huffman_alg.h"
#include <iostream>
#include <unordered_map>
#include <fstream>

int main(){
    std::unordered_map<char, int> chars = {
        {'a', 20},
        {'b', 13},
        {'c', 10},
        {'d', 19}
    };
    std::ifstream in("in.txt", std::ios_base::in | std::ios_base::binary);
    std::ofstream out("out.txt", std::ios_base::out | std::ios_base::binary);
    Huffman exmp;
    exmp.Encode(in, out);
    in.close();
    out.close();

    std::ifstream in_decode("out.txt", std::ios_base::in | std::ios_base::binary);
    std::ofstream out_decode("out_decode.txt", std::ios_base::out | std::ios_base::binary);
    Huffman newExmpl;
    newExmpl.Decode(in_decode, out_decode);
    in_decode.close();
    out_decode.close();
}