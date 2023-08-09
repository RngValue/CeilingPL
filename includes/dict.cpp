#include <iostream>
#include <map>

using std::cout;
using std::string;
using std::endl;
using std::pair;

std::map<string, string> opcodes;

void upt_dict() {
    opcodes.insert(pair<string, string>("libs", "#include <stdio.h>\n#include <stdbool.h>\n#include <stdlib.h>\nint update();\n"));
    //Default functions
    opcodes.insert(pair<string, string>("start:", "int main(void) {\n"));
    opcodes.insert(pair<string, string>("update:", "update();\n}\n\nint update() {\nwhile(true) {\n"));
    //Variables
    opcodes.insert(pair<string, string>("INTV", "int \1 = \2;\n"));
    opcodes.insert(pair<string, string>("STRV", "char \1[] = \"\2\";\n"));
    opcodes.insert(pair<string, string>("CHRV", "char \1 = \2;\n"));
    opcodes.insert(pair<string, string>("FLOV", "float \1 = \2;\n"));
    opcodes.insert(pair<string, string>("BOOV", "bool \1 = \2;\n"));
    //Console
    opcodes.insert(pair<string, string>("COUT", "printf(\"\1\"\2);\n"));
    opcodes.insert(pair<string, string>("CCIN", "fflush(stdin);\nscanf(\"\1\",\2);\n"));
    opcodes.insert(pair<string, string>("CGIN", "fflush(stdin);\ngetc(stdin);\n"));
    opcodes.insert(pair<string, string>("CTER", "exit(0);\n"));
    //Conditionals & loops
    opcodes.insert(pair<string, string>("IF", "if (\1"));
    opcodes.insert(pair<string, string>("ELIF", "} else if (\1"));
    opcodes.insert(pair<string, string>("ELSE", "} else {\n"));
    opcodes.insert(pair<string, string>("FOR", "for (int \1 = 0; \1<=\2; \1++\3"));
    opcodes.insert(pair<string, string>("WHILE", "while (\1"));
    opcodes.insert(pair<string, string>("BREAK", "break;\n"));
    opcodes.insert(pair<string, string>("THEN", ") {\n"));
    opcodes.insert(pair<string, string>("END", "}\n"));
}

//Documentation
const int MAX_HELP_PAGES = 3;
void print_dict(char page){
    cout << "======================PAGE " << page << "/" << MAX_HELP_PAGES << "=======================" << endl;
    switch(page){
        case '1':
            cout << "[default functions]" << endl;
            cout << "start:" << "\t\t\t\t" << "Think of it as the \"main\" function." << endl;
            cout << "update:" << "\t\t\t\t" << "Loops code that belongs to it." << endl;
            cout << "[variables]" << endl;
            cout << "INTV [name] [value]" << "\t\t" << "Declares an integer." << endl;
            cout << "STRV [name] [value]" << "\t\t" << "Declares a string. (Which is just an array of characters)" << endl;
            cout << "CHRV [name] [value]" << "\t\t" << "Declares a character." << endl;
            cout << "FLOV [name] [value]" << "\t\t" << "Declares a float." << endl;
            cout << "BOOV [name] [value]" << "\t\t" << "Declares a boolean." << endl;
            break;
        case '2':
            cout << "[console]" << endl;
            cout << "COUT [string] [variable/value]" << "\t" << "Outputs a string into the console. Allows C-like formating." << endl;
            cout << "CCIN [string] [variable]" << "\t" << "Checks user input and stores it into a variable. (Just like scanf in C)" << endl;
            cout << "CGIN" << "\t\t\t\t" << "Waits for user to press enter." << endl;
            cout << "CTER" << "\t\t\t\t" << "Terminates program." << endl;
            break;
        case '3':
            cout << "[conditions & loops]" << endl;
            cout << "IF [condition] THEN" << "\t\t" << "If statement." << endl;
            cout << "ELIF [condition] THEN" << "\t\t" << "Else if statement." << endl;
            cout << "ELSE" << "\t\t\t\t" << "Else statement." << endl;
            cout << "FOR [itirator] IN [range] THEN" << "\t" << "For loop." << endl;
            cout << "WHILE [condition] THEN" << "\t\t" << "While loop." << endl;
            cout << "END" << "\t\t\t\t" << "Marks an end of a condition/loop." << endl;
            break;
        default:
            cout << "Page " << page << " doesn't exist.\n";
            break;
    }
}
