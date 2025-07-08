#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        // Operadores aritméticos básicos
        PLUS, MINUS, MUL, DIV, NUM, ERR, PD, PI, 
        
        // Identificadores y asignación
        ID, ASSIGN, COLON, PC, COMA,
        
        // Control de flujo básico
        IF, THEN, ELSE, ENDIF, WHILE, DO, ENDWHILE, FOR,
        
        // Estructura del programa
        PROGRAM,        // "program" - inicio del programa
        BEGIN,          // "begin" - inicio de bloque
        END,            // "end" - fin de bloque
        DOT,            // "." - punto final del programa

        // Declaraciones
        TYPE,           // "type" - declaración de tipos personalizados
        VAR,            // "var" - declaración de variables
        FUNCTION,       // "function" - declaración de función
        PROCEDURE,      // "procedure" - declaración de procedimiento

        // Control de flujo adicional
        TO,             // "to" - en bucle for (for i := 1 to 10)

        // Operadores lógicos
        OR,             // "or" - operador lógico OR
        AND,            // "and" - operador lógico AND
        NOT,            // "not" - operador lógico NOT

        // Operadores aritméticos adicionales
        DIV_OP,         // "div" - división entera
        MOD,            // "mod" - módulo

        // Operadores de comparación
        LT, LE, EQ,     // <, <=, == (ya existían)
        EQUAL,          // "=" - igual
        NOTEQUAL,       // "<>" - diferente
        GREATER,        // ">" - mayor
        GREATEREQUAL,   // ">=" - mayor o igual
        
        // Arrays y punteros
        ARRAY,          // "array" - declaración de array
        OF,             // "of" - en arrays (array[1..10] of Integer)
        LBRACKET,       // "[" - corchete izquierdo
        RBRACKET,       // "]" - corchete derecho
        DOTDOT,         // ".." - rango en arrays
        POINTER,        // "^" - puntero
        AT,             // "@" - dirección de

        // Valores literales
        TRUE,           // "true" - valor booleano
        FALSE,          // "false" - valor booleano
        NIL,            // "nil" - valor nulo para punteros

        // Tipos básicos
        INTEGER,        // "Integer" - tipo entero
        LONGINT,        // "Longint" - tipo entero largo
        BOOLEAN,        // "Boolean" - tipo booleano
        CHAR,           // "Char" - tipo carácter
        REAL,           // "Real" - tipo real
        STRING,         // "String" - tipo cadena

        // Parámetros
        CONST,          // "const" - parámetro constante
        OUT,            // "out" - parámetro de salida

        // Otros
        BREAK,          // "break" - salir del bucle
        WRITELN,        // "writeln" - escribir con salto de línea
        PRINT,          // "print" - imprimir
        RETURN,         // "return" - retornar
        EXIT,           // "exit" - salir de función/procedimiento
        NEW,            // "new" - asignar memoria dinámicamente
        DISPOSE,        // "dispose" - liberar memoria dinámicamente
        FUN, ENDFUN,    // "fun", "endfun" - función
        IFEXP,          // "ifexp" - expresión condicional
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