#include "type_checker.hh"
#include <iostream>

using namespace std;

TypeChecker::TypeChecker() {
    type_env.add_level();
}

TypeChecker::~TypeChecker() {}

void TypeChecker::check(Program* program) {
    cout << "🔍 Iniciando verificación de tipos..." << endl;
    type_env.clear();
    functions.clear();
    type_definitions.clear();
    type_env.add_level();
    
    try {
        program->accept(this);
        cout << "✅ Verificación de tipos exitosa" << endl;
    } catch (const exception& e) {
        cout << "❌ Error en verificación de tipos: " << e.what() << endl;
        exit(1);
    }
}

ImpType TypeChecker::get_type_from_string(const std::string& type_name) {
    ImpType type;
    
    if (type_name == "Integer" || type_name == "Longint") {
        type.set_basic_type(ImpType::INT);
    } else if (type_name == "Boolean") {
        type.set_basic_type(ImpType::BOOL);
    } else if (type_name == "Char") {
        type.set_basic_type(ImpType::INT); // Tratar char como int por simplicidad
    } else if (type_name == "Real") {
        type.set_basic_type(ImpType::INT); // Tratar real como int por simplicidad
    } else if (type_name == "String") {
        type.set_basic_type(ImpType::INT); // Tratar string como int por simplicidad
    } else if (type_name.find("array") != std::string::npos) {
        // Para arrays, usar INT como tipo base por simplicidad
        type.set_basic_type(ImpType::INT);
    } else if (!type_name.empty() && type_name[0] == '^') {
        // Para punteros, usar INT como tipo base por simplicidad
        type.set_basic_type(ImpType::INT);
    } else {
        // Tipo definido por usuario - buscar en type_definitions
        auto it = type_definitions.find(type_name);
        if (it != type_definitions.end()) {
            type = it->second;
        } else {
            // Por defecto, asumir INT
            type.set_basic_type(ImpType::INT);
        }
    }
    
    return type;
}

bool TypeChecker::is_compatible(ImpType t1, ImpType t2) {
    return t1.match(t2);
}

void TypeChecker::check_expression_type(Exp* exp, ImpType expected_type, const std::string& context) {
    ImpType actual_type = exp->accept(this);
    if (!is_compatible(actual_type, expected_type)) {
        cout << "❌ Error de tipo en " << context << ": se esperaba ";
        cout << expected_type << " pero se obtuvo " << actual_type << endl;
        exit(1);
    }
}

// Implementación de visitas para expresiones
ImpType TypeChecker::visit(NumberExp* e) {
    ImpType type;
    type.set_basic_type(ImpType::INT);
    return type;
}

ImpType TypeChecker::visit(BoolExp* e) {
    ImpType type;
    type.set_basic_type(ImpType::BOOL);
    return type;
}

ImpType TypeChecker::visit(IdentifierExp* e) {
    if (!type_env.check(e->name)) {
        cout << "❌ Error: variable '" << e->name << "' no declarada" << endl;
        exit(1);
    }
    return type_env.lookup(e->name);
}

ImpType TypeChecker::visit(BinaryExp* e) {
    ImpType left_type = e->left->accept(this);
    ImpType right_type = e->right->accept(this);
    ImpType result_type;
    
    switch (e->op) {
        case PLUS_OP:
        case MINUS_OP:
        case MUL_OP:
        case DIV_OP:
        case MOD_OP:
            // Operadores aritméticos requieren operandos enteros
            if (left_type.ttype != ImpType::INT || right_type.ttype != ImpType::INT) {
                cout << "❌ Error: operadores aritméticos requieren operandos enteros" << endl;
                exit(1);
            }
            result_type.set_basic_type(ImpType::INT);
            break;
            
        case LT_OP:
        case LE_OP:
        case EQ_OP:
        case GT_OP:
        case GE_OP:
        case NE_OP:
            // Operadores de comparación requieren operandos del mismo tipo
            if (!is_compatible(left_type, right_type)) {
                cout << "❌ Error: operandos de comparación deben ser del mismo tipo" << endl;
                exit(1);
            }
            result_type.set_basic_type(ImpType::BOOL);
            break;
            
        case AND_OP:
        case OR_OP:
            // Operadores lógicos requieren operandos booleanos
            if (left_type.ttype != ImpType::BOOL || right_type.ttype != ImpType::BOOL) {
                cout << "❌ Error: operadores lógicos requieren operandos booleanos" << endl;
                exit(1);
            }
            result_type.set_basic_type(ImpType::BOOL);
            break;
            
        default:
            cout << "❌ Error: operador binario no soportado" << endl;
            exit(1);
    }
    
    return result_type;
}

