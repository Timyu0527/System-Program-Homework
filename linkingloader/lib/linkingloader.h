#ifndef LINKINGLOADER_H
#define LINKINGLOADER_H

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "utility.h"

using namespace std;

class Linkingloader{
    private:
        int progaddr, csaddr, memory_space;
        map<string, int> es_tab;
        string memory_content;
        vector<string> read_file(string filename);
        string get_header_name(string line);
        void process_t_record(string line);
        void process_m_record(string line);
        int pass_one(vector<string> argv);
        void pass_two(vector<string> argv);
    public:
        void execute(vector<string> argv);
        void print_estab();
        void print_mem_content(string addr);
};

#endif
