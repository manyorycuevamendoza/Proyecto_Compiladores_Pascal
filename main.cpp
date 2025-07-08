#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "visitor.h"
#include "imp_interpreter.hh"
#include "gencode.hh"
#include "imp_type.hh"
#include "type_checker.hh"

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
    string input_copy = input;
    Scanner scanner_test(input_copy.c_str());
    test_scanner(&scanner_test);
    cout << "Scanner exitoso" << endl;
    cout << endl;
    cout << "Iniciando parsing:" << endl;

    Parser parser(&scanner); 
    try {
        Program* program = parser.parseProgram();
        // 🔍 NUEVA FASE: Type Checker (Análisis Semántico)
        cout << "Iniciando Type Checker:" << endl;
        TypeChecker typeChecker;
        typeChecker.check(program);
        cout << endl;
        
        cout << "Iniciando Visitor:" << endl;
        
        PrintVisitor printVisitor;
        ImpInterpreter interpreter;
        cout << endl;
        cout << "IMPRIMIR:" << endl;
        printVisitor.imprimir(program);
        cout  << endl;
        cout << endl << "Run program:" << endl;
        interpreter.interpret(program);
        cout << "End of program execution" << endl;
        
        // Generar código ensamblador en un archivo separado
        string output_filename = argv[1];
        size_t last_dot = output_filename.find_last_of('.');
        if (last_dot != string::npos) {
            output_filename = output_filename.substr(0, last_dot);
        }
        output_filename += ".s";
        
        // Generar código assembly usando un proceso separado
        cout << endl << "Generando código assembly..." << endl;
        
        // Crear un comando para ejecutar solo el generador de código
        string cmd = "./compilador_asm " + string(argv[1]) + " > " + output_filename + " 2>/dev/null";
        int result = system(cmd.c_str());
        
        if (result == 0) {
            cout << "Código ensamblador generado en: " << output_filename << endl;
        } else {
            cout << "Error generando código assembly" << endl;
        }
        
        delete program;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }

    return 0;
}