#include <iostream>
#include <stdexcept>
#include <vector>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype) {  
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() { //avanza al siguiente token
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() { //verifica si se ha llegado al final del archivo
    return false;
}

Parser::Parser(Scanner* sc):scanner(sc) { //constructor que recibe un puntero al scanner
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

// Función auxiliar para parsear tipos (incluyendo arrays anidados)
string Parser::parseType() {
    string type = "";
    if (match(Token::ARRAY)) {
        type = "array[";
        if (!match(Token::LBRACKET)) {
            cout << "Error: se esperaba '[' después de 'array'." << endl;
            exit(1);
        }
        if (!match(Token::NUM)) {
            cout << "Error: se esperaba un número como límite inferior del array." << endl;
            exit(1);
        }
        type += previous->text;
        if (!match(Token::DOTDOT)) {
            cout << "Error: se esperaba '..' en el rango del array." << endl;
            exit(1);
        }
        type += "..";
        if (!match(Token::NUM)) {
            cout << "Error: se esperaba un número como límite superior del array." << endl;
            exit(1);
        }
        type += previous->text + "]";
        if (!match(Token::RBRACKET)) {
            cout << "Error: se esperaba ']' al final del rango del array." << endl;
            exit(1);
        }
        if (!match(Token::OF)) {
            cout << "Error: se esperaba 'of' después de la declaración de rango de array." << endl;
            exit(1);
        }
        // Recursivamente parsear el tipo base (puede ser otro array)
        type += " of " + parseType();
    } else if (match(Token::POINTER)) {
        type = "^";
        // Permitir varios tipos base para punteros
        if (match(Token::INTEGER)) {
            type += "Integer";
        } else if (match(Token::BOOLEAN)) {
            type += "Boolean";
        } else if (match(Token::CHAR)) {
            type += "Char";
        } else if (match(Token::REAL)) {
            type += "Real";
        } else if (match(Token::STRING)) {
            type += "String";
        } else if (match(Token::LONGINT)) {
            type += "Longint";
        } else {
            cout << "Error: solo se soportan los tipos base Integer, Boolean, Char, Real, String y Longint para punteros." << endl;
            exit(1);
        }
    } else if (match(Token::INTEGER)) {
        type = "Integer";
    } else if (match(Token::BOOLEAN)) {
        type = "Boolean";
    } else if (match(Token::CHAR)) {
        type = "Char";
    } else if (match(Token::REAL)) {
        type = "Real";
    } else if (match(Token::STRING)) {
        type = "String";
    } else if (match(Token::LONGINT)) {
        type = "Longint";
    } else if (match(Token::ID)) {
        // Tipo definido por el usuario
        type = previous->text;
    } else {
        cout << "Error: tipo de variable no soportado o no reconocido." << endl;
        exit(1);
    }
    return type;
}

VarDec* Parser::parseVarDec() {
    VarDec* vd = NULL;
    if (match(Token::VAR)) {
        list<string> ids;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'var'." << endl;
            exit(1);
        }
        ids.push_back(previous->text);
        while (match(Token::COMA)) {
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador después de ','." << endl;
                exit(1);
            }
            ids.push_back(previous->text);
        }
        if (!match(Token::COLON)) {
            cout << "Error: se esperaba ':' después de la lista de identificadores." << endl;
            exit(1);
        }
        // Usar la función auxiliar para parsear tipos
        string type = parseType();
        if (!match(Token::PC)) {
            cout << "Error: se esperaba ';' al final de la declaración." << endl;
            exit(1);
        }
        vd = new VarDec(type, ids);
    }
    return vd;
}

