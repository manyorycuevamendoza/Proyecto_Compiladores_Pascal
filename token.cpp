#include "token.h"
#include <iostream>

using namespace std;

Token::Token(Type type): type(type) { text = ""; }

Token::Token(Type type, char c): type(type) { text = string(1, c); }

Token::Token(Type type, const string& source, int first, int last): type(type) {
    text = source.substr(first, last);
}

std::ostream& operator<<(std::ostream& outs, const Token& tok) {
    switch (tok.type) {
        case Token::PLUS: outs << "TOKEN(PLUS)"; break;
        case Token::MINUS: outs << "TOKEN(MINUS)"; break;
        case Token::MUL: outs << "TOKEN(MUL)"; break;
        case Token::DIV: outs << "TOKEN(DIV)"; break;
        case Token::LT: outs << "TOKEN(LT)"; break;
        case Token::LE: outs << "TOKEN(LE)"; break;
        case Token::EQ: outs << "TOKEN(EQ)"; break;
        case Token::ASSIGN: outs << "TOKEN(ASSIGN)"; break;
        case Token::COMA: outs << "TOKEN(COMA)"; break;
        case Token::PC: outs << "TOKEN(PC)"; break;
        case Token::PD: outs << "TOKEN(PD)"; break;
        case Token::PI: outs << "TOKEN(PI)"; break;
        case Token::CORCHETE_A: outs << "TOKEN(CORCHETE_A)"; break;
        case Token::CORCHETE_C: outs << "TOKEN(CORCHETE_C)"; break;
        case Token::PUNTO_PUNTO: outs << "TOKEN(PUNTO_PUNTO)"; break;

        // Palabras clave
        case Token::VAR: outs << "TOKEN(VAR)"; break;
        case Token::ARRAY: outs << "TOKEN(ARRAY)"; break;
        case Token::OF: outs << "TOKEN(OF)"; break;
        case Token::RANGE: outs << "TOKEN(RANGE)"; break;
        case Token::POINTER: outs << "TOKEN(POINTER)"; break;
        case Token::FUN: outs << "TOKEN(FUN)"; break;
        case Token::ENDFUN: outs << "TOKEN(ENDFUN)"; break;
        case Token::RETURN: outs << "TOKEN(RETURN)"; break;
        case Token::IF: outs << "TOKEN(IF)"; break;
        case Token::THEN: outs << "TOKEN(THEN)"; break;
        case Token::ELSE: outs << "TOKEN(ELSE)"; break;
        case Token::ENDIF: outs << "TOKEN(ENDIF)"; break;
        case Token::WHILE: outs << "TOKEN(WHILE)"; break;
        case Token::DO: outs << "TOKEN(DO)"; break;
        case Token::ENDWHILE: outs << "TOKEN(ENDWHILE)"; break;
        case Token::FOR: outs << "TOKEN(FOR)"; break;
        case Token::TO: outs << "TOKEN(TO)"; break;
        case Token::ENDFOR: outs << "TOKEN(ENDFOR)"; break;
        case Token::PRINT: outs << "TOKEN(PRINT)"; break;

        // Tipos de datos
        case Token::INTEGER: outs << "TOKEN(INTEGER)"; break;
        case Token::LONGINT: outs << "TOKEN(LONGINT)"; break;
        case Token::CHAR: outs << "TOKEN(CHAR)"; break;
        case Token::STRING: outs << "TOKEN(STRING)"; break;
        case Token::BOOLEAN: outs << "TOKEN(BOOLEAN)"; break;
        case Token::INT: outs << "TOKEN(INT)"; break;
        case Token::VOID: outs << "TOKEN(VOID)"; break;

        // Constantes y otros
        case Token::TRUE: outs << "TOKEN(TRUE)"; break;
        case Token::FALSE: outs << "TOKEN(FALSE)"; break;
        case Token::NUM: outs << "TOKEN(NUM)"; break;
        case Token::BOOL: outs << "TOKEN(BOOL)"; break;
        case Token::ID: outs << "TOKEN(ID)"; break;
        case Token::ERR: outs << "TOKEN(ERR)"; break;
        case Token::END: outs << "TOKEN(END)"; break;

        default: outs << "TOKEN(UNKNOWN)"; break;
    }
    return outs;
}

std::ostream& operator<<(std::ostream& outs, const Token* tok) {
    return outs << *tok;
}
