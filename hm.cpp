#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <cstring>

struct Frequency{
    int freq{0};
    std::string chars = "";

    friend bool operator< (const Frequency& l, const Frequency& r){
        return l.freq>r.freq;
    };

    Frequency& operator= (const Frequency& other){
        if (this == &other){
            return *this;
        } else {
            chars = other.chars;
            freq = other.freq;
            return *this;
        };
    };
};

void hfm_encode(std::string file, std::string path){
    std::map <char, int> dict;
    std::map <char, std::string> result;
    FILE *f_to_encode = std::fopen(file.c_str(), "rb");
    char buf[1];
    
    while(!feof(f_to_encode)){
        if(fread(buf, 1, 1, f_to_encode)==1) {
            dict[buf[0]] += 1;
        };
    };
    fclose(f_to_encode);

    std::priority_queue <Frequency> tree;
    for (auto& it: dict)
        tree.push({it.second, std::string(1, it.first)});
    
    Frequency first, second;
    first = tree.top();
    if (tree.size() == 1)
        result[first.chars[0]] = "0";
    
    while (tree.size()>=2){
        first = tree.top();
        tree.pop();
        second = tree.top();
        tree.pop();

        for (auto& c: first.chars)
            result[c] = "1" + result[c];
        
        for (auto& c: second.chars)
            result[c] = "0" + result[c];

        tree.push({first.freq+second.freq, first.chars+second.chars});
    };

    std::string s_info ="";
    for (auto c : result)
        s_info += c.second + "||" + std::to_string((int)c.first) + "||";
    s_info = "hfm||" + std::to_string(s_info.length()) + "||" + s_info;

    path += "binary_encoded_hfm.zipper";
    FILE * out= std::fopen(path.c_str(), "ab");
    f_to_encode = std::fopen(file.c_str(), "rb");
    fputs(s_info.c_str(), out);
    
    while(!feof(f_to_encode)){
        if(fread(buf, 1, 1, f_to_encode) == 1) {
            fputs(result[buf[0]].c_str(), out);
        };
    };

    fclose(out);
    fclose(f_to_encode);
} 

void hfm_decode(std::string file, std::string path){
    std::string temp, encoded_letters = "";
    std::map <std::string, char> code;
    FILE * f_to_decode = std::fopen(file.c_str(), "rb");
    path += "decoded_binary.zipper";
    FILE *out = std::fopen(path.c_str(),"ab");
    char buf[1];

    fseek(f_to_decode, 5, 0);
    fread(buf, 1, 1, f_to_decode);
    temp += buf[0];
    while (buf[0] != '|'){
        fread(buf, 1, 1, f_to_decode);
        temp += buf[0];
    }
    fread(buf, 1, 1, f_to_decode);
    int _sz = std::stoi(temp);
    char * info_block = new char[_sz];
    fread(info_block, 1, _sz, f_to_decode);

    char * tok = strtok(info_block, "||");
    while (tok){
        char * temptok = tok;
        tok = strtok(NULL, "||");
        code[temptok] = std::atoi(tok);
        tok = strtok(NULL, "||");
    }

    while(!feof(f_to_decode)) {
        if (fread(buf, 1, 1, f_to_decode) == 1){
            encoded_letters += buf[0];
            if (code.find(encoded_letters) != code.end()){
                buf[0] = code[encoded_letters];
                fwrite(buf, 1, 1, out);
                encoded_letters = "";
            };
        };
    };

    fclose(out);
    fclose(f_to_decode);
}