VarDecList* Parser::parseVarDecList() {
    VarDecList* vdl = new VarDecList();

    if (match(Token::VAR)) {
        // Parsear todas las declaraciones de variables hasta encontrar 'begin'
        while (!check(Token::BEGIN)) {
            // Parsear una declaración de variable (sin buscar 'var' porque ya fue consumido)
            list<string> ids;
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador en declaración de variable." << endl;
                exit(1);
            }
            ids.push_back(previous->text);
            while (match(Token::COMA)) {
                if (!match(Token::ID)) {
                    cout << "Error: se esperaba un identificador después de ','." << endl;
                    exit(1);
                }
                ids.push_back(previous->text);
            }
            if (!match(Token::COLON)) {
                cout << "Error: se esperaba ':' después de la lista de identificadores." << endl;
                exit(1);
            }
            // Usar la función auxiliar para parsear tipos
            string type = parseType();
            if (!match(Token::PC)) {
                cout << "Error: se esperaba ';' al final de la declaración." << endl;
                exit(1);
            }
            VarDec* vd = new VarDec(type, ids);
            vdl->add(vd);
        }
    } else {
    }
    return vdl;
}

StatementList* Parser::parseStatementList() {
    StatementList* sl = new StatementList();
    sl->add(parseStatement());
    while (match(Token::PC)) {
        // Solo parsear otro statement si el siguiente token es válido para iniciar un statement
        if (check(Token::ID) || check(Token::WRITELN) || check(Token::IF) || check(Token::WHILE) || check(Token::FOR) || check(Token::BREAK) || check(Token::EXIT) || check(Token::NEW) || check(Token::DISPOSE)) {
            sl->add(parseStatement());
        } else {
            // Si el siguiente token no es válido para iniciar un statement, salir del bucle
            break;
        }
    }
    return sl;
}

Body* Parser::parseBody() {
    if (!match(Token::BEGIN)) {
        cout << "Error: se esperaba 'begin' al inicio del bloque." << endl;
        exit(1);
    }
    StatementList* sl = parseStatementList();
    if (!match(Token::END)) {
        cout << "Error: se esperaba 'end' al final del bloque. Token actual: " << *current << endl;
        exit(1);
    }
    return new Body(new VarDecList(), sl);
}

FunctionDef* Parser::parseFunctionDeclaration() {
    
    if (!match(Token::FUNCTION)) {
        cout << "Error: se esperaba 'function'." << endl;
        exit(1);
    }
    
    if (!match(Token::ID)) {
        cout << "Error: se esperaba el nombre de la función." << endl;
        exit(1);
    }
    string funcName = previous->text;
    
    if (!match(Token::PI)) {
        cout << "Error: se esperaba '(' después del nombre de la función." << endl;
        exit(1);
    }
    
    // Parsear parámetros
    vector<string> params;

    //verificacion ----------------------------------------------------------
    if (!check(Token::PD)) {  // Si no es paréntesis derecho, hay parámetros
        // Parsear lista de parámetros: id, id, id: tipo
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador de parámetro." << endl;
            exit(1);
        }
        params.push_back(previous->text);
        
        //en caso de que haya mas de un parametro, se parsea la lista de parametros
        while (match(Token::COMA)) {
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador de parámetro después de ','." << endl;
                exit(1);
            }
            params.push_back(previous->text);
        }
        
        // Después de los parámetros debe venir el tipo
        if (!match(Token::COLON)) {
            cout << "Error: se esperaba ':' después de los parámetros." << endl;
            exit(1);
        }
        
        // Parsear tipo de parámetros (puede ser puntero, array, etc.)
        string paramType = parseType();
    }
    
    if (!match(Token::PD)) {
        cout << "Error: se esperaba ')' después de los parámetros." << endl;
        exit(1);
    }
    
    if (!match(Token::COLON)) {
        cout << "Error: se esperaba ':' después de los parámetros." << endl;
        exit(1);
    }
    
    // Parsear tipo de retorno
    if (!match(Token::INTEGER) && !match(Token::BOOLEAN) && !match(Token::CHAR) && 
        !match(Token::REAL) && !match(Token::STRING) && !match(Token::LONGINT)) {
        cout << "Error: se esperaba un tipo de retorno válido." << endl;
        exit(1);
    }
    string returnType = previous->text;//tipo de retorno
    
    if (!match(Token::PC)) {
        cout << "Error: se esperaba ';' después del tipo de retorno." << endl;
        exit(1);
    }
    
    // Parsear el cuerpo de la función
    VarDecList* localVars = new VarDecList();

    if (check(Token::VAR)) {
        if (!match(Token::VAR)) {
            cout << "Error: se esperaba 'var'." << endl;
            exit(1);
        }
        
        // Parsear declaraciones de variables locales hasta encontrar 'begin'
        while (!check(Token::BEGIN)) {
            list<string> ids;
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador en declaración de variable local." << endl;
                exit(1);
            }
            ids.push_back(previous->text);
            while (match(Token::COMA)) {
                if (!match(Token::ID)) {
                    cout << "Error: se esperaba un identificador después de ','." << endl;
                    exit(1);
                }
                ids.push_back(previous->text);
            }
            if (!match(Token::COLON)) {
                cout << "Error: se esperaba ':' después de la lista de identificadores." << endl;
                exit(1);
            }
            string type = parseType();
            if (!match(Token::PC)) {
                cout << "Error: se esperaba ';' al final de la declaración." << endl;
                exit(1);
            }
            VarDec* vd = new VarDec(type, ids);
            localVars->add(vd);
        }
    }
    
    if (!match(Token::BEGIN)) {
        cout << "Error: se esperaba 'begin' en el cuerpo de la función." << endl;
        exit(1);
    }
    
    // Parsear el cuerpo como un bloque de sentencias
    StatementList* sl = parseStatementList();
    
    if (!match(Token::END)) {
        cout << "Error: se esperaba 'end' al final de la función." << endl;
        exit(1);
    }
    
    if (!match(Token::PC)) {
        cout << "Error: se esperaba ';' al final de la función." << endl;
        exit(1);
    }
    
    Body* funcBody = new Body(localVars, sl);
    FunctionDef* func = new FunctionDef(funcName, params, returnType, funcBody);
    
    return func;
}

