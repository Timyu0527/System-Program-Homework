#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <cstdio>
#include <iomanip>

#include "utility.hpp"

using namespace std;

class Sic_xe{
    private:
        map<string, long long> op_tab[3], sym_tab;
        map<string, string> reg_tab;
        set<string> directive;
        vector<vector<string> > lines;
        long long start, locctr, prog_len, base;
        bool error;
        int format(string token);
        bool is_dir_or_op(string token);
        vector<string> read_line(string line);
        long long compute_bp(long long nixbpe, long long &dist);
        string compute_byte(string byte);
        string m_record();
        string t_record(string f_line, long long start, long long len);
        bool pass_one(string filename);
        string pass_two();
    public:
        static Sic_xe sic_xe;
        Sic_xe();
        bool read(string filename);
        bool write(string filename);
};

#endif
