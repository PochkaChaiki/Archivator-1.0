#include <queue>
#include "Huffman_alg.h"
#include "header.h"


// struct Frequency{
//     int freq{0};
//     std::string chars = "";

//     friend bool operator< (const Frequency& l, const Frequency& r){
//         return l.freq>r.freq;
//     };

//     Frequency& operator= (const Frequency& other){
//         if (this == &other){
//             return *this;
//         } else {
//             chars = other.chars;
//             freq = other.freq;
//             return *this;
//         };
//     };
// };



const size_t BUFFER_SIZE = 1024;

void countFrequencies(std::istream& in, std::unordered_map<char, uint64_t>& frequencies){
    std::vector<char> buffer(BUFFER_SIZE);
    while (!in.eof()){
        in.read(buffer.data(), BUFFER_SIZE);
        for (int i(0); i < in.gcount(); ++i){
            frequencies[buffer[i]]++;
        }
    }
}

void Huffman::DFS(Frequency* node, char bitmap, char bitsAmount){
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
    for (auto& [byte, freq] : frequencies){
        Frequency* node = new Frequency;
        node->byte = byte;
        node->freq = freq;
        chars.push(node);
    }
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
    DFS(chars.top(), 0, 0);
}

void Huffman::writeDataToOut(std::istream& in, std::ostream& out){
    const int BITS_AMOUNT = 8;
    std::vector<char> buffer_in(BUFFER_SIZE);
    std::vector<char> buffer_out(BUFFER_SIZE);
    char byte = 0;
    unsigned char bits_left = BITS_AMOUNT;
    int index_out;
    while (!in.eof()){
        in.read(buffer_in.data(), BUFFER_SIZE);
        index_out = 0;
        for (int i(0); i < in.gcount(); ++i){
            auto [enc_byte, enc_bits_amount] = freqDictionary[buffer_in[i]];
            if (enc_bits_amount <= bits_left){
                byte = byte<<enc_bits_amount;
                byte += enc_byte;
                bits_left -= enc_bits_amount;
            } else {
                byte = byte<<bits_left;
                char mask = 0;
                for (int j(0); j < bits_left; j++)
                    mask = (mask<<1)+1;
                byte += (enc_byte&mask);
                if (index_out == BUFFER_SIZE){
                    out.write(buffer_out.data(), BUFFER_SIZE);
                    index_out = 0;
                }
                buffer_out[index_out] = byte;
                byte = ((enc_byte & (!mask)) >> bits_left);
                bits_left = BITS_AMOUNT - (enc_bits_amount - bits_left);
                index_out++;
            }
        }
        out.write(buffer_out.data(), index_out+1);

    }
    
}
// void Huffman::PrintTree(){
//     for (const auto& [key, value] : freqDictionary){
//         std::cout << key << " "<<(int)value.first << " " << (int)value.second << "\n";
//     }
// }

void Huffman::Encode(std::istream& in, std::ostream& out){
    std::unordered_map<char, uint64_t> freqs;
    countFrequencies(in, freqs);
    makeFrequencyTree(freqs);

    
} 

// void Decode(std::string file, std::string path){
//     std::string temp, encoded_letters = "";
//     std::map <std::string, char> code;
//     FILE * f_to_decode = std::fopen(file.c_str(), "rb");
//     path += "decoded_binary.zipper";
//     FILE *out = std::fopen(path.c_str(),"ab");
//     char buf[1];

//     fseek(f_to_decode, 5, 0);
//     fread(buf, 1, 1, f_to_decode);
//     temp += buf[0];
//     while (buf[0] != '|'){
//         fread(buf, 1, 1, f_to_decode);
//         temp += buf[0];
//     }
//     fread(buf, 1, 1, f_to_decode);
//     int _sz = std::stoi(temp);
//     char * info_block = new char[_sz];
//     fread(info_block, 1, _sz, f_to_decode);

//     char * tok = strtok(info_block, "||");
//     while (tok){
//         char * temptok = tok;
//         tok = strtok(NULL, "||");
//         code[temptok] = std::atoi(tok);
//         tok = strtok(NULL, "||");
//     }

//     while(!feof(f_to_decode)) {
//         if (fread(buf, 1, 1, f_to_decode) == 1){
//             encoded_letters += buf[0];
//             if (code.find(encoded_letters) != code.end()){
//                 buf[0] = code[encoded_letters];
//                 fwrite(buf, 1, 1, out);
//                 encoded_letters = "";
//             };
//         };
//     };

//     fclose(out);
//     fclose(f_to_decode);
// }
