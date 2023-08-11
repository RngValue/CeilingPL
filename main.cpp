#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "includes/dict.cpp"

using std::cout;
using std::string;
using std::endl;

//using std::cout;

const int MAX_TOKENS = 10;
const char CEILING_VERSION[] = "Ceiling Programming Language v0.1-DEV1";

int lineNumber = 0;
int tokenCount = 0;
int tokCount = 0;

bool isInRange = false;
bool isInEquel = false;
bool updateExists = false;
bool tokenInQuotes = false;

std::vector<string> tokens;

std::stringstream ccodeArgs;
std::stringstream ccodeArgsAdd;
std::stringstream ccodeArgsAddAdd;
std::stringstream sstm;
std::stringstream systemCom;

string preTok;
string code;
string ccode;
string line;
string argument;
string systemComFail;

const char Separators[] = { ' ', 9 };
char c;

void tokenization(string str) {
    tokCount = 0;
    tokens.push_back("");
    for (size_t i = 0; i < str.length(); i++) {
        tokens.push_back("");
        c = str[i];
        if (c == ' ') {
            tokCount++;
        } else if (c == '\"') {
            tokenInQuotes = true;
            i++;
            tokens[tokCount] += '\"';
            do {
                if (str[i-1] == '\\') {
                    i++;
                    tokens[tokCount] += '\"';
                }
                while (str[i] != '\"') {
                    tokens[tokCount] += str[i];
                    i++;
                }
            } while (str[i-1] == '\\' and str[i-2] != '\\');
            tokens[tokCount] += '\"';
        } else {
            tokenInQuotes = false;
            tokens[tokCount] += c;
        }
    }
}

void replace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