Program* Parser::parseProgram() {
    if (!match(Token::PROGRAM)) {
        cout << "Error: se esperaba 'program' al inicio del archivo." << endl;
        exit(1);
    }
    if (!match(Token::ID)) {
        cout << "Error: se esperaba el nombre del programa." << endl;
        exit(1);
    }
    if (!match(Token::PC)) {
        cout << "Error: se esperaba ';' después del nombre del programa." << endl;
        exit(1);
    }

    VarDecList* globalVars = new VarDecList(); 
    FunctionList* functions = new FunctionList();

    // Consumir todas las secciones TYPE y VAR antes del bloque principal
    while (check(Token::TYPE) || check(Token::VAR) || check(Token::FUNCTION)) {
        if (check(Token::TYPE)) {
            match(Token::TYPE);
            while (!check(Token::FUNCTION) && !check(Token::VAR) && !check(Token::BEGIN)) {
                if (!match(Token::ID)) {
                    cout << "Error: se esperaba un identificador de tipo." << endl;
                    exit(1);
                }
                if (!match(Token::EQUAL)) {
                    cout << "Error: se esperaba '=' después del nombre del tipo." << endl;
                    exit(1);
                }
                string typeDef = parseType();
                if (!match(Token::PC)) {
                    cout << "Error: se esperaba ';' al final de la declaración de tipo." << endl;
                    exit(1);
                }
            }
        }
        else if (check(Token::FUNCTION)) { 
            FunctionDef* func = parseFunctionDeclaration(); 
            functions->add(func);
        }
        else if (check(Token::VAR)) {
            // Consumir toda la sección VAR y recolectar las declaraciones
            match(Token::VAR);
            while (!check(Token::FUNCTION) && !check(Token::BEGIN)) {
                // Parsear una declaración de variable
                list<string> ids;
                if (!match(Token::ID)) {
                    cout << "Error: se esperaba un identificador en declaración de variable." << endl;
                    exit(1);
                }
                ids.push_back(previous->text);
                while (match(Token::COMA)) {
                    if (!match(Token::ID)) {
                        cout << "Error: se esperaba un identificador después de ','." << endl;
                        exit(1);
                    }
                    ids.push_back(previous->text);
                }
                if (!match(Token::COLON)) {
                    cout << "Error: se esperaba ':' después de la lista de identificadores." << endl;
                    exit(1);
                }
                string type = parseType();
                if (!match(Token::PC)) {
                    cout << "Error: se esperaba ';' al final de la declaración." << endl;
                    exit(1);
                }
                // Agregar la declaración a la lista global
                VarDec* vd = new VarDec(type, ids);
                globalVars->add(vd);
            }
        }
    }

    // Ahora parsear el bloque principal
    if (!match(Token::BEGIN)) {
        cout << "Error: se esperaba 'begin' al inicio del bloque principal." << endl;
        exit(1);
    }
    StatementList* sl = parseStatementList();
    if (!match(Token::END)) {
        cout << "Error: se esperaba 'end' al final del bloque principal." << endl;
        exit(1);
    }
    if (!match(Token::DOT)) {
        cout << "Error: se esperaba '.' al final del programa." << endl;
        exit(1);
    }
    return new Program(functions, new Body(globalVars, sl));
}

