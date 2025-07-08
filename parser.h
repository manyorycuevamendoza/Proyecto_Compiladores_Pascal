#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"

class Parser {
private:
    Scanner* scanner;               // Scanner que proporciona tokens
    Token *current, *previous;      // Punteros a los tokens actual y anterior
    bool match(Token::Type ttype);  // Verifica si el token actual es del tipo especificado
    bool check(Token::Type ttype);  
    bool advance();                 // Avanza al siguiente token
    bool isAtEnd();                 // Verifica si se ha llegado al final del archivo
    list<Stm*> parseStmList();      // Analiza una lista de sentencias
    Exp* parseTerm();               // Analiza un término aritmético
    Exp* parseFactor();             // Analiza un factor aritmético
    // Función auxiliar para parsear tipos (incluyendo arrays anidados)
    string parseType();             // parsea un tipo de dato
    // Nuevas funciones para arrays y punteros
    Exp* parseExprRel();            // Analiza una expresión relacional
    Exp* parseExprOr();
    Exp* parseExprAnd();            // Analiza una expresión lógica AND
    Exp* parseExprNot();            // Analiza una expresión lógica NOT
    Exp* parseExprAdd();            // Analiza una expresión additiva
    FunctionDef* parseFunctionDeclaration();
    // Fin de las nuevas funciones para arrays y punteros
public:
    Parser(Scanner* scanner);
    Program* parseProgram();       //punto de entrada del programa
    Stm* parseStatement();         //analiza una sentencia
    StatementList* parseStatementList(); //analiza una lista de sentencias
    VarDec* parseVarDec();         //analiza una declaración de variable
    VarDecList* parseVarDecList(); //analiza una lista de declaraciones de variable
    Body* parseBody();             //analiza un cuerpo de programa
};

#endif // PARSER_H