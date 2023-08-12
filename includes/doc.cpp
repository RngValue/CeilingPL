#include <iostream>

using std::cout;

//Help
void show_help() {
    cout << "Please, use any of these arguments:\n";
    cout << "\"-o [.ceil file] [output file]\"\tCompiles your code to an executable file\n";
    cout << "\"-v\"\t\t\t\tDisplays the Ceiling version installed.\n";
    cout << "\"-h [page]\"\t\t\tDisplays all the opcodes.\n";
}

//Documentation
const int MAX_HELP_PAGES = 3;
void print_dict(char page){
    cout << "======================PAGE " << page << "/" << MAX_HELP_PAGES << "=======================\n";
    switch(page){
        case '1':
            cout << "[default functions]\n\n";
            cout << "start:" << "\t\t\t\t" << "Think of it as the \"main\" function.\n";
            cout << "update:" << "\t\t\t\t" << "Loops code that belongs to it.\n";
            cout << "[variables]\n\n";
            cout << "INT [name] [value]" << "\t\t" << "Declares an integer.\n";
            cout << "STRING [name] [value]" << "\t\t" << "Declares a string. (Which is just an array of characters)\n";
            cout << "CHAR [name] [value]" << "\t\t" << "Declares a character.\n";
            cout << "FLOAT [name] [value]" << "\t\t" << "Declares a float.\n";
            cout << "BOOL [name] [value]" << "\t\t" << "Declares a boolean.\n";
            break;
        case '2':
            cout << "[console]\n";
            cout << "COUT [string] [variable/value]" << "\t" << "Outputs a string into the console. Allows C-like formating.\n";
            cout << "CCIN [string] [variable]" << "\t" << "Checks user input and stores it into a variable. (Just like scanf in C)\n";
            cout << "CGIN" << "\t\t\t\t" << "Waits for user to press enter.\n";
            cout << "CTER" << "\t\t\t\t" << "Terminates program.\n";
            break;
        case '3':
            cout << "[conditions & loops]\n";
            cout << "IF [condition] THEN" << "\t\t" << "If statement.\n";
            cout << "ELIF [condition] THEN" << "\t\t" << "Else if statement.\n";
            cout << "ELSE" << "\t\t\t\t" << "Else statement.\n";
            cout << "FOR [itirator] IN [range] THEN" << "\t" << "For loop.\n";
            cout << "WHILE [condition] THEN" << "\t\t" << "While loop.\n";
            cout << "BREAK" << "\t\t\t\t" << "Breaks out of a loop.\n";
            cout << "END" << "\t\t\t\t" << "Marks an end of a condition/loop.\n";
            cout << "DO" << "\n";
            cout << "   [code]" << "\t\t\t" << "Do while loop.\n";
            cout << "WHEN [condition]" << "\n";
            break;
        default:
            cout << "Page " << page << " doesn't exist.\n";
            break;
    }
}