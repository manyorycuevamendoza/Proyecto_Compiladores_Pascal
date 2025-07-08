#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

Scanner::Scanner(const char* s):input(s),first(0), current(0) { }


bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token* Scanner::nextToken() {
    Token* token;
    // Ignorar espacios en blanco
    while (current < input.length() &&  is_white_space(input[current]) ) current++;
    if (current >= input.length()) return new Token(Token::END);
    char c  = input[current];
    first = current;
    // Reconocer digitos
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    }

     // 3. Reconocer identificadores y palabras clave
    else if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;
        string word = input.substr(first, current - first);
        if (word == "print") {
            token = new Token(Token::PRINT, word, 0, word.length());
        } else if (word == "if") {
            token = new Token(Token::IF, word, 0, word.length());
        } else if (word == "then") {
            token = new Token(Token::THEN, word, 0, word.length());
        } else if (word == "else") {
            token = new Token(Token::ELSE, word, 0, word.length());
        } else if (word == "endif") {
            token = new Token(Token::ENDIF, word, 0, word.length());
        } else if (word == "ifexp") {
            token = new Token(Token::IFEXP, word, 0, word.length());
        } else if (word == "while") {
            token = new Token(Token::WHILE, word, 0, word.length());
        } else if (word == "do") {
            token = new Token(Token::DO, word, 0, word.length());
        }else if (word == "for") {
            token = new Token(Token::FOR, word, 0, word.length());
        }
        else if (word == "var") {
            token = new Token(Token::VAR, word, 0, word.length());
        }
        else if (word == "true") {
            token = new Token(Token::TRUE, word, 0, word.length());
        }
        else if (word == "false") {
            token = new Token(Token::FALSE, word, 0, word.length());
        }
        else if (word == "return") {
            token = new Token(Token::RETURN, word, 0, word.length());
        }
        else if (word == "fun") {
            token = new Token(Token::FUN, word, 0, word.length());
        }
        else if (word == "endfun") {
            token = new Token(Token::ENDFUN, word, 0, word.length());
        }
        // Nuevas palabras clave de Pascal
        else if (word == "program") {
            token = new Token(Token::PROGRAM, word, 0, word.length());
        } else if (word == "begin") {
            token = new Token(Token::BEGIN, word, 0, word.length());
        } else if (word == "end") {
            token = new Token(Token::END, word, 0, word.length());
        } else if (word == "type") {
            token = new Token(Token::TYPE, word, 0, word.length());
        } else if (word == "function") {
            token = new Token(Token::FUNCTION, word, 0, word.length());
        } else if (word == "procedure") {
            token = new Token(Token::PROCEDURE, word, 0, word.length());
        } else if (word == "to") {
            token = new Token(Token::TO, word, 0, word.length());
        } else if (word == "or") {
            token = new Token(Token::OR, word, 0, word.length());
        } else if (word == "and") {
            token = new Token(Token::AND, word, 0, word.length());
        } else if (word == "not") {
            token = new Token(Token::NOT, word, 0, word.length());
        } else if (word == "div") {
            token = new Token(Token::DIV_OP, word, 0, word.length());
        } else if (word == "mod") {
            token = new Token(Token::MOD, word, 0, word.length());
        } else if (word == "array") {
            token = new Token(Token::ARRAY, word, 0, word.length());
        } else if (word == "of") {
            token = new Token(Token::OF, word, 0, word.length());
        } else if (word == "nil") {
            token = new Token(Token::NIL, word, 0, word.length());
        } else if (word == "Integer") {
            token = new Token(Token::INTEGER, word, 0, word.length());
        } else if (word == "Longint") {
            token = new Token(Token::LONGINT, word, 0, word.length());
        } else if (word == "Boolean") {
            token = new Token(Token::BOOLEAN, word, 0, word.length());
        } else if (word == "Char") {
            token = new Token(Token::CHAR, word, 0, word.length());
        } else if (word == "Real") {
            token = new Token(Token::REAL, word, 0, word.length());
        } else if (word == "String") {
            token = new Token(Token::STRING, word, 0, word.length());
        } else if (word == "const") {
            token = new Token(Token::CONST, word, 0, word.length());
        } else if (word == "out") {
            token = new Token(Token::OUT, word, 0, word.length());
        } else if (word == "break") {
            token = new Token(Token::BREAK, word, 0, word.length());
        } else if (word == "writeln") {
            token = new Token(Token::WRITELN, word, 0, word.length());
        } else if (word == "exit") {
            token = new Token(Token::EXIT, word, 0, word.length());
        } else if (word == "new") {
            token = new Token(Token::NEW, word, 0, word.length());
        } else if (word == "dispose") {
            token = new Token(Token::DISPOSE, word, 0, word.length());
        } else {
            token = new Token(Token::ID, word, 0, word.length());
        }
    }

    else if (c == ':') {
        if (current + 1 < input.length() && input[current + 1] == '=') {
            token = new Token(Token::ASSIGN, ":=", 0, 2);
            current++;
        } else {
            token = new Token(Token::COLON, c);
        }
        current++;
    }
    // 4. Reconocer operadores y delimitadores
    else if (strchr("+-*/()=;,<>[]^@.", c)) {
        switch(c) {
            case '+': token = new Token(Token::PLUS, c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case ',': token = new Token(Token::COMA, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case '[': token = new Token(Token::LBRACKET, c); break;
            case ']': token = new Token(Token::RBRACKET, c); break;
            case '^': token = new Token(Token::POINTER, c); break;
            case '@': token = new Token(Token::AT, c); break;
            case '=':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::EQ, "==", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::EQUAL, c);
                }
                break;
            case '<':
                if (current + 1 < input.length()) {
                    if (input[current + 1] == '=') {
                        token = new Token(Token::LE, "<=", 0, 2);
                        current++;
                    } else if (input[current + 1] == '>') {
                        token = new Token(Token::NOTEQUAL, "<>", 0, 2);
                        current++;
                    } else {
                        token = new Token(Token::LT, c);
                    }
                } else {
                    token = new Token(Token::LT, c);
                }
                break;
            case '>':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::GREATEREQUAL, ">=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::GREATER, c);
                }
                break;
            case '.':
                if (current + 1 < input.length() && input[current + 1] == '.') {
                    token = new Token(Token::DOTDOT, "..", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::DOT, c);
                }
                break;
            case ';': token = new Token(Token::PC, c); break;
            default:
                cout << "No debería llegar acá" << endl;
                token = new Token(Token::ERR, c);
        }
        current++;
    }
    // 5. Reconocer errores
    else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() { }

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl<< endl;
    while ((current = scanner->nextToken())->type != Token::DOT) {
        if (current->type == Token::ERR) {
            cout << "Error en scanner - carácter inválido: " << current->text << endl;
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(DOT)" << endl;
    delete current;
}