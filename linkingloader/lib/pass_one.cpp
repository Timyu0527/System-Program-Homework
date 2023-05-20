#include "linkingloader.h"
#include "utility.h"

void Linkingloader::print_estab(){
    for(pair<string, int> i : es_tab){
        cout << i.first << ": 0x" << base_10_llong_to_base_16_string(i.second, 4) << endl;
    }
}

int Linkingloader::pass_one(vector<string> argv){
    int memory_space_len = 0;
    progaddr = base_16_string_to_base_10_llong(argv[1]);
    csaddr = progaddr;
    // cout << progaddr << endl;
    // cout << (int)argv.size() << endl;
    for(int i = 2; i < (int)argv.size(); ++i){
        vector<string> lines = read_file(argv[i]);
        string csname = get_header_name(lines[0]);
        string start;
        for(int j = 13; j < 19; ++j) start += lines[0][j];
        int cslen = base_16_string_to_base_10_llong(start);

        memory_space_len += cslen;
        es_tab[csname] = csaddr;
        
        // cout << lines.size() << endl;
        for(int j = 1; j < (int)lines.size(); ++j){
            // cout << lines[j][0] << endl;
            if(lines[j][0] != 'D') continue; 

            int n = ((int)lines[j].size() - 1) / 12;
            // cout << n << endl;
            for(int k = 0; k < n; ++k){
                string name, saddr;
                for(int l = 1 + 12 * k; l < 7 + 12 * k; ++l){
                    if(lines[j][l] != ' ') name += lines[j][l];
                    saddr += lines[j][l + 6];
                }
                int addr = base_16_string_to_base_10_llong(saddr);
                es_tab[name] = csaddr + addr;
            }
        }
        csaddr += cslen;
    } 

    memory_space = memory_space_len;
    for(int i = 0; i < memory_space << 1; ++i){
        memory_content += ".";
    }

    return memory_space_len;
}


