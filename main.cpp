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

int lineNumber = 0;
int tokCount = 0;
int lenghtOfIndentation;

bool isInArray = false;
bool isInRange = false;
bool isInEquel = false;
bool isInLogic = false;
bool updateExists = false;
bool functionExists = false;

std::vector<string> tokens;

std::stringstream ccodeArgs;
std::stringstream ccodeArgsAdd;
std::stringstream ccodeArgsAddAdd;
std::stringstream systemCom;

std::ifstream myfile;
std::ofstream outfile("output.c");

const string CEILING_VERSION = "Ceiling Programming Language v0.1-DEV1";
const string charactersToSkip = "(),;";
string code;
string ccode;
string line;
string argument;

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

//Character tokenizer
int tokenizing_singlequotes(string str, int i) {
    tokens[tokCount] += '\'';
    do {
        if (str[i-1] == '\\') { i++; tokens[tokCount] += '\''; }
        while (str[i] != '\'') { tokens[tokCount] += str[i]; i++; }
    } while (str[i-1] == '\\' and str[i-2] != '\\');
    tokens[tokCount] += '\'';
    return i;
}

//Tokenizer
void tokenization(string str) {
    tokCount = 0;
    tokens.push_back("");
    for (size_t i = 0; i < str.length(); i++) {
        tokens.push_back("");
        lineCharacter = str[i];
        if (lineCharacter == '{') { isInArray=true; }
        else if (lineCharacter == '}') { isInArray=false; }
        if (lineCharacter == '=') { isInEquel=true; }
        if (lineCharacter == ' ') {
            tokCount++;
        } else if (charactersToSkip.find(lineCharacter) != std::string::npos) {
            if (lineCharacter == ',' and isInArray) tokens[tokCount] += lineCharacter;
            if ((lineCharacter == '(' or lineCharacter == ')') and isInEquel) tokens[tokCount] += lineCharacter;
            if (str[i+1] == ' ') i++;
            else tokCount++;
        } else if (lineCharacter == '\"') {
            i++;
            i = tokenizing_quotes(str, i);
        } else if (lineCharacter == '\'') {
            i++;
            i = tokenizing_singlequotes(str, i);
        } else { tokens[tokCount] += lineCharacter; }
    }
    isInEquel = false;
}

//The replace function (Replaces all instances of a certain string within a string with a different string)
void replace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

//Colonize function
/*There is no pattern to go off of when it comes to setting condOne and condTwo:
| condOne | condTwo | conversion                                    |
| true    | false   | , token1, token2, token3...                   |
| false   | false   | token1 token2 token3 ...                      |
| false   | true    | token1 token2, token3 token4...               |
| true    | true    | token1token2 ("to" appears) token3, token4... |*/
void colonize_tokens(int index, bool condOne, bool condTwo) {
    for (int i = index; i<tokCount+1; i++) {
        if (tokens[i] != "" and condOne and condTwo == false) { ccodeArgs << ", "; ccodeArgs << tokens[i]; }
        if (tokens[i] != "" and condOne == false and condTwo == false) {
            ccodeArgs << tokens[i];
            if (i < tokCount) ccodeArgs << " ";
        }
        if (tokens[i] != "" and condOne == false and condTwo) {
            transform(tokens[i].begin(), tokens[i].end(), tokens[i].begin(), ::tolower);
            ccodeArgs << tokens[i] << " " << tokens[i+1];
            if (i < tokCount-1) ccodeArgs << ", ";
            i++;
        }
        if (tokens[i] != "" and condOne and condTwo) {
            ccodeArgs << tokens[i];
            if (tokens[i+1] == "TO" or tokens[i+1] == "to") break;
            if (i < tokCount) ccodeArgs << ", ";
        }
    }
}

//Thenify simple
void thenify_tokens(int index) {
    for (int i = index; i<tokCount+1; i++) {
        if (tokens[i] != "" and (tokens[i] == "THEN" || tokens[i] == "then")) { ccodeArgs << opcodes["THEN"]; }
        else if (tokens[i] != "") { ccodeArgs << " "; ccodeArgs << tokens[i]; }
    }
}

