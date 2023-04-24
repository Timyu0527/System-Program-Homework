#include "assembler.hpp"

bool Sic_xe::pass_one(string filename){
    regex reg(".*\\.asm");

    if(!regex_match(filename, reg)){
        cout << "This file is not asm file." << endl;
        return 0;
    }
    
    ifstream file;
    file.open(filename);
    
    if(file.is_open()){
        string instruction;
        while(file.good()){

            getline(file, instruction);
            vector<string> token(read_line(instruction));
            if(error) break;

            if(token.empty()) continue;

            lines.emplace_back(token);

            if(token[1] == "START"){
                 start = base_16_string_to_base_10_llong(token[2]);
                 locctr = start;
                 continue;
            }
            else if(token[1] == "END"){
                prog_len = locctr - start;
                continue;
            }

            if(token[0] != ""){
                if(sym_tab.count(token[0])){
                    cout << "Your assembly code has problem.";
                    error = 1;
                    break;
                }
                sym_tab[token[0]] = locctr;
            }

            // cout << setfill('0') << setw(5) << hex << locctr << "   " << instruction << endl;
            if(format(token[1]) == 0) locctr += 1;
            else if(format(token[1]) == 1) locctr += 2;
            else if(format(token[1]) == 2 ||
                    format(token[1].substr(1, token[1].size() - 1)) == 2) {
                if(token[1][0] == '+') locctr += 4;
                else locctr += 3;
            }
            else if(token[1] == "WORD") locctr += 3;
            else if(token[1] == "RESW") locctr += atoll(token[2].c_str()) * 3;
            else if(token[1] == "RESB") locctr += atoll(token[2].c_str());
            else if(token[1] == "BYTE"){
                if(token[2][0] == 'C') locctr += token[2].size() - 3;
                else if(token[2][0] == 'X') locctr += (token[2].size() - 3) / 2;
            }
        }
    }
    // for(auto line:lines){
    //     for(auto token:line){
    //         cout << token << "  ";
    //     }
    //     cout << endl;
    // }
    // for(auto [i, j]:sym_tab){
    //     cout << i << ": " << hex << j << endl;
    // }
    file.close(); 
    return !error;
}
