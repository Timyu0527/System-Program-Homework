#include "lib/linkingloader.h"

Linkingloader linkingloader;
using namespace std;
int main(int argc, char *argv[]){
    vector<string> vargv;
    for(int i = 0; i < argc; ++i){
        vargv.emplace_back(argv[i]);
    }

    linkingloader.execute(vargv);
    return 0;
}
