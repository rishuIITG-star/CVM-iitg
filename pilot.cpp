#include <iostream>
#include <fstream>
#include <sstream>
#include<vector>
#include "Compiler.h"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << " Could not open file '" << filename << "'\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2)
    {  
         std::cout << "supply source file\n";
        return 1;
    }
    
    std::string source = readFile(argv[1]);
    
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    
    Compiler compiler;
    compiler.compile(ast);
    
    VirtualMachine vm;
    vm.execute(compiler.getBytecode(), compiler.getConstants());
    
    return 0;
}