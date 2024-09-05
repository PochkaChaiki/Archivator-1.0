#include <queue>
#include "Huffman_alg.h"
#include "header.h"


const size_t BUFFER_SIZE = 1024;
const char HALF_BYTE_MASK = 15;

void countFrequencies(std::istream& in, std::unordered_map<char, uint64_t>& frequencies){
    std::vector<char> buffer(BUFFER_SIZE);

    while (!in.eof()){
        in.read(buffer.data(), BUFFER_SIZE);
        int n = in.gcount();
        for (int i(0); i < n; ++i){
            char read_byte = 0;
            read_byte = (buffer[i]&HALF_BYTE_MASK);
            frequencies[read_byte]++;
            read_byte = ((buffer[i]>>4)&HALF_BYTE_MASK);
            frequencies[read_byte]++;
        }
    }
}

void Huffman::DFS(Frequency* node, uint16_t bitmap, char bitsAmount){
    if (!(node->left || node->right)){
        freqDictionary.insert({node->byte, std::make_pair(bitmap, bitsAmount)});
        return;
    }
    DFS(node->left, ((bitmap<<1)+0), bitsAmount+1);
    DFS(node->right, ((bitmap<<1)+1), bitsAmount+1);
}


void Huffman::makeFrequencyTree(std::unordered_map<char, uint64_t>& frequencies){
    struct customLess{
        bool operator()(const Frequency* a, const Frequency* b) const {return a->freq > b->freq;}
    };
    std::priority_queue<Frequency*, std::vector<Frequency*>, customLess> chars;  
    // Pushing values to priority_queue to make a tree later
    for (auto& [byte, freq] : frequencies){
        Frequency* node = new Frequency;
        node->byte = byte;
        node->freq = freq;
        chars.push(node);
    }
    // Processing values in queue and making tree
    while (chars.size() != 1){
        Frequency *nodeLeft, *nodeRight, *parentNode; 
        nodeLeft = chars.top();
        chars.pop();
        nodeRight = chars.top();
        chars.pop();
        parentNode = new Frequency;
        parentNode->freq = nodeLeft->freq + nodeRight->freq;
        parentNode->left = nodeLeft;
        parentNode->right = nodeRight;
        chars.push(parentNode);
    }
    // Making a complete dictionary
    DFS(chars.top(), 0, 0);
}

void Huffman::writeDataToOut(std::istream& in, std::ostream& out){
    in.clear();
    in.seekg(0, std::ios_base::beg);
    const int BITS_AMOUNT = 8;
    std::vector<char> buffer_in(BUFFER_SIZE);
    char byte = 0;
    unsigned char bits_left = BITS_AMOUNT;
    
    // Writing dictionary to out file
    out.put(static_cast<char>(freqDictionary.size()));
    for (const auto& [key, value] : freqDictionary){
        out.put(key);
        char part_of_bitmap = (value.first>>8);
        out.put(part_of_bitmap);
        part_of_bitmap = value.first&255;
        out.put(part_of_bitmap);
        out.put(value.second);
    }
    
    // Actual file compressing
    while (!in.eof()){
        in.read(buffer_in.data(), BUFFER_SIZE);
        for (int i(0); i < in.gcount(); ++i){
            for (int part_of_byte(1); part_of_byte >= 0; part_of_byte--){
                char byte_in = (buffer_in[i] >> 4*part_of_byte) & 15;
                auto [enc_byte, enc_bits_amount] = freqDictionary[byte_in];
                if (enc_bits_amount <= bits_left){
                    byte = byte<<enc_bits_amount;
                    byte |= enc_byte;
                    bits_left -= enc_bits_amount;
                } else {
                    byte = byte<<bits_left;
                    char add_bits = enc_byte>>(enc_bits_amount-bits_left);
                    byte |= add_bits;
                    out.put(byte);
                    char mask = 0;
                    for (int bit(0); bit < enc_bits_amount-bits_left; bit++){
                        mask = (mask<<1)+1;
                    }
                    byte = enc_byte & mask;
                    bits_left = BITS_AMOUNT - (enc_bits_amount - bits_left);
                }
            }
        }
    }
    out.put(byte<<bits_left);
    out.put(static_cast<char>(bits_left));
    
}

void Huffman::Encode(std::istream& in, std::ostream& out){
    std::unordered_map<char, uint64_t> freqs;
    countFrequencies(in, freqs);
    makeFrequencyTree(freqs);
    writeDataToOut(in, out);
} 

void Huffman::Decode(std::istream& in, std::ostream& out){
    char size_of_dictionary = 0;
    in.get(size_of_dictionary);
    Frequency* root = new Frequency;
    Frequency* node;
    
    // Reading word dictionary and making a tree for decompression 
    for (int i(0); i < size_of_dictionary; ++i){
        node = root;
        char half_byte, byte, bitAmount;
        uint16_t bitmap(0);
        
        in.get(half_byte);
        for (int i(0); i < 2; ++i){
            bitmap <<= 8;
            in.get(byte);
            bitmap |= byte;
        }
        in.get(bitAmount);

        for (int tree_index(bitAmount-1); tree_index >= 0; tree_index--){
            char mask = (1<<tree_index);
            char way = bitmap&mask;
            if (way != 0){
                if (node->right == nullptr)
                    node->right = new Frequency;
                node = node->right;
            } else {
                if (node->left == nullptr)
                    node->left = new Frequency;
                node = node->left;
            }
            if (tree_index == 0){
                node->byte = half_byte;
            }
        }
    }   

    node = root;
    char byte_in(0), second_byte(0), byte_out(0);
    bool byte_out_first_part(false);
    int bits_limit = 0;
    in.get(byte_in);
    while (!in.eof()){
    /* 
     * second byte is added to get know, 
     * if it is an end of file so then second_byte is a number 
     * of unnessesary bits
     */
        in.get(second_byte);
        
        if (in.eof()){
            bits_limit = static_cast<int>(second_byte);
        }
        for (int bit(7); bit >= bits_limit; bit--){
            char mask = 1<<bit;
            char way = byte_in&mask;
            if (way != 0){
                node = node->right;
                if (node->right == nullptr){
                    byte_out <<= 4;
                    byte_out |= node->byte;
                    node = root;
                    if (byte_out_first_part){
                        out.put(node->byte);
                        byte_out_first_part = false;
                        continue;
                    }
                    byte_out_first_part = true;
                }
            } else {
                node = node->left;
                if (node->left == nullptr){
                    byte_out <<= 4;
                    byte_out |= node->byte;
                    node = root;
                    if (byte_out_first_part){
                        out.put(node->byte);
                        byte_out_first_part = false;
                        continue;
                    }
                    byte_out_first_part = true;
                }
            }
        }
        byte_in = second_byte;
    }
}
