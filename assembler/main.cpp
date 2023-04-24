#include "lib/assembler.hpp"
#include <iostream>

using namespace std;

/*README

    SIC/XE Assembler

        Environment

            Platform: Ubuntu 22.04 LTS <br>
            Language: C++17

        Usage

            Build
            Build the assembler.
            ```
            make
            ```

            Development
            Build and run the assembler.
            ```
            make dev
            ```

            Clean object file
            Remove all object file and a.out.
            ```
            make clean
            ```

            Run
            ```
            ./a.out
            <source file>
            ```

*/

Sic_xe Sic_xe::sic_xe;

int main(){
    string filename;
    cin >> filename;

    Sic_xe::sic_xe.read(filename);
    Sic_xe::sic_xe.write(filename);
    return 0;
}
