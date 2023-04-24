#include "assembler.hpp"

int Sic_xe::format(string token){
    for(int i = 0; i < 3; ++i){
        if(op_tab[i].count(token)) return i;
    }
    return -1;
}

bool Sic_xe::is_dir_or_op(string token){
    return op_tab[0].count(token) |
           op_tab[1].count(token) |
           op_tab[2].count(token) |
           directive.count(token);
}

vector<string> Sic_xe::read_line(string line){
    vector<string> token;

    if(line[0] == '.' || line[0] == '\n') return token;

    stringstream ss(line);
    string f_in;

    while(ss >> f_in){
        token.emplace_back(f_in);
    }
    
    if(token.size() == 1){
        if(!is_dir_or_op(token[0])){
            cout << "Your assembly code has some problem." << endl;
            error = 1;
            return {};
        }
        return {"", token[0], ""};
    }
    else if(token.size() == 2){
        if(is_dir_or_op(token[0]) ||
           is_dir_or_op(token[0].substr(1, token[0].size() - 1))) return {"", token[0], token[1]};
        else if(is_dir_or_op(token[1])) return {token[0], token[1], ""};
        else if(token[0] == "BASE") return {};
        else{
            cout << "Your assembly code has some problem." << endl;
            error = 1;
            return {};
        }
    }
    else if(token.size() == 3){
        if(is_dir_or_op(token[1]) ||
           is_dir_or_op(token[1].substr(1, token[1].size() - 1))) return token;
        else{
            cout << token[1].substr(1, token[1].size() - 1) << endl;
            cout << "Your assembly code has some problem." << endl;
            error = 1;
            return {};
        }
    }

    error = 1;
    return {};
}

long long Sic_xe::compute_bp(long long nixbpe, long long &dist){
    int b_dist = dist + locctr - base;
    if(dist >= -2048 && dist <= 2047){
        nixbpe += (1 << 1);
    }
    else if(0 <= b_dist && b_dist <= 4095){
        nixbpe += (1 << 2);
        dist = b_dist;
    }
    return nixbpe;
}

string Sic_xe::compute_byte(string byte){
    string obj;

    for(int i = 2; i < (int)byte.size() - 1; ++i){
        obj += base_10_llong_to_base_16_string((long long)byte[i], 2);
    }
    
    return obj;
}


bool Sic_xe::read(string filename){
    pass_one(filename);
    return 1;
}

bool Sic_xe::write(string filename){
    string obj = pass_two();
    ofstream file;
    file.open(filename.substr(0, filename.size() - 4) + ".obj");

    if(file.is_open()){
        file << obj << "\n"; 
        file.close();
        return 1;
    }
    else return 0;
}
