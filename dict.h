#pragma once
#include <iostream>
#include <map>

using namespace std;

map<string, string> opcodes;

void upt_dict() {
    opcodes.insert(pair<string, string>("libs", "#include <stdio.h>\n#include <stdbool.h>\n#include <stdlib.h>\nint update();\n"));
    opcodes.insert(pair<string, string>("start:", "int main(void) {\n"));
    opcodes.insert(pair<string, string>("update:", "update();\n}\n\nint update() {\nwhile(true) {\n"));
    opcodes.insert(pair<string, string>("INTV", "int \1 = \2;\n"));
    opcodes.insert(pair<string, string>("STRV", "char \1[] = \"\2\";\n"));
    opcodes.insert(pair<string, string>("CHRV", "char \1 = \2;\n"));
    opcodes.insert(pair<string, string>("FLOV", "float \1 = \2;\n"));
    opcodes.insert(pair<string, string>("BOOV", "bool \1 = \2;\n"));
    opcodes.insert(pair<string, string>("COUT", "printf(\"\1\"\2);\n"));
    opcodes.insert(pair<string, string>("CTER", "exit(0);\n"));
}

const int MAX_HELP_PAGES = 2;
void print_dict(char page){
    cout << "======================PAGE " << page << "/" << MAX_HELP_PAGES << "=======================" << endl;
    switch(page){
        case '1':
            cout << "[default functions]" << endl;
            cout << "start:" << "\t\t\t\t" << "Think of it as the \"main\" function. Should always be on the first line." << endl;
            cout << "update:" << "\t\t\t\t" << "Loops code that belongs to it." << endl;
            cout << "[variables]" << endl;
            cout << "INTV [name] [value]" << "\t\t" << "Declares an integer." << endl;
            cout << "STRV [name] [value]" << "\t\t" << "Declares a string." << endl;
            cout << "CHRV [name] [value]" << "\t\t" << "Declares a character." << endl;
            cout << "FLOV [name] [value]" << "\t\t" << "Declares a float." << endl;
            cout << "BOOV [name] [value]" << "\t\t" << "Declares a boolean." << endl;
            break;
        case '2':
            cout << "[console]" << endl;
            cout << "COUT [string] [variable/value]" << "\t" << "Outputs a string into the console. Allows C-like formating." << endl;
            cout << "CINP [string] [variable]" << "\t" << "Waits for user input. (not functional... yet)" << endl;
            cout << "CTER" << "\t\t\t\t" << "Terminates program (can be used to stop the update function)" << endl;
            break;
        default:
            cout << "Page " << page << " doesn't exist.\n";
            break;
    }
}
