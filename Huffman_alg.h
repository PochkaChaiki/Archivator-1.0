#ifndef __HUFFMAN_ALG__
#define __HUFFMAN_ALG__

#include "Algorithm.h"
#include <queue>
#include <unordered_map>

struct Frequency{
    int freq;
    char byte;

    struct Frequency *left=nullptr, *right=nullptr;
    // friend bool operator> (const Frequency& l, const Frequency& r) {return l.freq>r.freq;};
};

class Huffman : Algorithm {
public:
    Huffman() = default;
    virtual ~Huffman() {};

    virtual void Encode(std::istream& in, std::ostream& out) override {};
    virtual void Decode(std::istream& in, std::ostream& out) override {};   
    void makeFrequencyTree(std::unordered_map<char, int>& countedFrequencies);
    void PrintTree();

private:
    std::unordered_map<char, std::pair<char, char>>freqDictionary;
    void DFS(Frequency* node, char bitmask, char bitsAmount);
};



#endif