#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include <cstring>
#include "hm.h"
#include "lzw.h"

using namespace std;

class Zipper {
private:
    vector<string> files;
    string path;
    string real_bin_file;

public:
    Zipper(vector<string> &vec, string p){
        if(vec.size() > 0) 
            files.assign(vec.begin(), vec.end());
        path = p + "\\";
        real_bin_file = path + "binary.zipper";
    }

    void getInfo();   
    void InCompress(string alg);   
    void OutCompress(string binary);

    static string get_file_name(string fn){
        return fn.substr(fn.find_last_of("\\") + 1, fn.size());
    };
};

int digs(double w) {
    int yield = 0;
    while (w > 10) {
        yield++;
        w /= 10;
    };
    return yield + 1;
}

void Zipper::getInfo() {
    basic_string<char> s_info = "";
    remove((this->path+"info.temp").c_str());
    FILE *info = fopen((this->path+"info.temp").c_str(),"a+");
    int bytes_size = 0;
    
    for (vector<string>::iterator itr = this->files.begin(); itr != this->files.end(); ++itr) {
        FILE *f = fopen((*itr).c_str(),"rb");
        if(!f) 
            break;
        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        string name = Zipper::get_file_name(*itr);
        char *m_size = new char[digs(size)];
        itoa(size, m_size, 10);
        fclose(f);

        s_info.append(m_size);
        s_info.append("||");
        s_info.append(name);
        s_info.append("||");
        delete [] m_size;
    }

    bytes_size = s_info.size() + 2;
    char *b_buff = new char[digs(bytes_size)];
    itoa(bytes_size, b_buff, 10);
    if(digs(bytes_size) < 5) 
        fputs(string(5 - digs(bytes_size),'0').c_str(), info);
        
    fputs(b_buff, info);
    fputs("||", info);
    fputs(s_info.c_str(), info);
    delete[] b_buff;
    fclose(info);
}

void Zipper::InCompress(string alg) {
    char byte[1];
    getInfo();
    FILE *f, *main = fopen((this->real_bin_file).c_str(), "wb");
    FILE *info = fopen((this->path + "info.temp").c_str(), "rb");
    
    while(!feof(info)){
        if(fread(byte, 1, 1, info) == 1) 
            fwrite(byte, 1, 1, main);
    }
    fclose(info);
    remove((this->path + "info.temp").c_str());

    for(vector<string>::iterator itr = this->files.begin(); itr != this->files.end(); ++itr){
        f = fopen((*itr).c_str(), "rb");
        if (!f){ 
            cout<<*itr<<" not found!"<<endl; 
            break;
        }
        while (!feof(f)){
            if (fread(byte, 1, 1, f) == 1) 
                fwrite(byte, 1, 1, main);
        }
        cout<<*itr<<" packed to '"<<this->real_bin_file<<"'."<<endl;
        fclose(f);
    }
    fclose(main);

    if (alg == "hfm")
        hfm_encode(this->real_bin_file, this->path);
    else if (alg == "lzw")
        lzw_encode(this->real_bin_file, this->path);

    // remove((this->path + "binary.zipper").c_str());
}


void Zipper::OutCompress(string binary) {
    FILE *encoded = fopen(binary.c_str(), "rb");

    char alg[5];
    fread(alg, 1, 5, encoded);
    fclose(encoded);
    if (strcmp(alg, "hfm||") == 0)
        hfm_decode(binary, this->path);
    else if (strcmp(alg, "lzw||") == 0)
        lzw_decode(binary, this->path);

    FILE *bin = fopen((this->path + "decoded_binary.zipper").c_str(),"rb"); 
    
    char info_block_size[5];
    fread(info_block_size, 1, 5, bin);
    int _sz = atoi(info_block_size);
    char *info_block = new char[_sz];
    fread(info_block, 1, _sz, bin);

    vector<string> tokens;
    char *tok = strtok(info_block, "||");
    int toks = 0;
    while(tok){
        if (strlen(tok) == 0) 
            break;
        tokens.push_back(tok);
        tok = strtok(NULL, "||");
        toks++;
    };

    int files{1};
    if (toks != 1)
        files = toks/2;

    char byte[1];
    for(int i = 0; i < files; ++i){
        char full_path[255];
        strcpy(full_path, this->path.c_str());
        strcat(full_path, tokens[i*2 + 1].c_str());
        int _sz = atoi(tokens[i*2].c_str());
        
        FILE *curr = fopen(full_path, "wb");
        for(int r {1}; r <= _sz; ++r)
            if(fread(byte, 1, 1, bin) == 1) 
                fwrite(byte, 1, 1, curr);

        cout<<"--  '"<<tokens[i*2 + 1].c_str()<<"' unpacked to '"<<this->path<<"' ."<<endl;
        fclose(curr);
    };

    fclose(bin);
    delete[] info_block;
    // remove((this->path + "decoded_binary.zipper").c_str());   
}

int main(int argv, char* argc[]){
    /*/  Suppotred args:
    //
    //    -pack, -unpack, -files, -path, -alg
    //
    /*/
    
    cout<<endl<<"######################## ZIPPER ########################"<<endl<<endl;
    if(argv > 1){
        vector <string> files;
        string path = "", alg = "";
        bool flag_fs = false, flag_path = false, flag_alg = false;
        char type[7];
        memset(type, '0', 7);
        for(int i {1}; i < argv; i++)
        {
            if (strcmp(argc[i], "-pack") == 0) {
                strcpy(type, "pack"); 
                flag_fs = flag_path = false;
            }
            if (strcmp(argc[i], "-unpack") == 0){
                strcpy(type, "unpack"); 
                flag_fs = flag_path = false;
            }
            if (strcmp(argc[i], "-path") == 0) {
                flag_path = true; 
                flag_fs = false; 
                flag_alg = false;
                continue; 
            }
            if (strcmp(argc[i], "-files") == 0) {
                flag_fs = true; 
                flag_path = false; 
                flag_alg = false;
                continue; 
            }
            if (strcmp(argc[i], "-alg") == 0){
                flag_fs = false;
                flag_path = false;
                flag_alg = true;
                continue;
            }
            if (flag_path) 
                path.assign(argc[i]); 
            if (flag_fs) 
                files.push_back(string(argc[i]));
            if (flag_alg)
                alg.assign(argc[i]);

        }
        Zipper *zip = new Zipper(files, path);
        if (strcmp(type, "pack") == 0) 
            zip->InCompress(alg);
        if (strcmp(type, "unpack") == 0) 
            zip->OutCompress(files[0]);
    }
    else 
        cout<<"Parameters -pack/-unpack , -files, -path are compulsary!"<<endl;
    cout<<endl<<"########################################################"<<endl<<endl;
}