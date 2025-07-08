#include <iostream>
#include "token.h"

using namespace std;

Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = string(1, c); }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first, last);
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    switch (tok.type) {
        case Token::PLUS: outs << "TOKEN(PLUS)"; break;
        case Token::MINUS: outs << "TOKEN(MINUS)"; break;
        case Token::MUL: outs << "TOKEN(MUL)"; break;
        case Token::DIV: outs << "TOKEN(DIV)"; break;
        case Token::NUM: outs << "TOKEN(NUM)"; break;
        case Token::ERR: outs << "TOKEN(ERR)"; break;
        case Token::PD: outs << "TOKEN(PD)"; break;
        case Token::PI: outs << "TOKEN(PI)"; break;
        case Token::END: outs << "TOKEN(END)"; break;
        case Token::ID: outs << "TOKEN(ID)"; break;
        case Token::PRINT: outs << "TOKEN(PRINT)"; break;
        case Token::ASSIGN: outs << "TOKEN(ASSIGN)"; break;
        case Token::PC: outs << "TOKEN(PC)"; break;
        case Token::LT: outs << "TOKEN(LT)"; break;
        case Token::LE: outs << "TOKEN(LE)"; break;
        case Token::EQ: outs << "TOKEN(EQ)"; break;
        case Token::IF: outs << "TOKEN(IF)"; break;
        case Token::THEN: outs << "TOKEN(THEN)"; break;
        case Token::ELSE: outs << "TOKEN(ELSE)"; break;
        case Token::ENDIF: outs << "TOKEN(ENDIF)"; break;
        case Token::WHILE: outs << "TOKEN(WHILE)"; break;
        case Token::COMA: outs << "TOKEN(COMA)"; break;
        case Token::IFEXP: outs << "TOKEN(IFEXP)"; break;
        case Token::DO: outs << "TOKEN(DO)"; break;
        case Token::VAR: outs << "TOKEN(VAR)"; break;
        case Token::FOR : outs << "TOKEN(FOR)"; break;
        case Token::TRUE : outs << "TOKEN(TRUE)"; break;
        case Token::FALSE : outs << "TOKEN(FALSE)"; break;
        case Token::RETURN : outs << "TOKEN(RETURN)"; break;
        case Token::FUN : outs << "TOKEN(FUN)"; break;
        case Token::ENDFUN : outs << "TOKEN(ENDFUN)"; break;
        
        // Nuevos tokens de Pascal
        case Token::PROGRAM: outs << "TOKEN(PROGRAM)"; break;
        case Token::BEGIN: outs << "TOKEN(BEGIN)"; break;
        case Token::DOT: outs << "TOKEN(DOT)"; break;
        case Token::TYPE: outs << "TOKEN(TYPE)"; break;
        case Token::FUNCTION: outs << "TOKEN(FUNCTION)"; break;
        case Token::PROCEDURE: outs << "TOKEN(PROCEDURE)"; break;
        case Token::TO: outs << "TOKEN(TO)"; break;
        case Token::OR: outs << "TOKEN(OR)"; break;
        case Token::AND: outs << "TOKEN(AND)"; break;
        case Token::NOT: outs << "TOKEN(NOT)"; break;
        case Token::DIV_OP: outs << "TOKEN(DIV_OP)"; break;
        case Token::MOD: outs << "TOKEN(MOD)"; break;
        case Token::EQUAL: outs << "TOKEN(EQUAL)"; break;
        case Token::NOTEQUAL: outs << "TOKEN(NOTEQUAL)"; break;
        case Token::GREATER: outs << "TOKEN(GREATER)"; break;
        case Token::GREATEREQUAL: outs << "TOKEN(GREATEREQUAL)"; break;
        case Token::ARRAY: outs << "TOKEN(ARRAY)"; break;
        case Token::OF: outs << "TOKEN(OF)"; break;
        case Token::LBRACKET: outs << "TOKEN(LBRACKET)"; break;
        case Token::RBRACKET: outs << "TOKEN(RBRACKET)"; break;
        case Token::DOTDOT: outs << "TOKEN(DOTDOT)"; break;
        case Token::POINTER: outs << "TOKEN(POINTER)"; break;
        case Token::AT: outs << "TOKEN(AT)"; break;
        case Token::NIL: outs << "TOKEN(NIL)"; break;
        case Token::INTEGER: outs << "TOKEN(INTEGER)"; break;
        case Token::LONGINT: outs << "TOKEN(LONGINT)"; break;
        case Token::BOOLEAN: outs << "TOKEN(BOOLEAN)"; break;
        case Token::CHAR: outs << "TOKEN(CHAR)"; break;
        case Token::REAL: outs << "TOKEN(REAL)"; break;
        case Token::STRING: outs << "TOKEN(STRING)"; break;
        case Token::CONST: outs << "TOKEN(CONST)"; break;
        case Token::OUT: outs << "TOKEN(OUT)"; break;
        case Token::BREAK: outs << "TOKEN(BREAK)"; break;
        case Token::WRITELN: outs << "TOKEN(WRITELN)"; break;
        case Token::COLON: outs << "TOKEN(COLON)"; break;
        case Token::NEW: outs << "TOKEN(NEW)"; break;
        case Token::DISPOSE: outs << "TOKEN(DISPOSE)"; break;
        default: outs << "TOKEN(" << tok.type << ")"; break;
    }
    return outs;
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}