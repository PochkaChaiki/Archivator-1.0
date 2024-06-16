#include "header.h"
#include <clocale>
#include <cstring>
#include "hm.h"
#include "lzw.h"
#include "archivator.h"

void process_args(int argv, char* argc[]);



int main(int argv, char* argc[]){
    /*/  Suppotred args:
    //
    //    -pack, -unpack, -files, -path, -alg
    //
    /*/
    
    std::cout<<"\n######################## ARCHIVATOR ########################\n\n";
    if(argv > 1){
        process_args(argv, argc);
    }
    else 
        std::cout<<"Parameters -pack/-unpack , -files, -path are compulsary!\n";
    std::cout<<"\n########################################################\n\n";
}





void process_args(int argv, char* argc[]){

    std::string arg = "", mode = "", path = "", alg = "";
    bool file_flag (0);
    std::vector<std::string> files;

    mode = argc[1];
    for (int i(2); i < argv; ++i){
        arg = argc[i];

        if (arg == "-files") {
            file_flag = 1;
            continue;
        }
        if (file_flag)
            files.push_back(arg);

        if (arg == "-path"){
            file_flag = 0;
            path = argc[i+1];
            ++i;
        }
        if (arg == "-alg"){
            file_flag = 0;
            alg = argc[i+1];
            ++i;
        }
    }
    
    Archivator *zip = new Archivator(files, path);
    if (mode == "-pack") 
        zip->InCompress(alg);
    if (mode == "-unpack") 
        zip->OutCompress(files[0]);
}