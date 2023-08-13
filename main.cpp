#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "includes/dict.cpp"
#include "includes/doc.cpp"

using std::cout;
using std::string;
using std::endl;

//using std::cout;

const int MAX_TOKENS = 10;
const char CEILING_VERSION[] = "Ceiling Programming Language v0.1-DEV1";

int lineNumber = 0;
int tokCount = 0;

bool isInRange = false;
bool isInEquel = false;
bool updateExists = false;

std::vector<string> tokens;

std::stringstream ccodeArgs;
std::stringstream ccodeArgsAdd;
std::stringstream ccodeArgsAddAdd;
std::stringstream systemCom;

std::ifstream myfile;
std::ofstream outfile("output.c");

string preTok;
string code;
string ccode;
string line;
string argument;

const char Separators[] = { ' ', 9 };
char lineCharacter;

//String tokenizer
int tokenizing_quotes(string str, int i) {
    tokens[tokCount] += '\"';
    do {
        if (str[i-1] == '\\') { i++; tokens[tokCount] += '\"'; }
        while (str[i] != '\"') { tokens[tokCount] += str[i]; i++; }
    } while (str[i-1] == '\\' and str[i-2] != '\\');
    tokens[tokCount] += '\"';
    return i;
}

//Tokenizer
void tokenization(string str) {
    tokCount = 0;
    tokens.push_back("");
    for (size_t i = 0; i < str.length(); i++) {
        tokens.push_back("");
        lineCharacter = str[i];
        if (lineCharacter == ' ') {
            tokCount++;
        } else if (lineCharacter == '\"') {
            i++;
            i = tokenizing_quotes(str, i);
        } else { tokens[tokCount] += lineCharacter; }
    }
}

//The replace function (Replace all instances of a certain string within a string with a different string)
void replace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

//Colonize function (either separate tokens by colons, or don't, and add to a string)
void colonize_tokens(int index, bool whichone) {
    for (int i = index; i<tokens.size(); i++) {
        if (tokens[i] != "" and whichone) { ccodeArgs << ", "; ccodeArgs << tokens[i]; }
        if (tokens[i] != "" and whichone == false) ccodeArgs << tokens[i];
    }
}

//Thenify simple
void thenify_tokens(int index) {
    for (int i = index; i<tokens.size(); i++) {
        if (tokens[i] != "" and (tokens[i] == "THEN" || tokens[i] == "then")) { ccodeArgs << opcodes["THEN"]; }
        else if (tokens[i] != "") { ccodeArgs << " "; ccodeArgs << tokens[i]; }
    }
}

//Thenify complex
void thenify_tokens(int index, string separatorUppercase, string separatorLowercase) {
    for (int i = index; i<tokens.size(); i++) {
        if (tokens[i] != "" and (tokens[i] == "THEN" || tokens[i] == "then")) {
            replace(ccode, "\4", opcodes["THEN"]);
            break;
        } else if (tokens[i] != "" and (tokens[i] == separatorUppercase || tokens[i] == separatorLowercase)) {
            isInRange = true;
        } else if (tokens[i] != "" and (tokens[i] == "=")) {
            isInEquel = true;
        } else if (tokens[i] != "" and isInRange) {
            ccodeArgsAdd << " ";
            ccodeArgsAdd << tokens[i];
        } else if (isInEquel) {
            ccodeArgsAddAdd << " ";
            ccodeArgsAddAdd << tokens[i];
        } else { ccodeArgs << " "; ccodeArgs << tokens[i]; }
    }
}