int main(int argc, char* argv[]) {
    upt_dict();
    if (argc < 2) {
        cout << "Please, use any of these arguments:" << endl;
        cout << "\"-o [.ceil file] [output file]\"\tCompiles your code to an executable file" << endl;
        cout << "\"-v\"\t\t\t\tDisplays the Ceiling version installed." << endl;
        cout << "\"-h [page]\"\t\t\tDisplays all the opcodes." << endl;
    }
    argument = argv[1];
    ///THE ACTUAL COMPILER
    if (argument == "-o"){
        std::ifstream myfile;
        std::ofstream outfile("output.c");
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
        while(getline(myfile, line)) {
            replace(line, "    ", "");
            tokens.clear();
            tokens.resize(MAX_TOKENS);
            tokenization(line);
            lineNumber++;
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
            } else
            //Console
            if (code == "COUT") {
                ccode = opcodes["COUT"];
                replace(ccode, "\1", tokens[1]);
                if (tokens[2] == ""){
                    replace(ccode, "\2", "");
                }else{
                    for (int i = 2; i<tokens.size(); i++) {
                        if (tokens[i] != ""){
                            ccodeArgs << ", ";
                            ccodeArgs << tokens[i];
                        }
                    }
                    replace(ccode, "\2", ccodeArgs.str());
                    ccodeArgs.str(string());
                }
                outfile << ccode;
            } else
            if (code == "CCIN") {
                ccode = opcodes["CCIN"];
                replace(ccode, "\1", tokens[1]);
                replace(ccode, "\2", tokens[2]);
                outfile << ccode;
            } else
            if (code == "CGIN") {
                ccode = opcodes["CGIN"];
                outfile << ccode;
            } else
            if (code == "CTER") {
                ccode = opcodes["CTER"];
                outfile << ccode;
            } else
            //Variables
            if (code == "INT") {
                ccode = opcodes["INT"];
                replace(ccode, "\1", tokens[1]);
                if (tokens[2] == "=")
                    for (int i = 3; i<tokens.size(); i++)
                        if (tokens[i] != "")
                            ccodeArgs << tokens[i];
                    replace(ccode, "\2", ccodeArgs.str());
                ccodeArgs.str(string());
                outfile << ccode;
            } else
            if (code == "STRING") {
                ccode = opcodes["STRING"];
                replace(ccode, "\1", tokens[1]);
                if (tokens[2] == "=")
                    for (int i = 3; i<tokens.size(); i++)
                        if (tokens[i] != "")
                            ccodeArgs << tokens[i];
                    replace(ccode, "\2", ccodeArgs.str());
                ccodeArgs.str(string());
                outfile << ccode;
            } else
            if (code == "CHAR") {
                ccode = opcodes["CHAR"];
                replace(ccode, "\1", tokens[1]);
                if (tokens[2] == "=")
                    for (int i = 3; i<tokens.size(); i++)
                        if (tokens[i] != "")
                            ccodeArgs << tokens[i];
                    replace(ccode, "\2", ccodeArgs.str());
                ccodeArgs.str(string());
                outfile << ccode;
            } else
            if (code == "FLOAT") {
                ccode = opcodes["FLOAT"];
                replace(ccode, "\1", tokens[1]);
                if (tokens[2] == "=")
                    for (int i = 3; i<tokens.size(); i++)
                        if (tokens[i] != "")
                            ccodeArgs << tokens[i];
                    replace(ccode, "\2", ccodeArgs.str());
                ccodeArgs.str(string());
                outfile << ccode;
            } else
            if (code == "BOOL") {
                ccode = opcodes["BOOL"];
                replace(ccode, "\1", tokens[1]);
                if (tokens[2] == "=")
                    for (int i = 3; i<tokens.size(); i++)
                        if (tokens[i] != "")
                            ccodeArgs << tokens[i];
                    replace(ccode, "\2", ccodeArgs.str());
                ccodeArgs.str(string());
                outfile << ccode;
            } else
            //Conditions and loops
            if (code == "IF") {
                ccode = opcodes["IF"];
                if (tokens[1] == ""){
                    replace(ccode, "\1", "");
                }else{
                    for (int i = 1; i<tokens.size(); i++) {
                        if (tokens[i] != "") {
                            if (tokens[i] == "THEN" || tokens[i] == "then") { ccodeArgs << opcodes["THEN"]; } else {
                                ccodeArgs << " ";
                                ccodeArgs << tokens[i];
                            }
                        }
                    }
                    replace(ccode, "\1", ccodeArgs.str());
                    ccodeArgs.str(string());
                }
                outfile << ccode;
            } else
            if (code == "ELIF") {
                ccode = opcodes["ELIF"];
                if (tokens[1] == ""){
                    replace(ccode, "\1", "");
                }else{
                    for (int i = 1; i<tokens.size(); i++) {
                        if (tokens[i] != "") {
                            if (tokens[i] == "THEN" || tokens[i] == "then") { ccodeArgs << opcodes["THEN"]; } else {
                                ccodeArgs << " ";
                                ccodeArgs << tokens[i];
                            }
                        }
                    }
                    replace(ccode, "\1", ccodeArgs.str());
                    ccodeArgs.str(string());
                }
                outfile << ccode;
            } else
            if (code == "ELSE") {
                ccode = opcodes["ELSE"];
                outfile << ccode;
            } else
            if (code == "FOR") {
                ccode = opcodes["FOR"];
                if (tokens[1] == ""){
                    replace(ccode, "\1", "");
                }else{
                    for (int i = 1; i<tokens.size(); i++) {
                        if (tokens[i] != "") {
                            if (tokens[i] == "THEN" || tokens[i] == "then") {
                                replace(ccode, "\4", opcodes["THEN"]);
                                break;
                            } else if (tokens[i] == "IN" || tokens[i] == "in") {
                                isInRange = true;
                            } else if (tokens[i] == "=") {
                                isInEquel = true;
                            } else {
                                if (isInRange) {
                                    ccodeArgsAdd << " ";
                                    ccodeArgsAdd << tokens[i];
                                } else {
                                    if (isInEquel){
                                        ccodeArgsAddAdd << " ";
                                        ccodeArgsAddAdd << tokens[i];
                                    } else {
                                        ccodeArgs << " ";
                                        ccodeArgs << tokens[i];
                                    }
                                }
                            }
                        }
                    }
                    replace(ccode, "\1", ccodeArgs.str());
                    replace(ccode, "\2", ccodeArgsAdd.str());
                    replace(ccode, "\3", ccodeArgsAddAdd.str());
                    isInRange = false;
                    ccodeArgs.str(string());
                    ccodeArgsAdd.str(string());
                }
                outfile << ccode;
            } else
            if (code == "WHILE") {
                ccode = opcodes["WHILE"];
                if (tokens[1] == ""){
                    replace(ccode, "\1", "");
                }else{
                    for (int i = 1; i<tokens.size(); i++) {
                        if (tokens[i] != "") {
                            if (tokens[i] == "THEN" || tokens[i] == "then") { ccodeArgs << opcodes["THEN"]; } else {
                                ccodeArgs << " ";
                                ccodeArgs << tokens[i];
                            }
                        }
                    }
                    replace(ccode, "\1", ccodeArgs.str());
                    ccodeArgs.str(string());
                }
                outfile << ccode;
            } else
            if (code == "BREAK") {
                ccode = opcodes["BREAK"];
                outfile << ccode;
            } else
            if (code == "END") {
                ccode = opcodes["END"];
                outfile << ccode;
            } else
            //handeling nonexistant opcodes, comments and variable manipulation
            if (code[0] == '/' and code[1] == '/'){} else {
                if (tokens[1] != "=" and tokens[1] != "+=" and tokens[1] != "-=" and tokens[1] != "*=" and tokens[1] != "/=") {
                    std::stringstream errMessage;
                    errMessage << "\"" << code << "\" at line " << lineNumber << " doesn't exist.";
                    throw std::invalid_argument(errMessage.str().c_str());
                } else {
                    for (int i = 0; i<tokens.size(); i++)
                        if (tokens[i] != "")
                            outfile << tokens[i];
                    outfile << ";\n";
                }
            }
        }
        if(updateExists) { outfile << "}}"; } else { outfile << "}"; }
        outfile.close();
        cout << "done" << endl;
        cout << "output.c => your program: ";
        systemCom << "gcc output.c -o " << argv[3];
        system(systemCom.str().c_str());
        cout << "done" << endl;
    }else if (argument == "-v"){ //CHECK VERSION
        cout << CEILING_VERSION << endl;
    }else if (argument == "-h"){ //HELP
        print_dict((char)*argv[2]);
    } else {
        cout << "Please, use any of these arguments:" << endl;
        cout << "\"-o [.ceil file] [output file]\"\tCompiles your code to an executable file" << endl;
        cout << "\"-v\"\t\t\t\tDisplays the Ceiling version installed." << endl;
        cout << "\"-h [page]\"\t\t\tDisplays all the opcodes." << endl;
    }
    return 0;
}