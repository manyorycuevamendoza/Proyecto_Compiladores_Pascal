#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

Scanner::Scanner(const char* s) : input(s), first(0), current(0) {}

bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token* Scanner::nextToken() {
    Token* token;
    
    while (current < input.length() && is_white_space(input[current])) current++;
    if (current >= input.length()) return new Token(Token::END_OF_FILE);

    char c = input[current];
    first = current;

    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    }
    else if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;

        string word = input.substr(first, current - first);

        // Palabras clave extendidas
        if (word == "program") token = new Token(Token::PROGRAM, word, 0, word.length());
        else if (word == "begin") token = new Token(Token::BEGIN, word, 0, word.length());
        else if (word == "end") token = new Token(Token::END, word, 0, word.length());
        else if (word == "function") token = new Token(Token::FUNCTION, word, 0, word.length());
        else if (word == "procedure") token = new Token(Token::PROCEDURE, word, 0, word.length());
        else if (word == "type") token = new Token(Token::TYPE, word, 0, word.length());
        else if (word == "var") token = new Token(Token::VAR, word, 0, word.length());
        else if (word == "for") token = new Token(Token::FOR, word, 0, word.length());
        else if (word == "to") token = new Token(Token::TO, word, 0, word.length());
        else if (word == "do") token = new Token(Token::DO, word, 0, word.length());
        else if (word == "div") token = new Token(Token::DIV, word, 0, word.length());
        else if (word == "mod") token = new Token(Token::MOD, word, 0, word.length());
        else if (word == "or") token = new Token(Token::OR, word, 0, word.length());
        else if (word == "and") token = new Token(Token::AND, word, 0, word.length());
        else if (word == "not") token = new Token(Token::NOT, word, 0, word.length());
        else if (word == "true") token = new Token(Token::TRUE, word, 0, word.length());
        else if (word == "false") token = new Token(Token::FALSE, word, 0, word.length());
        else if (word == "nil") token = new Token(Token::NIL, word, 0, word.length());
        else if (word == "array") token = new Token(Token::ARRAY, word, 0, word.length());
        else if (word == "of") token = new Token(Token::OF, word, 0, word.length());
        else {
            token = new Token(Token::ID, word, 0, word.length());
        }
    }
    else if (strchr("+-*/()=;,<>[]:.", c)) {
        switch (c) {
            case '+': token = new Token(Token::PLUS, c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV_OP, c); break;
            case ',': token = new Token(Token::COMA, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case '=': token = new Token(Token::EQUAL, c); break;
            case '[': token = new Token(Token::LBRACKET, c); break;
            case ']': token = new Token(Token::RBRACKET, c); break;
            case ':':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::ASSIGN, ":=", 0, 2);
                    current++;
                } else token = new Token(Token::ERR, c);
                break;
            case '<':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::LESSEQUAL, "<=", 0, 2);
                    current++;
                } else if (current + 1 < input.length() && input[current + 1] == '>') {
                    token = new Token(Token::NOTEQUAL, "<>", 0, 2);
                    current++;
                } else token = new Token(Token::LESS, c);
                break;
            case '>':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::GREATEREQUAL, ">=", 0, 2);
                    current++;
                } else token = new Token(Token::GREATER, c);
                break;
            case '.':
                if (current + 1 < input.length() && input[current + 1] == '.') {
                    token = new Token(Token::DOTDOT, "..", 0, 2);
                    current++;
                } else token = new Token(Token::ERR, c);
                break;
            case ';': token = new Token(Token::PC, c); break;
            default: token = new Token(Token::ERR, c); break;
        }
        current++;
    }
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

Scanner::~Scanner() {}

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "=== Iniciando Scanner ===" << endl;

    while ((current = scanner->nextToken())->type != Token::END_OF_FILE) {
        if (current->type == Token::ERR) {
            cout << "Error: carácter inválido '" << current->text << "'" << endl;
            break;
        }
        cout << *current << endl;
        delete current;
    }
    cout << "TOKEN(END_OF_FILE)" << endl;
    delete current;
}
