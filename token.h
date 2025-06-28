#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        // Operadores y Símbolos
        PLUS, MINUS, MUL, DIV,
        LT, LE, EQ,
        ASSIGN,       // :=
        COMA,         // ,
        PC,           // ;
        PD, PI,       // ) y (
        CORCHETE_A,   // [
        CORCHETE_C,   // ]
        PUNTO_PUNTO,  // ..

        // Palabras clave
        VAR, ARRAY, OF, RANGE, 
        POINTER,       // ^
        FUN, ENDFUN, RETURN,
        IF, THEN, ELSE, ENDIF,
        WHILE, DO, ENDWHILE,
        FOR, TO, ENDFOR,
        PRINT,

        // Tipos de datos
        INTEGER, LONGINT, CHAR, STRING, BOOLEAN, INT, VOID,

        // Constantes y Literales
        TRUE, FALSE, NUM, BOOL,

        // Identificadores
        ID,

        // Otros
        ERR, END
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