ImpType TypeChecker::visit(UnaryExp* e) {
    ImpType operand_type = e->operand->accept(this);
    ImpType result_type;
    
    switch (e->op) {
        case NOT_OP:
            if (operand_type.ttype != ImpType::BOOL) {
                cout << "❌ Error: operador 'not' requiere operando booleano" << endl;
                exit(1);
            }
            result_type.set_basic_type(ImpType::BOOL);
            break;
            
        case NEG_OP:
            if (operand_type.ttype != ImpType::INT) {
                cout << "❌ Error: operador '-' requiere operando entero" << endl;
                exit(1);
            }
            result_type.set_basic_type(ImpType::INT);
            break;
            
        default:
            cout << "❌ Error: operador unario no soportado" << endl;
            exit(1);
    }
    
    return result_type;
}

ImpType TypeChecker::visit(ArrayAccessExp* e) {
    ImpType array_type = type_env.lookup(e->arrayName);
    ImpType index_type = e->index->accept(this);
    
    if (index_type.ttype != ImpType::INT) {
        cout << "❌ Error: índice de array debe ser entero" << endl;
        exit(1);
    }
    
    // Por simplicidad, asumimos que el tipo del array es INT
    ImpType result_type;
    result_type.set_basic_type(ImpType::INT);
    return result_type;
}

ImpType TypeChecker::visit(PointerDerefExp* e) {
    ImpType pointer_type = type_env.lookup(e->pointerName);
    
    // Por simplicidad, asumimos que el tipo del puntero es INT
    ImpType result_type;
    result_type.set_basic_type(ImpType::INT);
    return result_type;
}

ImpType TypeChecker::visit(AddressOfExp* e) {
    if (!type_env.check(e->variableName)) {
        cout << "❌ Error: variable '" << e->variableName << "' no declarada" << endl;
        exit(1);
    }
    
    // Por simplicidad, asumimos que el tipo de la dirección es INT
    ImpType result_type;
    result_type.set_basic_type(ImpType::INT);
    return result_type;
}

ImpType TypeChecker::visit(FunctionCallExp* e) {
    auto it = functions.find(e->funcName);
    if (it == functions.end()) {
        cout << "❌ Error: función '" << e->funcName << "' no definida" << endl;
        exit(1);
    }
    
    FunctionDef* func = it->second;
    
    // Verificar número de argumentos
    if (e->args.size() != func->params.size()) {
        cout << "❌ Error: número incorrecto de argumentos para función '" << e->funcName << "'" << endl;
        exit(1);
    }
    
    // Verificar tipos de argumentos
    for (size_t i = 0; i < e->args.size(); ++i) {
        ImpType arg_type = e->args[i]->accept(this);
        // Por simplicidad, asumimos que todos los parámetros son INT
        ImpType expected_type;
        expected_type.set_basic_type(ImpType::INT);
        
        if (!is_compatible(arg_type, expected_type)) {
            cout << "❌ Error: tipo incorrecto en argumento " << (i+1) << " de función '" << e->funcName << "'" << endl;
            exit(1);
        }
    }
    
    // Retornar tipo de retorno de la función
    return get_type_from_string(func->returnType);
}

// Implementación de visitas para sentencias
void TypeChecker::visit(Program* p) {
    // Procesar funciones primero
    if (p->functions) {
        p->functions->accept(this);
    }
    // Luego procesar el cuerpo principal
    p->body->accept(this);
}

void TypeChecker::visit(Body* b) {
    type_env.add_level();
    b->vardecs->accept(this);
    b->slist->accept(this);
    type_env.remove_level();
}

void TypeChecker::visit(VarDecList* vdl) {
    for (auto vardec : vdl->vardecs) {
        vardec->accept(this);
    }
}

void TypeChecker::visit(VarDec* vd) {
    ImpType var_type = get_type_from_string(vd->type);
    
    for (const string& var_name : vd->vars) {
        if (type_env.check(var_name)) {
            cout << "❌ Error: variable '" << var_name << "' ya declarada" << endl;
            exit(1);
        }
        type_env.add_var(var_name, var_type);
        cout << "✅ Variable '" << var_name << "' declarada con tipo " << var_type << endl;
    }
}

