#ifndef __HUFFMAN_ALG__
#define __HUFFMAN_ALG__

#include "Algorithm.h"

class Huffman : Algorithm {
public:
    Huffman() = default;
    virtual ~Huffman() = default;

    virtual void Encode(std::istream& in, std::ostream& out);
    virtual void Decode(std::istream& in, std::ostream& out);    
};


#endif