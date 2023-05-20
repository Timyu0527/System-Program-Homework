#include "linkingloader.h"
#include "utility.h"

void Linkingloader::print_mem_content(string addr){
    progaddr = base_16_string_to_base_10_llong(addr);
    for(int i = 0; i < (int)memory_content.size(); ++i){
        if(i % 32 == 0) cout << endl << base_10_llong_to_base_16_string(progaddr + i / 2, 4);
        cout << " " << memory_content[i];
    }
    cout << endl;
}

void Linkingloader::process_t_record(string line){
    int addr = base_16_string_to_base_10_llong(line.substr(1, 6));
    addr += csaddr;

    int mem_arr_idx = (addr - progaddr) << 1;
    int len = base_16_string_to_base_10_llong(line.substr(7, 2));

    for(int i = 0; i < len << 1; ++i, ++mem_arr_idx){
        memory_content[mem_arr_idx] = line[9 + i];
    }
}

void Linkingloader::process_m_record(string line){
    // cout << line.substr(1, 6) << endl;
    // cout << "csaddr: " << csaddr << endl;
    int addr = base_16_string_to_base_10_llong(line.substr(1, 6));
    addr += csaddr;
    // cout << "progaddr: " << progaddr << endl;
    // cout << "addr: " << addr << endl;

    int mem_arr_idx = (addr - progaddr) << 1;
    // cout << "mem_arr_idx: " << mem_arr_idx << endl;
    int ml = base_16_string_to_base_10_llong(line.substr(7, 2));

    if(ml == 5) ++mem_arr_idx;

    // cout << "ml: " << ml << endl;

    string cur_mem_val = memory_content.substr(mem_arr_idx, ml);
    // cout << "cur_mem_val: " << cur_mem_val << endl;

    int value = base_16_string_to_base_10_llong(cur_mem_val, ml << 2);
    string token(line.begin() + 10, line.end());
    // cout << "token: " << token << " " << es_tab[token] << endl;
    // cout << "b value: " << value << endl;
    if(line[9] == '+'){
        value += es_tab[token];
    }
    else{
        value -= es_tab[token];
    }
    // cout << "a value: " << value << endl;
    string result = base_10_llong_to_base_16_string(value, ml);

    for(int i = 0; i < ml; ++i, ++mem_arr_idx){
        memory_content[mem_arr_idx] = result[i];
    }
    // cout << "result: " << result << endl;
}

void Linkingloader::pass_two(vector<string> argv){
    progaddr = base_16_string_to_base_10_llong(argv[1]);
    // cout << progaddr << endl;
    csaddr = progaddr;
    // cout << csaddr << endl;

    for(int i = 2; i < (int)argv.size(); ++i){
        vector<string> lines = read_file(argv[i]);
        int cslen = base_16_string_to_base_10_llong(lines[0].substr(13, 6));

        for(int j = 1; j < (int)lines.size(); ++j){
            if(lines[j][0] == 'T'){
                process_t_record(lines[j]);
            }
            else if(lines[j][0] == 'M'){
                process_m_record(lines[j]);
            }
        }
        csaddr += cslen;
    }
}
