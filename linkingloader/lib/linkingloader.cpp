#include "linkingloader.h"

vector<string> Linkingloader::read_file(string filename){
    regex reg(".*\\.obj");

    if(!regex_match(filename, reg)){
        cout << "All file must be obj file." << endl;
        return {};
    }

    ifstream file;
    file.open(filename);
    vector<string> lines;

    if(file.is_open()){
        string line;
        while(file.good()){
            getline(file, line);
            lines.emplace_back(line);
        }
    }
    else{
        cout << "Something worng when reading " << filename << "." << endl;
        return {};
    }
    return lines;
}

string Linkingloader::get_header_name(string line){
    string header_name, tmp;
    stringstream ss(line);
    while(ss >> tmp){
        if(header_name.empty()){
            header_name = tmp.substr(1, tmp.size() - 1);
        }
    }
    return header_name;
}

void Linkingloader::execute(vector<string> argv){
    pass_one(argv);
    print_estab();
    pass_two(argv);
    print_mem_content(argv[1]);
}