//Thenify complex
void thenify_tokens(int index, string separatorUppercase, string separatorLowercase) {
    for (int i = index; i<tokCount+1; i++) {
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

void function_declaration() {
    if (functionExists) outfile << "}\n";
    functionExists = true;
    ccode = opcodes["function:"];
    replace(tokens[0], ":", "");
    replace(tokens[1], ":", "");
    if (tokens[1][0] == '<' and tokens[1].at(tokens[1].length()-1) == '>') {
        replace(ccode, "\2", tokens[1]);
        replace(ccode, "<", "");
        replace(ccode, ">", "");
        transform(ccode.begin(), ccode.end(), ccode.begin(), ::tolower);
    } else { replace(ccode, "\2", "void"); }
    if (tokCount > 1) { colonize_tokens(2, false, true); replace(ccode, "\3", ccodeArgs.str()); ccodeArgs.str(string()); }
    else { replace(ccode, "\3", ""); }
    replace(ccode, "\1", tokens[0]);
    replace(ccode, ":", "");
}

//Ceiling to C translator
void ceil_to_c() {
    lenghtOfIndentation = 0;
    for (int i = 0; i<=line.length(); i++) {
        if (line[i] != ' ') break;
        lenghtOfIndentation++;
    }
    replace(line, "\n", "");
    replace(line, "\t", "");
    line.erase(0, lenghtOfIndentation);
    tokens.clear();
    tokenization(line);
    code = tokens[0];
    
    if (code[code.length()-1] != ':') transform(code.begin(), code.end(), code.begin(), ::toupper);
    ///Checking opcodes and converting to C
    // Default functions
    if (code == "") {} else
    if (code == "start:") {
        if (functionExists) outfile << "}\n\n";
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
            colonize_tokens(2, true, false);
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
        if (tokens[2] == "=") {
            colonize_tokens(3, false, false);
            replace(ccode, "\2", ccodeArgs.str());
        }
        ccodeArgs.str(string());
        outfile << ccode;
    } else if (code == "STRING") {
        ccode = opcodes["STRING"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == "=") {
            colonize_tokens(3, false, false);
            replace(ccode, "\2", ccodeArgs.str());
        }
        ccodeArgs.str(string());
        outfile << ccode;
    } else if (code == "CHAR") {
        ccode = opcodes["CHAR"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == "=") {
            colonize_tokens(3, false, false);
            replace(ccode, "\2", ccodeArgs.str());
        }
        ccodeArgs.str(string());
        outfile << ccode;
    } else if (code == "FLOAT") {
        ccode = opcodes["FLOAT"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == "=") {
            colonize_tokens(3, false, false);
            replace(ccode, "\2", ccodeArgs.str());
        }
        ccodeArgs.str(string());
        outfile << ccode;
    } else if (code == "BOOL") {
        ccode = opcodes["BOOL"];
        replace(ccode, "\1", tokens[1]);
        if (tokens[2] == "=") {
            colonize_tokens(3, false, false);
            replace(ccode, "\2", ccodeArgs.str());
        }
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
    //functions
    } else if (code == "RETURN") {
        ccode = opcodes["RETURN"];
        colonize_tokens(1, false, false);
        replace(ccode, "\1", ccodeArgs.str());
        outfile << ccode;
        ccodeArgs.str(string());
    } else if (code == "CONTINUE") {
        ccode = opcodes["CONTINUE"];
        outfile << ccode;
        ccodeArgs.str(string());
    } else if (code == "CALLIN") {
        ccode = opcodes["CALLIN"];
        replace(ccode, "\1", tokens[1]);
        colonize_tokens(2, true, true);
        replace(ccode, "\2", ccodeArgs.str());
        outfile << ccode;
        ccodeArgs.str(string());
    } else if (code == "CALLTO") {
        ccode = opcodes["CALLTO"];
        replace(ccode, "\1", tokens[1]);
        colonize_tokens(2, true, true);
        replace(ccode, "\2", ccodeArgs.str());
        replace(ccode, "\3", tokens[tokCount]);
        outfile << ccode;
        ccodeArgs.str(string());
        ccodeArgsAdd.str(string());
    }
    //handeling nonexistant opcodes, comments, function creation and variable manipulation
    else if (code[0] != '/' and code[1] != '/') {
        if (line[line.length()-1] == ':') {
            function_declaration();
            outfile << ccode;
        } else if (line[line.length()-1] != ':' and (tokens[1] != "=" and tokens[1] != "+=" and tokens[1] != "-=" and tokens[1] != "*=" and tokens[1] != "/=")) {
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
        //myfile.open(argv[2]);
        myfile = std::ifstream(argv[2], std::ios::out);
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

        while(getline(myfile, line)) { replace(line, "\r", "\n"); lineNumber++; ceil_to_c(); }

        if(updateExists) { outfile << "}}"; } else { outfile << "return 0;\n}"; }
        outfile.close();
        cout << "done\n";
        cout << "output.c => your program: ";
        systemCom << "gcc output.c -o " << argv[3];
        system(systemCom.str().c_str());
        cout << "done\n";
    }else if (argument == "-v"){ //CHECK VERSION
        cout << CEILING_VERSION.c_str() << "\n";
    }else if (argument == "-h"){ //HELP
        print_dict((char)*argv[2]);
    } else {
        show_help();
        return 0;
    }
    return 0;
}
