#include "Huffman_alg.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){

    std::ifstream in(argv[1], std::ios_base::in | std::ios_base::binary);
    std::ofstream out(argv[2], std::ios_base::out | std::ios_base::binary);
    Huffman exmp;
    exmp.Encode(in, out);
    in.close();
    out.close();

    std::ifstream in_decode(argv[2], std::ios_base::in | std::ios_base::binary);
    std::ofstream out_decode(argv[3], std::ios_base::out | std::ios_base::binary);
    Huffman newExmpl;
    newExmpl.Decode(in_decode, out_decode);
    in_decode.close();
    out_decode.close();
}