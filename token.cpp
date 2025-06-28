#include "token.h"
#include <iostream>

using namespace std;

Token::Token(Type type): type(type) { text = ""; }
Token::Token(Type type, char c): type(type) { text = string(1, c); }
Token::Token(Type type, const string& source, int first, int last): type(type) {
    text = source.substr(first, last);
}

ostream& operator<<(ostream& outs, const Token& tok) {
    switch (tok.type) {
        // Palabras clave
        case Token::PROGRAM: outs << "TOKEN(PROGRAM)"; break;
        case Token::BEGIN: outs << "TOKEN(BEGIN)"; break;
        case Token::END: outs << "TOKEN(END)"; break;
        case Token::FUNCTION: outs << "TOKEN(FUNCTION)"; break;
        case Token::PROCEDURE: outs << "TOKEN(PROCEDURE)"; break;
        case Token::TYPE: outs << "TOKEN(TYPE)"; break;
        case Token::VAR: outs << "TOKEN(VAR)"; break;
        case Token::FOR: outs << "TOKEN(FOR)"; break;
        case Token::TO: outs << "TOKEN(TO)"; break;
        case Token::DO: outs << "TOKEN(DO)"; break;
        case Token::DIV: outs << "TOKEN(DIV)"; break;
        case Token::MOD: outs << "TOKEN(MOD)"; break;
        case Token::OR: outs << "TOKEN(OR)"; break;
        case Token::AND: outs << "TOKEN(AND)"; break;
        case Token::NOT: outs << "TOKEN(NOT)"; break;
        case Token::TRUE: outs << "TOKEN(TRUE)"; break;
        case Token::FALSE: outs << "TOKEN(FALSE)"; break;
        case Token::NIL: outs << "TOKEN(NIL)"; break;
        case Token::ARRAY: outs << "TOKEN(ARRAY)"; break;
        case Token::OF: outs << "TOKEN(OF)"; break;
        case Token::POINTER: outs << "TOKEN(POINTER)"; break;

        // Operadores y símbolos
        case Token::PLUS: outs << "TOKEN(PLUS)"; break;
        case Token::MINUS: outs << "TOKEN(MINUS)"; break;
        case Token::MUL: outs << "TOKEN(MUL)"; break;
        case Token::DIV_OP: outs << "TOKEN(DIV_OP)"; break;
        case Token::ASSIGN: outs << "TOKEN(ASSIGN)"; break;
        case Token::EQUAL: outs << "TOKEN(EQUAL)"; break;
        case Token::NOTEQUAL: outs << "TOKEN(NOTEQUAL)"; break;
        case Token::GREATER: outs << "TOKEN(GREATER)"; break;
        case Token::GREATEREQUAL: outs << "TOKEN(GREATEREQUAL)"; break;
        case Token::LESS: outs << "TOKEN(LESS)"; break;
        case Token::LESSEQUAL: outs << "TOKEN(LESSEQUAL)"; break;
        case Token::LBRACKET: outs << "TOKEN(LBRACKET)"; break;
        case Token::RBRACKET: outs << "TOKEN(RBRACKET)"; break;
        case Token::DOTDOT: outs << "TOKEN(DOTDOT)"; break;
        case Token::PD: outs << "TOKEN(PD)"; break;
        case Token::PI: outs << "TOKEN(PI)"; break;
        case Token::PC: outs << "TOKEN(PC)"; break;
        case Token::COMA: outs << "TOKEN(COMA)"; break;

        // Tipos de datos
        case Token::INTEGER: outs << "TOKEN(INTEGER)"; break;
        case Token::LONGINT: outs << "TOKEN(LONGINT)"; break;
        case Token::CHAR: outs << "TOKEN(CHAR)"; break;
        case Token::STRING: outs << "TOKEN(STRING)"; break;
        case Token::BOOLEAN: outs << "TOKEN(BOOLEAN)"; break;
        case Token::INT: outs << "TOKEN(INT)"; break;
        case Token::VOID: outs << "TOKEN(VOID)"; break;

        // Identificadores y literales
        case Token::ID: outs << "TOKEN(ID)"; break;
        case Token::NUM: outs << "TOKEN(NUM)"; break;
        case Token::BOOL: outs << "TOKEN(BOOL)"; break;
        case Token::NIL_LITERAL: outs << "TOKEN(NIL_LITERAL)"; break;

        // Otros
        case Token::ERR: outs << "TOKEN(ERR)"; break;
        case Token::END_OF_FILE: outs << "TOKEN(END_OF_FILE)"; break;

        default: outs << "TOKEN(UNKNOWN)"; break;
    }
    return outs;
}

ostream& operator<<(ostream& outs, const Token* tok) {
    return outs << *tok;
}
