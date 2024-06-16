#ifndef __ARCHIVATOR__
#define __ARCHIVATOR__

#include "header.h"

class Archivator {
private:
    std::vector<std::string> files;
    std::string path;
    std::string real_bin_file;

public:
    Archivator(std::vector<std::string> &vec, std::string p){
        if(vec.size() > 0) 
            files.assign(vec.begin(), vec.end());
        path = p + "\\";
        real_bin_file = path + "binary.Archivator";
    }

    void getInfo();   
    void InCompress(std::string alg);   
    void OutCompress(std::string binary);

    static std::string get_file_name(std::string fn){
        return fn.substr(fn.find_last_of("\\") + 1, fn.size());
    };
};





#endif