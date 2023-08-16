#include <iostream>
#include <map>


using std::string;
using std::pair;

std::map<string, string> opcodes;

void upt_dict() {
    opcodes.insert(pair<string, string>("libs", "#include <stdio.h>\n#include <stdbool.h>\n#include <stdlib.h>\nint update();\n"));
    //Default functions
    opcodes.insert(pair<string, string>("start:", "int main(void) {\n"));
    opcodes.insert(pair<string, string>("update:", "update();\nreturn 0;\n}\n\nint update() {\nwhile(true) {\n"));
    //Variables
    opcodes.insert(pair<string, string>("INT", "int \1 = \2;\n"));
    opcodes.insert(pair<string, string>("STRING", "char \1[] = \2;\n"));
    opcodes.insert(pair<string, string>("CHAR", "char \1 = \2;\n"));
    opcodes.insert(pair<string, string>("FLOAT", "float \1 = \2;\n"));
    opcodes.insert(pair<string, string>("BOOL", "bool \1 = \2;\n"));
    //Console
    opcodes.insert(pair<string, string>("COUT", "printf(\1 \2);\n"));
    opcodes.insert(pair<string, string>("CCIN", "scanf(\1,\2);\n"));
    opcodes.insert(pair<string, string>("CGIN", "while(getchar() != \'\\n\'){};\n"));
    opcodes.insert(pair<string, string>("CTER", "exit(0);\n"));
    //Conditionals & loops
    opcodes.insert(pair<string, string>("IF", "if (\1"));
    opcodes.insert(pair<string, string>("ELIF", "} else if (\1"));
    opcodes.insert(pair<string, string>("ELSE", "} else {\n"));
    opcodes.insert(pair<string, string>("FOR", "for (int \1 = \3; \1<=\2; \1++\4"));
    opcodes.insert(pair<string, string>("WHILE", "while (\1"));
    opcodes.insert(pair<string, string>("BREAK", "break;\n"));
    opcodes.insert(pair<string, string>("THEN", ") {\n"));
    opcodes.insert(pair<string, string>("END", "}\n"));
    opcodes.insert(pair<string, string>("DO", "do {\n"));
    opcodes.insert(pair<string, string>("WHEN", "} while (\1);\n"));
    //Functions
    opcodes.insert(pair<string, string>("function:", "\2 \1(\3) {\n"));
    opcodes.insert(pair<string, string>("RETURN", "return \1;\n"));
    opcodes.insert(pair<string, string>("CONTINUE", "continue;\n"));
    opcodes.insert(pair<string, string>("CALLIN", "\1(\2);\n"));
    opcodes.insert(pair<string, string>("CALLTO", "\3 = \1(\2);\n"));
}