void TypeChecker::visit(StatementList* sl) {
    for (auto stm : sl->stms) {
        stm->accept(this);
    }
}

void TypeChecker::visit(AssignStatement* s) {
    ImpType rhs_type = s->rhs->accept(this);
    
    if (IdentifierExp* id = dynamic_cast<IdentifierExp*>(s->lhs)) {
        if (!type_env.check(id->name)) {
            cout << "❌ Error: variable '" << id->name << "' no declarada" << endl;
            exit(1);
        }
        ImpType lhs_type = type_env.lookup(id->name);
        if (!is_compatible(lhs_type, rhs_type)) {
            cout << "❌ Error: tipos incompatibles en asignación a '" << id->name << "'" << endl;
            exit(1);
        }
    } else if (ArrayAccessExp* arr = dynamic_cast<ArrayAccessExp*>(s->lhs)) {
        // Verificar que el array existe y el índice es entero
        if (!type_env.check(arr->arrayName)) {
            cout << "❌ Error: array '" << arr->arrayName << "' no declarado" << endl;
            exit(1);
        }
        ImpType index_type = arr->index->accept(this);
        if (index_type.ttype != ImpType::INT) {
            cout << "❌ Error: índice de array debe ser entero" << endl;
            exit(1);
        }
    } else if (PointerDerefExp* ptr = dynamic_cast<PointerDerefExp*>(s->lhs)) {
        if (!type_env.check(ptr->pointerName)) {
            cout << "❌ Error: puntero '" << ptr->pointerName << "' no declarado" << endl;
            exit(1);
        }
    }
    
    cout << "✅ Asignación verificada correctamente" << endl;
}

void TypeChecker::visit(PrintStatement* s) {
    for (auto expr : s->exprs) {
        expr->accept(this); // Verificar que la expresión es válida
    }
    cout << "✅ Print statement verificado correctamente" << endl;
}

void TypeChecker::visit(IfStatement* s) {
    ImpType cond_type = s->condition->accept(this);
    if (cond_type.ttype != ImpType::BOOL) {
        cout << "❌ Error: condición de if debe ser booleana" << endl;
        exit(1);
    }
    
    s->then->accept(this);
    if (s->els) {
        s->els->accept(this);
    }
    cout << "✅ If statement verificado correctamente" << endl;
}

void TypeChecker::visit(WhileStatement* s) {
    ImpType cond_type = s->condition->accept(this);
    if (cond_type.ttype != ImpType::BOOL) {
        cout << "❌ Error: condición de while debe ser booleana" << endl;
        exit(1);
    }
    
    s->b->accept(this);
    cout << "✅ While statement verificado correctamente" << endl;
}

void TypeChecker::visit(ForStatement* s) {
    // Verificar que la variable del bucle existe
    if (!type_env.check(s->var)) {
        cout << "❌ Error: variable '" << s->var << "' no declarada en bucle for" << endl;
        exit(1);
    }
    
    ImpType start_type = s->start->accept(this);
    ImpType end_type = s->end->accept(this);
    
    if (start_type.ttype != ImpType::INT || end_type.ttype != ImpType::INT) {
        cout << "❌ Error: límites del bucle for deben ser enteros" << endl;
        exit(1);
    }
    
    s->body->accept(this);
    cout << "✅ For statement verificado correctamente" << endl;
}

void TypeChecker::visit(BreakStatement* s) {
    cout << "✅ Break statement verificado correctamente" << endl;
}

void TypeChecker::visit(ExitStatement* s) {
    cout << "✅ Exit statement verificado correctamente" << endl;
}

void TypeChecker::visit(NewStatement* s) {
    if (!type_env.check(s->pointerName)) {
        cout << "❌ Error: puntero '" << s->pointerName << "' no declarado" << endl;
        exit(1);
    }
    cout << "✅ New statement verificado correctamente" << endl;
}

void TypeChecker::visit(DisposeStatement* s) {
    if (!type_env.check(s->pointerName)) {
        cout << "❌ Error: puntero '" << s->pointerName << "' no declarado" << endl;
        exit(1);
    }
    cout << "✅ Dispose statement verificado correctamente" << endl;
}

void TypeChecker::visit(FunctionDef* func) {
    functions[func->name] = func;
    cout << "✅ Función '" << func->name << "' registrada para verificación de tipos" << endl;
}

void TypeChecker::visit(FunctionList* funcs) {
    for (auto func : funcs->functions) {
        func->accept(this);
    }
} 