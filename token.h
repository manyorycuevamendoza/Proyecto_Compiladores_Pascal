#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        // Palabras clave
        PROGRAM, BEGIN, END, FUNCTION, PROCEDURE, TYPE, VAR,
        FOR, TO, DO, DIV, MOD, OR, AND, NOT, TRUE, FALSE, NIL,
        ARRAY, OF, POINTER,

        // Operadores y símbolos
        PLUS, MINUS, MUL, DIV_OP, // MUL: *, DIV_OP: operador de división real
        ASSIGN, EQUAL, NOTEQUAL, GREATER, GREATEREQUAL, LESS, LESSEQUAL,
        LBRACKET, RBRACKET, DOTDOT, // [ ], ..
        PD, PI, PC, COMA, // Paréntesis y coma
       
        // Tipos de datos
        INTEGER, LONGINT, CHAR, STRING, BOOLEAN, INT, VOID,

        // Identificadores y literales
        ID, NUM, BOOL, NIL_LITERAL,

        // Otros
        ERR, END_OF_FILE
    };

    Type type;
    std::string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string& source, int first, int last);

    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

#endif // TOKEN_H