list<Stm*> Parser::parseStmList() {
    list<Stm*> slist;
    slist.push_back(parseStatement());
    while(match(Token::PC)) {
        slist.push_back(parseStatement());
    }
    return slist;
}

Stm* Parser::parseStatement() {
    Stm* s = NULL;
    Exp* e = NULL;
    Body* tb = NULL; //true case
    Body* fb = NULL; //false case


    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }

    if (match(Token::ID)) {
        string lex = previous->text; //lex es el identificador

        // Verificar si es acceso a array o puntero
        if (check(Token::LBRACKET)) {
            // Es acceso a array: arr[0] := ... o arr[0][1] := ...
            
            // Parsear el primer acceso al array
            if (!match(Token::LBRACKET)) {
                cout << "Error: se esperaba '[' después del nombre del array." << endl;
                exit(1);
            }
            Exp* firstIndex = parseExprRel();
            if (!match(Token::RBRACKET)) {
                cout << "Error: se esperaba ']' después del índice del array." << endl;
                exit(1);
            }
            
            // Crear el primer acceso al array
            Exp* arrayAccess = new ArrayAccessExp(lex, firstIndex);
            // No permitir más índices (solo unidimensional)
            if (!match(Token::ASSIGN)) {
                cout << "Error: se esperaba ':=' después del acceso al array." << endl;
                exit(1);
            }
            Exp* value = parseExprRel();
            s = new AssignStatement(arrayAccess, value);
        }
        else if (check(Token::POINTER)) {
            // Es dereferencia de puntero: p^ := ...
            if (!match(Token::POINTER)) {
                cout << "Error: se esperaba '^' después del nombre del puntero." << endl;
                exit(1);
            }
            if (!match(Token::ASSIGN)) {
                cout << "Error: se esperaba ':=' después de la dereferencia del puntero." << endl;
                exit(1);
            }
            Exp* value = parseExprRel();
            Exp* pointerDeref = new PointerDerefExp(lex);
            s = new AssignStatement(pointerDeref, value);
        }
        else if (match(Token::ASSIGN)) {
            Exp* value = parseExprRel();
            Exp* var = new IdentifierExp(lex);
            s = new AssignStatement(var, value);
        }
        else {
            cout << "Error: se esperaba ':=' después del identificador." << endl;
            exit(1);
        }

    } else if (match(Token::WRITELN)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'writeln'." << endl;
            exit(1);
        }
        std::vector<Exp*> exprs;
        exprs.push_back(parseExprRel());
        while (match(Token::COMA)) {
            exprs.push_back(parseExprRel());
        }
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(exprs);
    }
    else if (match(Token::IF)) {
        e = parseExprRel();
        if (!match(Token::THEN)) {
            cout << "Error: se esperaba 'then' después de la expresión." << endl;
            exit(1);
        }
        
        // Parsear el bloque then - puede ser una sentencia simple o un bloque BEGIN/END
        if (check(Token::BEGIN)) {
            tb = parseBody();
        } else {
            // Bloque simple - solo una sentencia
            StatementList* sl = new StatementList();
            sl->add(parseStatement());
            tb = new Body(new VarDecList(), sl);
        }

        if (match(Token::ELSE)) {
            // Parsear el bloque else - puede ser una sentencia simple o un bloque BEGIN/END
            if (check(Token::BEGIN)) {
                fb = parseBody();
            } else {
                // Bloque simple - solo una sentencia
                StatementList* sl = new StatementList();
                sl->add(parseStatement());
                fb = new Body(new VarDecList(), sl);
            }
        }
        s = new IfStatement(e, tb, fb);
    }
    else if (match(Token::WHILE)) {
        e = parseExprRel();
        if (!match(Token::DO)) {
            cout << "Error: se esperaba 'do' después de la expresión." << endl;
            exit(1);
        }
        // Permitir bloque o sentencia simple
        if (check(Token::BEGIN)) {
            tb = parseBody();
        } else {
            StatementList* sl = new StatementList();
            sl->add(parseStatement());
            tb = new Body(new VarDecList(), sl);
        }
        s = new WhileStatement(e, tb);
    }
    
    else if (match(Token::FOR)) {
        // for id := expr to expr do ...
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'for'." << endl;
            exit(1);
        }
        string var = previous->text;
        if (!match(Token::ASSIGN)) {
            cout << "Error: se esperaba ':=' después del identificador en for." << endl;
            exit(1);
        }
        Exp* start = parseExprRel();
        if (!match(Token::TO)) {
            cout << "Error: se esperaba 'to' en for." << endl;
            exit(1);
        }
        Exp* end = parseExprRel();
        if (!match(Token::DO)) {
            cout << "Error: se esperaba 'do' en for." << endl;
            exit(1);
        }
        Body* body = nullptr;
        if (check(Token::BEGIN)) {
            body = parseBody();
        } else {
            StatementList* sl = new StatementList();
            sl->add(parseStatement());
            body = new Body(new VarDecList(), sl);
        }
        s = new ForStatement(var, start, end, body);
    }
    else if (match(Token::BREAK)) {
        s = new BreakStatement();
    }
    else if (match(Token::EXIT)) {
        s = new ExitStatement();
    }
    else if (match(Token::NEW)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba '(' después de 'new'." << endl;
            exit(1);
        }
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador de puntero después de '('." << endl;
            exit(1);
        }
        string pointerName = previous->text;
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después del identificador de puntero." << endl;
            exit(1);
        }
        s = new NewStatement(pointerName);
    }
    else if (match(Token::DISPOSE)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba '(' después de 'dispose'." << endl;
            exit(1);
        }
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador de puntero después de '('." << endl;
            exit(1);
        }
        string pointerName = previous->text;
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después del identificador de puntero." << endl;
            exit(1);
        }
        s = new DisposeStatement(pointerName);
    }
    else {
        cout << "Error: Se esperaba un identificador, 'writeln', 'if', 'while', 'for', 'break', 'exit', 'new' o 'dispose', pero se encontró: " << *current << endl;
        exit(1);
    }
    return s;
}