//Ceiling to C translator
void ceil_to_c() {
    replace(line, "    ", "");
    tokens.clear();
    tokens.resize(MAX_TOKENS);
    tokenization(line);
    code = tokens[0];
    if (code[code.length()-1] != ':') transform(code.begin(), code.end(), code.begin(), ::toupper);
    
    ///Checking opcodes and converting to C
    // Default functions
    if (code == "") {} else
    if (code == "start:") {
        outfile << opcodes["start:"];
    } else
    if (code == "update:") {
        outfile << opcodes["update:"];
        updateExists = true;
    //Console
    } else if (code == "COUT") {
        ccode = opcodes["COUT"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == ""){
            replace(ccode, "\2", "");
        } else {
            colonize_tokens(2, true);
            replace(ccode, "\2", ccodeArgs.str());
            ccodeArgs.str(string());
        }
        outfile << ccode;
    } else if (code == "CCIN") {
        ccode = opcodes["CCIN"];
        replace(ccode, "\1", tokens[1]);
        replace(ccode, "\2", tokens[2]);
        outfile << ccode;
    } else if (code == "CGIN") {
        ccode = opcodes["CGIN"];
        outfile << ccode;
    } else if (code == "CTER") {
        ccode = opcodes["CTER"];
        outfile << ccode;
    //Variables
    } else if (code == "INT") {
        ccode = opcodes["INT"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == "=")
            colonize_tokens(3, false);
            replace(ccode, "\2", ccodeArgs.str());
        ccodeArgs.str(string());
        outfile << ccode;
    } else if (code == "STRING") {
        ccode = opcodes["STRING"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == "=")
            colonize_tokens(3, false);
            replace(ccode, "\2", ccodeArgs.str());
        ccodeArgs.str(string());
        outfile << ccode;
    } else if (code == "CHAR") {
        ccode = opcodes["CHAR"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == "=")
            colonize_tokens(3, false);
            replace(ccode, "\2", ccodeArgs.str());
        ccodeArgs.str(string());
        outfile << ccode;
    } else if (code == "FLOAT") {
        ccode = opcodes["FLOAT"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == "=")
            colonize_tokens(3, false);
            replace(ccode, "\2", ccodeArgs.str());
        ccodeArgs.str(string());
        outfile << ccode;
    } else if (code == "BOOL") {
        ccode = opcodes["BOOL"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == "=")
            colonize_tokens(3, false);
            replace(ccode, "\2", ccodeArgs.str());
        ccodeArgs.str(string());
        outfile << ccode;
    //Conditions and loops
    } else if (code == "IF") {
        ccode = opcodes["IF"];
        if (tokens[1] == ""){
            replace(ccode, "\1", "");
        }else{
            thenify_tokens(1);
            replace(ccode, "\1", ccodeArgs.str());
            ccodeArgs.str(string());
        }
        outfile << ccode;
    } else if (code == "ELIF") {
        ccode = opcodes["ELIF"];
        if (tokens[1] == ""){
            replace(ccode, "\1", "");
        }else{
            thenify_tokens(1);
            replace(ccode, "\1", ccodeArgs.str());
            ccodeArgs.str(string());
        }
        outfile << ccode;
    } else if (code == "ELSE") {
        ccode = opcodes["ELSE"];
        outfile << ccode;
    } else if (code == "FOR") {
        ccode = opcodes["FOR"];
        if (tokens[1] == ""){
            replace(ccode, "\1", "");
        }else{
            thenify_tokens(1, "IN", "in");
            replace(ccode, "\1", ccodeArgs.str());
            replace(ccode, "\2", ccodeArgsAdd.str());
            replace(ccode, "\3", ccodeArgsAddAdd.str());
            isInRange = false;
            ccodeArgs.str(string());
            ccodeArgsAdd.str(string());
            ccodeArgsAddAdd.str(string());
        }
        outfile << ccode;
    } else if (code == "WHILE") {
        ccode = opcodes["WHILE"];
        if (tokens[1] == ""){
            replace(ccode, "\1", "");
        }else{
            thenify_tokens(1);
            replace(ccode, "\1", ccodeArgs.str());
            ccodeArgs.str(string());
        }
        outfile << ccode;
    } else if (code == "BREAK") {
        ccode = opcodes["BREAK"];
        outfile << ccode;
    } else if (code == "END") {
        ccode = opcodes["END"];
        outfile << ccode;
    } else if (code == "DO") {
        ccode = opcodes["DO"];
        outfile << ccode;
    } else if (code == "WHEN") {
        ccode = opcodes["WHEN"];
        if (tokens[1] == ""){
            replace(ccode, "\1", "");
        }else{
            thenify_tokens(1);
            replace(ccode, "\1", ccodeArgs.str());
            ccodeArgs.str(string());
        }
        outfile << ccode;
    //handeling nonexistant opcodes, comments and variable manipulation
    } else if (code[0] != '/' and code[1] != '/') {
        if (tokens[1] != "=" and tokens[1] != "+=" and tokens[1] != "-=" and tokens[1] != "*=" and tokens[1] != "/=") {
            std::stringstream errMessage;
            errMessage << "\"" << code << "\" at line " << lineNumber << " doesn't exist.";
            throw std::invalid_argument(errMessage.str().c_str());
        } else {
            for (int i = 0; i<tokens.size(); i++) if (tokens[i] != "") outfile << tokens[i];
            outfile << ";\n";
        }
    }
}

//The main function
int main(int argc, char* argv[]) {
    upt_dict();
    if (argc < 2) {
        show_help();
        return 0;
    }
    argument = argv[1];
    if (argument == "-o"){ //Compile code
        myfile.open(argv[2]);
        if(myfile.fail()) {
            perror("Couldn't open the file");
            exit(EXIT_FAILURE);
        }
        if(outfile.fail()){
            perror("Couldn't create the file");
            exit(EXIT_FAILURE);
        }
        outfile << opcodes["libs"];
        cout << "your.ceil file => output.c: ";

        while(getline(myfile, line)) { lineNumber++; ceil_to_c(); }

        if(updateExists) { outfile << "}}"; } else { outfile << "}"; }
        outfile.close();
        cout << "done\n";
        cout << "output.c => your program: ";
        systemCom << "gcc output.c -o " << argv[3];
        system(systemCom.str().c_str());
        cout << "done\n";
    }else if (argument == "-v"){ //CHECK VERSION
        cout << CEILING_VERSION << "\n";
    }else if (argument == "-h"){ //HELP
        print_dict((char)*argv[2]);
    } else {
        show_help();
        return 0;
    }
    return 0;
}