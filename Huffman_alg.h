#ifndef __HUFFMAN_ALG__
#define __HUFFMAN_ALG__

#include "Algorithm.h"
#include <queue>
#include <unordered_map>

struct Frequency{
    uint64_t freq;
    char byte;

    struct Frequency *left=nullptr, *right=nullptr;
};

class Huffman : Algorithm {
public:
    Huffman() = default;
    virtual ~Huffman() {};

    virtual void Encode(std::istream& in, std::ostream& out) override;
    virtual void Decode(std::istream& in, std::ostream& out) override;   
    // void PrintTree();

private:
    std::unordered_map<char, std::pair<uint16_t, char>>freqDictionary;
    void makeFrequencyTree(std::unordered_map<char, uint64_t>& countedFrequencies);
    void DFS(Frequency* node, uint16_t bitmask, char bitsAmount); // bitmask contains 16 bits because for 4 bits maximum encoded word is 14 bits in length.
    void writeDataToOut(std::istream& in, std::ostream& out);
};



#endif