#include "assembler.hpp"

string Sic_xe::m_record(){
    string obj = "M" + base_10_llong_to_base_16_string(locctr - 3, 6) + "05\n";
    return obj;
}

string Sic_xe::t_record(string f_line, long long start, long long len){
    f_line = "T" + base_10_llong_to_base_16_string(start, 6) +
             base_10_llong_to_base_16_string(len, 2) +
             f_line + "\n";

    return f_line;
}

string Sic_xe::pass_two(){
    string f_out, f_line, r, m_record_s;
    long long tstart = start;
    locctr = start;

    for(vector<string> line : lines){
        if(line[1] == "START"){
            string address;

            for(int i = 0; i < 6 - (int)line[2].size(); ++i) address += "0";
            f_out += "H" + line[0] + "  " + address + line[2] +
                     base_10_llong_to_base_16_string(prog_len, 6) + "\n";

            continue;
        }
        else if(line[1] == "END"){
            f_line = t_record(f_line, tstart, f_line.size() / 2);
            f_out += f_line + m_record_s;
            f_out += "E" + base_10_llong_to_base_16_string(start, 6);
        }
        else if(format(line[1]) == 0){
            locctr += 1;

            string obj;

            obj = base_10_llong_to_base_16_string(op_tab[0][line[1]], 1);

            // cout << line[1] << ": "; 
            // cout << hex << obj << dec << endl;
            f_line += obj;
        }
        else if(format(line[1]) == 1){
            locctr += 2;

            stringstream ss(line[2]);
            string reg, obj;

            obj += base_10_llong_to_base_16_string(op_tab[1][line[1]], 2);

            while(getline(ss, reg, ',')){
                obj += reg_tab[reg];
            }

            for(int i = 0; i < 4 - (int)obj.size(); ++i) obj += "0";

            // cout << line[1] << ": "; 
            // cout << hex << obj << dec << endl;
            f_line += obj;
        }
        else if(format(line[1]) == 2 ||
                format(line[1].substr(1, line[1].size() - 1)) == 2){
            long long nixbpe = 0, disp = 0, address = 0;
            unsigned long long obj = 0;
            vector<string> tokens;
            stringstream ss(line[2]);
            string token;

            if(line[1][0] == '+'){
                locctr += 4;

                obj += (op_tab[2][line[1].substr(1, line[1].size() - 1)] << 24);
                nixbpe = 1;

                while(getline(ss, token, ',')){
                    tokens.emplace_back(token);
                }

                if(tokens.size() > 1) if(tokens[1] == "X") nixbpe += (1 << 3);

                if(tokens[0][0] == '@'){
                    token = tokens[0].substr(1, tokens[0].size() - 1);
                    address = sym_tab[token];
                    nixbpe += (1 << 5);
                    m_record_s += m_record();
                }
                else if(tokens[0][0] == '#'){
                    token = tokens[0].substr(1, tokens[0].size() - 1);
                    nixbpe += (1 << 4);

                    if(sym_tab.count(token)) address = sym_tab[token];
                    else address = atoll(token.c_str());
                }
                else{
                    nixbpe += (1 << 4) + (1 << 5);
                    address = sym_tab[token];
                    m_record_s += m_record();
                }

                obj += (nixbpe << 20) + address;

                f_line += base_10_llong_to_base_16_string(obj, 8);
            }
            else{
                locctr += 3;

                int check_b = 1 << 2;

                if(line[1] == "LDB"){
                    token = line[2].substr(1, line[2].size() - 1);
                    if(line[2][0] == '#'){
                        if(sym_tab.count(token)) base = sym_tab[token];
                        else base = base_16_string_to_base_10_llong(token);
                    }
                }

                obj += (op_tab[2][line[1]] << 16);

                while(getline(ss, token, ',')){
                    tokens.emplace_back(token);
                }

                unsigned short fix = (1 << 15) + (1 << 14) + (1 << 13) + (1 << 12);
            
                if((int)tokens.size() > 1){
                    if(tokens[1] == "X") nixbpe += (1 << 3);
                }
                else if(tokens.empty()){
                    nixbpe += (1 << 4) + (1 << 5);
                    obj += (nixbpe << 12);
                    goto write;
                }

                if(tokens[0][0] == '@'){
                    token = token.substr(1, token.size() - 1);

                    address = sym_tab[token];
                    nixbpe += (1 << 5);
                    disp = address - locctr;

                    nixbpe = compute_bp(nixbpe, disp);
                }
                else if(token[0] == '#'){
                    token = token.substr(1, token.size() - 1);
                    nixbpe += (1 << 4);

                    if(sym_tab.count(token)){
                        address = sym_tab[token];
                        disp = address - locctr;

                        nixbpe = compute_bp(nixbpe, disp);
                    }
                    else{
                        disp = base_16_string_to_base_10_llong(token);
                    }
                }
                else{
                    address = sym_tab[tokens[0]];
                    nixbpe += (1 << 4) + (1 << 5);
                    disp = address - locctr;

                    nixbpe = compute_bp(nixbpe, disp);
                }

                if((nixbpe & check_b) || disp >= 0) fix = 0;
                obj += (nixbpe << 12) + (unsigned short)disp - fix;

                write:
                    f_line += base_10_llong_to_base_16_string(obj, 6);
                // cout << f_line << endl;
            }
            // cout << line[1] << ": ";
            // cout << hex << obj << dec << endl;
        }
        // cout << (locctr - tstart + inst_size > 30 )<< endl;
        // cout << "f_line: "<< f_line << endl;
        // cout << "tstart: " << tstart << endl;
        // cout << "locctr: " << locctr << endl;
        if(locctr - tstart >= 30 || line[1] == "RESW" || line[1] == "RESB" ){
            int rlen = 0;

            if(line[1] == "RESW" || line[1] == "RESB"){
                if(line[1] == "RESW") locctr += atoll(line[2].c_str()) * 3;
                else if(line[1] == "RESB") locctr += atoll(line[2].c_str());

                if(f_line.empty()){
                    tstart = locctr;
                    continue;
                }
            }
            else{
                // cout << "f_line size: "<< f_line.size()  << endl;
                rlen = (locctr - tstart - 30) << 1;
                // cout << "rlen: " << rlen << endl;
                // cout << "tstart: " << tstart << endl;
                // cout << "locctr: " << locctr << endl;
                r = f_line.substr(f_line.size() - rlen, rlen);
            }
            
            f_line = t_record(f_line.substr(0, f_line.size() - rlen),
                              tstart, (f_line.size() - rlen) / 2);

            tstart = locctr - (rlen >> 1);
            f_out += f_line;

            if(line[1] == "RESW" || line[1] == "RESB") f_line.clear();
            else f_line = r;
        }
        else if(directive.count(line[1])){
            stringstream ss;
            string obj;

            if(line[1] == "WORD"){
                locctr += 3;
                ss << setfill('0') << setw(6) << line[2];
                ss >> obj;
            }
            else if(line[1] == "BYTE"){
                if(line[2][0] == 'C'){
                    locctr += line[2].size() - 3;
                    obj = compute_byte(line[2]);
                }
                else if(line[2][0] == 'X'){
                    locctr += (line[2].size() - 3) / 2;
                    obj = line[2].substr(2, line[2].size() - 3);
                }
            }
            // cout << line[1] << ": ";
            // cout << hex << obj << dec << endl;
            f_line += obj;
        }
    }
    // cout << f_out << endl;
    return f_out;
}
