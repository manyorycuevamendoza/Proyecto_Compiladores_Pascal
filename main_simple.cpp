#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "gencode.hh"

using namespace std;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Numero incorrecto de argumentos. Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        exit(1);
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        exit(1);
    }

    string input;
    string line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    Scanner scanner(input.c_str());
    Parser parser(&scanner); 
    
    try {
        Program* program = parser.parseProgram();
        // Generar código assembly usando el generador simplificado
        GenCodeVisitor codeGen;
        codeGen.generar(program);
        
        delete program;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }

    return 0;
} 