#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "includes/dict.cpp"

using std::cout;
using std::string;
using std::endl;

//using std::cout;

const int MAX_TOKENS = 10;
const char CEILING_VERSION[] = "Ceiling Programming Language v0.1-DEV1";

int lineNumber = 0;
int tokenCount = 0;

bool isInRange = false;
bool updateExists = false;

std::vector<string> tokens;

std::stringstream ccodeArgs;
std::stringstream ccodeArgsAdd;
std::stringstream sstm;
std::stringstream systemCom;

string preTok;
string code;
string ccode;
string line;
string argument;
string systemComFail;

const char Separators[] = { ' ', 9 };

bool Str_IsSeparator( const char Ch ) {
    for ( size_t i = 0; i != sizeof( Separators ); i++ ) {
        if ( Separators[i] == Ch ) { return true; }
    }
    return false;
}

void tokenize_string( size_t FromToken, size_t ToToken, const std::string& Str, std::vector<std::string>& Components /*, bool ShouldTrimSpaces*/ ) {
    size_t TokenNum = 0;
    size_t Offset   = FromToken - 1;

    const char* CStr  = Str.c_str();
    const char* CStrj = Str.c_str();

    while ( *CStr ) {
        // bypass spaces & delimiting chars
        while ( *CStr && Str_IsSeparator( *CStr ) ) { CStr++; }
        if ( !*CStr ) { return; }
        bool InsideQuotes = ( *CStr == '\"' );
        if ( InsideQuotes ) {
            for ( CStrj = ++CStr; *CStrj && *CStrj != '\"'; CStrj++ );
        }
        else {
            for ( CStrj = CStr; *CStrj && !Str_IsSeparator( *CStrj ); CStrj++ );
        }
        // extract token
        if ( CStr != CStrj )
        {
            TokenNum++;
            // store each token found
            if ( TokenNum >= FromToken )
            {
                Components[ TokenNum-Offset ].assign( CStr, CStrj );
                // if ( ShouldTrimSpaces ) { Str_TrimSpaces( &Components[ TokenNum-Offset ] ); }
                // proceed to next token
                if ( TokenNum >= ToToken ) { return; }
            }
            CStr = CStrj;
            // exclude last " from token, handle EOL
            if ( *CStr ) { CStr++; }
        }
    }
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

int main(int argc, char* argv[]) {
    upt_dict(); //Loads a map dictionary from dict.h
    if (argc < 2) {
        cout << "Please, use any of these arguments:" << endl;
        cout << "\"-o [.ceil file] [output file]\"\tCompiles your code to an executable file" << endl;
        cout << "\"-v\"\t\t\t\tDisplays the Ceiling version installed." << endl;
        cout << "\"-h [page]\"\t\t\tDisplays all the opcodes." << endl;
    }
    argument = argv[1];
    if (argument == "-o"){ //THE ACTUAL COMPILER
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
            tokenize_string(1, MAX_TOKENS-1, line, tokens);
            lineNumber++;
            code = tokens[1];
            //cout << tokens[0] << " " << tokens[1] << " " << tokens[2] << endl;
            if (code == "start:" || code == "update:") {}
            else {outfile << "line" << lineNumber << ":" << endl;}

            if (code[code.length()-1] != ':') transform(code.begin(), code.end(), code.begin(), ::toupper);
            //cout << code;
            ///Checking opcodes and converting to C
            // Default functions
            if (code == "start:") {
                outfile << opcodes["start:"];
                outfile << "line" << lineNumber << ":" << endl;
            }
            if (code == "update:") {
                outfile << opcodes["update:"];
                outfile << "line" << lineNumber << ":" << endl;
                updateExists = true;
            }
            //Console
            if (code == "COUT") {
                ccode = opcodes["COUT"];
                replace(ccode, "\1", tokens[2]);
                if (tokens[3] == ""){
                    replace(ccode, "\2", "");
                }else{
                    for (int i = 3; i<tokens.size(); i++) {
                        if (tokens[i] != ""){
                            ccodeArgs << ", ";
                            ccodeArgs << tokens[i];
                        }
                    }
                    replace(ccode, "\2", ccodeArgs.str());
                    ccodeArgs.str(string());
                }
                outfile << ccode;
            }
            if (code == "CCIN") {
                ccode = opcodes["CCIN"];
                replace(ccode, "\1", tokens[2]);
                replace(ccode, "\2", tokens[3]);
                outfile << ccode;
            }
            if (code == "CGIN") {
                ccode = opcodes["CGIN"];
                outfile << ccode;
            }
            if (code == "CTER") {
                ccode = opcodes["CTER"];
                outfile << ccode;
            }
            //Variables
            if (code == "INTV") {
                ccode = opcodes["INTV"];
                replace(ccode, "\1", tokens[2]);
                replace(ccode, "\2", tokens[3]);
                outfile << ccode;
            }
            if (code == "STRV") {
                ccode = opcodes["STRV"];
                replace(ccode, "\1", tokens[2]);
                replace(ccode, "\2", tokens[3]);
                outfile << ccode;
            }
            if (code == "CHRV") {
                ccode = opcodes["CHRV"];
                replace(ccode, "\1", tokens[2]);
                replace(ccode, "\2", tokens[3]);
                outfile << ccode;
            }
            if (code == "FLOV") {
                ccode = opcodes["FLOV"];
                replace(ccode, "\1", tokens[2]);
                replace(ccode, "\2", tokens[3]);
                outfile << ccode;
            }
            if (code == "BOOV") {
                ccode = opcodes["BOOV"];
                replace(ccode, "\1", tokens[2]);
                replace(ccode, "\2", tokens[3]);
                outfile << ccode;
            }
            //Conditions and loops
            if (code == "IF") {
                ccode = opcodes["IF"];
                if (tokens[2] == ""){
                    replace(ccode, "\1", "");
                }else{
                    for (int i = 2; i<tokens.size(); i++) {
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
            }
            if (code == "ELIF") {
                ccode = opcodes["ELIF"];
                if (tokens[2] == ""){
                    replace(ccode, "\1", "");
                }else{
                    for (int i = 2; i<tokens.size(); i++) {
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
            }
            if (code == "ELSE") {
                ccode = opcodes["ELSE"];
                outfile << ccode;
            }
            if (code == "FOR") {
                ccode = opcodes["FOR"];
                if (tokens[2] == ""){
                    replace(ccode, "\1", "");
                }else{
                    for (int i = 2; i<tokens.size(); i++) {
                        if (tokens[i] != "") {
                            if (tokens[i] == "THEN" || tokens[i] == "then") {
                                replace(ccode, "\3", opcodes["THEN"]);
                                break;
                            } else if (tokens[i] == "IN" || tokens[i] == "in") {
                                isInRange = true;
                            } else {
                                if (isInRange) {
                                    ccodeArgsAdd << " ";
                                    ccodeArgsAdd << tokens[i];
                                } else {
                                    ccodeArgs << " ";
                                    ccodeArgs << tokens[i];
                                }
                            }
                        }
                    }
                    replace(ccode, "\1", ccodeArgs.str());
                    replace(ccode, "\1", ccodeArgs.str());
                    replace(ccode, "\1", ccodeArgs.str());
                    replace(ccode, "\2", ccodeArgsAdd.str());
                    isInRange = false;
                    ccodeArgs.str(string());
                    ccodeArgsAdd.str(string());
                }
                outfile << ccode;
            }
            if (code == "WHILE") {
                ccode = opcodes["WHILE"];
                if (tokens[2] == ""){
                    replace(ccode, "\1", "");
                }else{
                    for (int i = 2; i<tokens.size(); i++) {
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
            }
            if (code == "BREAK") {
                ccode = opcodes["BREAK"];
                outfile << ccode;
            }
            if (code == "END") {
                ccode = opcodes["END"];
                outfile << ccode;
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
    }else if (argument == "-h"){ //HEEEEEEELP STEP BRO, I'M STUUUUCK
        print_dict((char)*argv[2]);
    } else {
        cout << "Please, use any of these arguments:" << endl;
        cout << "\"-o [.ceil file] [output file]\"\tCompiles your code to an executable file" << endl;
        cout << "\"-v\"\t\t\t\tDisplays the Ceiling version installed." << endl;
        cout << "\"-h [page]\"\t\t\tDisplays all the opcodes." << endl;
    }
    return 0;
}