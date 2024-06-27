// #include <iostream>
// #include <unordered_map>
// #include <map>
// #include <iterator>
// #include <string>
// #include <fstream>
// #include <vector>
// #include <cstring>
#include "LZW_alg.h"
#include "header.h"


void LZW::Encode(std::istream& in, std::ostream& out){
    
}

void lzw_encode(std::string file, std::string path){
    std::string str = "", shitstr = "", s_info = "";
    
    std::map <std::string, int> dict;
    for (int i {0}; i < 256; ++i){
        str += (char)i;
        dict[str] = i;
        str = "";
    };

    FILE * f_to_encode = fopen(file.c_str(), "rb");
    path += "binary_encoded_lzw.zipper";
    FILE * out = std::fopen(path.c_str(), "ab");

    int i {256};
    char buf[1];
    char * info_block_size = (char*)malloc(4);
    info_block_size[5] = '\0';

    fread(info_block_size, 1, 5, f_to_encode);
    int _sz = std::stoi(std::string(info_block_size));    
    fseek(f_to_encode, _sz, SEEK_CUR);
    fputs("lzw||", out);
    fseek(f_to_encode, 0, 0);
    fread(info_block_size, 1, 5, f_to_encode);
    fputs(info_block_size, out);
    
    free(info_block_size);

    char info_block[_sz];
    fread(info_block, 1, _sz, f_to_encode);
    fputs(info_block, out);

    fread(buf, 1, 1, f_to_encode);
    str = "";
    str += buf[0];
    while(!feof(f_to_encode)){
        if (fread(buf, 1, 1, f_to_encode) == 1){
            if (dict.find(str + buf[0]) == dict.end()){
                fputs(std::to_string(dict[str]).c_str(), out);
                fputs("|", out);
                dict.insert({str + buf[0], i});
                str = "";
                str += buf[0];
                ++i;
                continue;
            };
            str += buf[0];
        };
    };
    fputs(std::to_string(dict[str]).c_str(), out);
    fputs("|", out);

    fclose(f_to_encode);
    fclose(out);
}

void lzw_decode(std::string file, std::string path){

    std::map <int, std::string> code;
    std::string temp = "", single_code = "", next_code = "";
    
    FILE * f_to_decode = std::fopen(file.c_str(), "rb");
    path += "decoded_binary.zipper";
    FILE * out = std::fopen(path.c_str(), "ab");
    
    for(int i{0}; i < 256; ++i){
        temp += (char)i;
        code[i] = temp;
        temp = "";
    };

    char buf[1];
    int i{256};

    fseek(f_to_decode, 5, 0);
    fread(buf, 1, 1, f_to_decode);
    temp += buf[0];
    while (buf[0] != '|'){
        fread(buf, 1, 1, f_to_decode);
        temp += buf[0];
    };
    fread(buf, 1, 1, f_to_decode);
    temp += buf[0];
    fputs(temp.c_str(), out);
    int _sz = std::stoi(temp) - 2;
    char * info_block = new char[_sz];
    fread(info_block, 1, _sz, f_to_decode);
    for (int j{0}; j < _sz; ++j){
        buf[0] = info_block[j];
        fwrite(buf, 1, 1, out);
    };
    delete[] info_block;

    fread(buf, 1, 1, f_to_decode);
    while (buf[0]!='|'){
        single_code += buf[0];
        fread(buf, 1, 1, f_to_decode);
    };
    int index = atoi(single_code.c_str());
    int old = index;
    fputs(code[index].c_str(), out);
    next_code = code[index][0];
    single_code = "";
    while (!feof(f_to_decode)){
        if (fread(buf, 1, 1, f_to_decode) == 1) {
            if (buf[0] == '|'){
                index = atoi(single_code.c_str());
                if (code.find(index) == code.end()){
                    single_code += code[old];
                    single_code += next_code;
                    fputs((single_code).c_str(), out);
                } else {
                    fputs(code[index].c_str(), out);
                    single_code = code[index]; //Here and above I use single_code not as code gatherer, but as string keeper for the moment (yeah, I know that this is a shitcoding)
                };
                next_code = "";
                next_code = single_code[0];
                code.insert({i++, code[old] + next_code});
                single_code = "";
                old = index;
                continue;
            } else 
                single_code += buf[0];
        }
    }
    fclose(f_to_decode);
    fclose(out);
}