Exp* Parser::parseExprRel() {
    Exp* left = parseExprOr();
    //en caso de que haya mas de un operador de comparación, se parsea la lista de operadores de comparación
    while (match(Token::LT) || match(Token::LE) || match(Token::EQ) || match(Token::EQUAL) || match(Token::GREATER) || match(Token::GREATEREQUAL) || match(Token::NOTEQUAL)) {
        BinaryOp op; //operador de comparación
        if (previous->type == Token::LT) op = LT_OP;
        else if (previous->type == Token::LE) op = LE_OP;
        else if (previous->type == Token::EQ) op = EQ_OP;
        else if (previous->type == Token::EQUAL) op = EQ_OP;
        else if (previous->type == Token::GREATER) op = GT_OP;
        else if (previous->type == Token::GREATEREQUAL) op = GE_OP;
        else if (previous->type == Token::NOTEQUAL) op = NE_OP;
        Exp* right = parseExprOr();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseExprOr() {
    Exp* left = parseExprAnd(); //se parsea la expresión lógica AND
    while (match(Token::OR)) {
        Exp* right = parseExprAnd(); //se parsea la expresión lógica AND
        left = new BinaryExp(left, right, OR_OP); //se crea una nueva expresión lógica OR
    }
    return left;
}

Exp* Parser::parseExprAnd() {
    Exp* left = parseExprNot();
    while (match(Token::AND)) {
        Exp* right = parseExprNot();
        left = new BinaryExp(left, right, AND_OP); //se crea una nueva expresión lógica AND
    }
    return left;
}

Exp* Parser::parseExprNot() {
    if (match(Token::NOT)) {
        Exp* operand = parseExprNot(); // Recursivo para manejar múltiples NOT
        return new UnaryExp(operand, NOT_OP); //se crea una nueva expresión lógica NOT
    }
    Exp* result = parseExprAdd();
    return result;
}

Exp* Parser::parseExprAdd() { //se parsea la expresión aritmética
    Exp* left = parseTerm(); //se parsea el termino aritmético
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS) op = PLUS_OP;
        else op = MINUS_OP;
        Exp* right = parseTerm(); //se parsea el termino aritmético
        left = new BinaryExp(left, right, op); //se crea una nueva expresión aritmética
    }
    return left;
}

