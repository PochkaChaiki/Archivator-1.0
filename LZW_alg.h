#ifndef __LZW_ALG__
#define __LZW_ALG__

#include "Algorithm.h"


class LZW : Algorithm {
public:
    LZW() = default;
    ~LZW() = default;

    void Encode(std::istream& in, std::ostream& out);
    void Decode(std::istream& in, std::ostream& out);
};

#endif