Exp* Parser::parseTerm() {//se parsea el termino aritmético
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV) || match(Token::DIV_OP) || match(Token::MOD)) {
        BinaryOp op;
        if (previous->type == Token::MUL) op = MUL_OP;
        else if (previous->type == Token::DIV) op = DIV_OP;
        else if (previous->type == Token::DIV_OP) op = DIV_OP;
        else op = MOD_OP;
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    Exp* e;
    Exp* e1;
    Exp* e2;
    if (match(Token::TRUE)){
        return new BoolExp(1);
    }else if (match(Token::FALSE)){
        return new BoolExp(0);
    }
    else if (match(Token::MINUS)) {
        // Número negativo
        if (!match(Token::NUM)) {
            cout << "Error: se esperaba un número después del signo '-'." << endl;
            exit(1);
        }
        return new NumberExp(-stoi(previous->text)); //
    }
    else if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::AT)) {
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de '@'." << endl;
            exit(1);
        }
        return new AddressOfExp(previous->text);
    }
    else if (match(Token::ID)) {
        string texto = previous->text;
        if (match(Token::LBRACKET)) {
            Exp* firstIndex = parseExprRel();
            if (!match(Token::RBRACKET)) {
                cout << "Error: se esperaba ']' después del índice del array." << endl;
                exit(1);
            }
            
            // Crear el primer acceso al array
            Exp* arrayAccess = new ArrayAccessExp(texto, firstIndex);
            // No permitir más índices (solo unidimensional)
            return arrayAccess;
        }
        else if (match(Token::POINTER)) {
            return new PointerDerefExp(texto);
        }
        else if (check(Token::PI)) {
            // Llamada a función
            advance(); // consumir '('
            std::vector<Exp*> args;
            if (!check(Token::PD)) { // Si no es ')', hay argumentos
                args.push_back(parseExprRel());
                while (match(Token::COMA)) {
                    args.push_back(parseExprRel());
                }
            }
            if (!match(Token::PD)) {
                cout << "Error: se esperaba ')' al final de la llamada a función." << endl;
                exit(1);
            }
            return new FunctionCallExp(texto, args);
        }
        return new IdentifierExp(previous->text);
    }
    else if (match(Token::PI)){
        e = parseExprRel();
        if (!match(Token::PD)){
            cout << "Error: falta paréntesis derecho" << endl;
            exit(1);
        }
        return e;
    }
    cout << "Error: se esperaba un número o identificador." << endl;
    exit(1);
}

