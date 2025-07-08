#include "imp_interpreter.hh"
#include <iostream>

using namespace std;

ImpInterpreter::ImpInterpreter() {
    env.add_level(); // Crear el primer nivel del entorno
}
ImpInterpreter::~ImpInterpreter() {}

void ImpInterpreter::interpret(Program* p) {
    env.clear();
    functions.clear();
    env.add_level();
    p->accept(this);
}

// Expresiones
ImpValue ImpInterpreter::visit(NumberExp* e) {
    return ImpValue(e->value);
}

ImpValue ImpInterpreter::visit(BoolExp* e) {
    return ImpValue((bool)e->value);
}

ImpValue ImpInterpreter::visit(IdentifierExp* e) {
    if (!env.check(e->name)) {
        cout << "Error: variable no definida: " << e->name << endl;
        exit(1);
    }
    return env.lookup(e->name);
}

ImpValue ImpInterpreter::visit(BinaryExp* e) {
    ImpValue v1 = e->left->accept(this);
    ImpValue v2 = e->right->accept(this);
    ImpValue result;
    switch (e->op) {
        case PLUS_OP:
            result = ImpValue(v1.int_value + v2.int_value);
            break;
        case MINUS_OP:
            result = ImpValue(v1.int_value - v2.int_value);
            break;
        case MUL_OP:
            result = ImpValue(v1.int_value * v2.int_value);
            break;
        case DIV_OP:
            if (v2.int_value == 0) {
                cout << "Error: división por cero" << endl;
                exit(1);
            }
            result = ImpValue(v1.int_value / v2.int_value);
            break;
        case MOD_OP:
            if (v2.int_value == 0) {
                cout << "Error: módulo por cero" << endl;
                exit(1);
            }
            result = ImpValue(v1.int_value % v2.int_value);
            break;
        case LT_OP:
            result = ImpValue(v1.int_value < v2.int_value);
            result.type = TBOOL;
            break;
        case LE_OP:
            result = ImpValue(v1.int_value <= v2.int_value);
            result.type = TBOOL;
            break;
        case EQ_OP:
            result = ImpValue(v1.int_value == v2.int_value);
            result.type = TBOOL;
            break;
        case GT_OP:
            result = ImpValue(v1.int_value > v2.int_value);
            result.type = TBOOL;
            break;
        case GE_OP:
            result = ImpValue(v1.int_value >= v2.int_value);
            result.type = TBOOL;
            break;
        case NE_OP:
            result = ImpValue(v1.int_value != v2.int_value);
            result.type = TBOOL;
            break;
        case AND_OP:
            if (v1.type != TBOOL || v2.type != TBOOL) {
                cout << "Error: operador 'and' requiere operandos booleanos" << endl;
                exit(1);
            }
            result = ImpValue(v1.bool_value && v2.bool_value);
            result.type = TBOOL;
            break;
        case OR_OP:
            if (v1.type != TBOOL || v2.type != TBOOL) {
                cout << "Error: operador 'or' requiere operandos booleanos" << endl;
                exit(1);
            }
            result = ImpValue(v1.bool_value || v2.bool_value);
            result.type = TBOOL;
            break;
        default:
            cout << "Operador binario no implementado" << endl;
            exit(1);
    }
    return result;
}

ImpValue ImpInterpreter::visit(UnaryExp* e) {
    ImpValue operand = e->operand->accept(this);
    ImpValue result;
    
    switch (e->op) {
        case NOT_OP:
            if (operand.type != TBOOL) {
                cout << "Error: operador 'not' requiere operando booleano" << endl;
                exit(1);
            }
            result = ImpValue(!operand.bool_value);
            result.type = TBOOL;
            break;
        case NEG_OP:
            if (operand.type != TINT) {
                cout << "Error: operador '-' requiere operando entero" << endl;
                exit(1);
            }
            result = ImpValue(-operand.int_value);
            break;
        default:
            cout << "Operador unario no implementado" << endl;
            exit(1);
    }
    return result;
}

ImpValue ImpInterpreter::visit(ArrayAccessExp* e) {
    ImpValue arr = env.lookup(e->arrayName);
    if (arr.type != TARRAY) {
        cout << "Error: " << e->arrayName << " no es un array" << endl;
        exit(1);
    }
    ImpValue idx = e->index->accept(this);
    if (idx.type != TINT) {
        cout << "Error: índice de array no es entero" << endl;
        exit(1);
    }
    int i = idx.int_value;
    
    // Para tipos definidos por usuario (que típicamente empiezan en 1)
    // ajustar el índice si es necesario
    if (i >= 1 && i <= (int)arr.array_value.size()) {
        i = i - 1; // Convertir de 1-based a 0-based
    }
    
    if (i < 0 || i >= (int)arr.array_value.size()) {
        cout << "Error: índice fuera de rango" << endl;
        exit(1);
    }
    return arr.array_value[i];
}

ImpValue ImpInterpreter::visit(PointerDerefExp* e) {
    ImpValue ptr = env.lookup(e->pointerName);
    if (ptr.type != TPOINTER || !ptr.ptr_value) {
        cout << "Error: puntero nulo o no es puntero" << endl;
        exit(1);
    }
    return *(ptr.ptr_value);
}

ImpValue ImpInterpreter::visit(AddressOfExp* e) {
    ImpValue var = env.lookup(e->variableName);
    return ImpValue(std::make_shared<ImpValue>(var));
}

ImpValue ImpInterpreter::visit(FunctionCallExp* e) {
    
    // Buscar la función en la tabla de funciones
    auto it = functions.find(e->funcName);
    if (it == functions.end()) {
        cout << "Error: función no definida: " << e->funcName << endl;
            exit(1);
        }
        
    FunctionDef* func = it->second;
    
    // Verificar número de argumentos
    if (e->args.size() != func->params.size()) {
        cout << "Error: número incorrecto de argumentos para función " << e->funcName 
             << ". Esperados: " << func->params.size() << ", proporcionados: " << e->args.size() << endl;
            exit(1);
        }
        
    // Crear un nuevo entorno para la función
    env.add_level();
    
    // Declarar automáticamente la variable de retorno (nombre de la función)
    ImpValue returnVar;
    if (func->returnType == "Integer") {
        returnVar = ImpValue(0);
    } else if (func->returnType == "Boolean") {
        returnVar = ImpValue(false);
    } else {
        returnVar = ImpValue(0); // Por defecto
    }
    env.add_var(func->name, returnVar);
    
    // Evaluar argumentos y asignarlos a parámetros
    for (size_t i = 0; i < e->args.size(); ++i) {
        ImpValue argValue = e->args[i]->accept(this);
        env.add_var(func->params[i], argValue);
    }
    
    // Ejecutar el cuerpo de la función
    func->body->accept(this);
    
    // Obtener el valor de retorno (en Pascal, el nombre de la función contiene el valor de retorno)
    ImpValue returnValue;
    if (env.check(func->name)) {
        returnValue = env.lookup(func->name);
    } else {
        cout << "Error: función " << func->name << " no retornó un valor" << endl;
    exit(1);
    }
    
    // Restaurar el entorno anterior
    env.remove_level();
    
    return returnValue;
}

// Sentencias
void ImpInterpreter::visit(AssignStatement* s) {
    ImpValue v = s->rhs->accept(this);
    
    // Evaluar el lado izquierdo para obtener la referencia
    if (IdentifierExp* id = dynamic_cast<IdentifierExp*>(s->lhs)) {
        // Asignación simple a variable
        if (!env.check(id->name)) {
            cout << "Error: variable no declarada: " << id->name << endl;
            exit(1);
        }
        ImpValue target = env.lookup(id->name);
        if (target.type == TARRAY && v.type == TARRAY) {
            target.array_value = v.array_value;
        } else if (target.type == TPOINTER && v.type == TPOINTER) {
            target.ptr_value = v.ptr_value;
        } else {
            target = v;
        }
        env.update(id->name, target);
    }
    else if (ArrayAccessExp* arr = dynamic_cast<ArrayAccessExp*>(s->lhs)) {
        // Asignación a elemento de array
        ImpValue array = env.lookup(arr->arrayName);
        if (array.type != TARRAY) {
            cout << "Error: " << arr->arrayName << " no es un array" << endl;
            exit(1);
        }
        ImpValue idx = arr->index->accept(this);
        if (idx.type != TINT) {
            cout << "Error: índice de array no es entero" << endl;
            exit(1);
        }
        int i = idx.int_value;
        
        // Para tipos definidos por usuario (que típicamente empiezan en 1)
        // ajustar el índice si es necesario
        if (i >= 1 && i <= (int)array.array_value.size()) {
            i = i - 1; // Convertir de 1-based a 0-based
        }
        
        if (i < 0 || i >= (int)array.array_value.size()) {
            cout << "Error: índice fuera de rango" << endl;
            exit(1);
        }
        array.array_value[i] = v;
        env.update(arr->arrayName, array);
    }
    else if (PointerDerefExp* ptr = dynamic_cast<PointerDerefExp*>(s->lhs)) {
        // Asignación a puntero desreferenciado
        ImpValue pointer = env.lookup(ptr->pointerName);
        if (pointer.type != TPOINTER || !pointer.ptr_value) {
            cout << "Error: puntero nulo o no es puntero" << endl;
            exit(1);
        }
        *(pointer.ptr_value) = v;
    }
    else {
        cout << "Error: tipo de asignación no soportado" << endl;
        exit(1);
    }
}

void ImpInterpreter::visit(PrintStatement* s) {
    for (size_t i = 0; i < s->exprs.size(); ++i) {
        ImpValue v = s->exprs[i]->accept(this);
        if (v.type == TINT) cout << v.int_value;
        else if (v.type == TBOOL) cout << (v.bool_value ? "true" : "false");
        else cout << "?";
        if (i + 1 < s->exprs.size()) cout << " ";
    }
    cout << endl;
}

void ImpInterpreter::visit(IfStatement* s) {
    ImpValue cond = s->condition->accept(this);
    if (cond.type != TBOOL) {
        cout << "Error: condición de if no es booleana" << endl;
        exit(1);
    }
    if (cond.bool_value) s->then->accept(this);
    else if (s->els) s->els->accept(this);
}

void ImpInterpreter::visit(WhileStatement* s) {
    ImpValue cond = s->condition->accept(this);
    if (cond.type != TBOOL) {
        cout << "Error: condición de while no es booleana" << endl;
        exit(1);
    }
    while (cond.bool_value) {
        s->b->accept(this);
        cond = s->condition->accept(this);
    }
}

void ImpInterpreter::visit(ForStatement* s) {
    ImpValue start = s->start->accept(this);
    ImpValue end = s->end->accept(this);
    if (start.type != TINT || end.type != TINT) {
        cout << "Error: límites de for no son enteros" << endl;
        exit(1);
    }
    for (int i = start.int_value; i <= end.int_value; ++i) {
        env.update(s->var, ImpValue(i));
        s->body->accept(this);
    }
}

void ImpInterpreter::visit(BreakStatement* s) {
    // En una implementación completa, esto debería salir del bucle más interno
    // Por ahora, simplemente imprimimos un mensaje
    // Nota: Para una implementación completa, necesitarías manejar el control de flujo
    // para salir del bucle más interno
}

void ImpInterpreter::visit(VarDec* s) {
    for (const string& var : s->vars) {
        ImpValue v;
        
        // Resolver el tipo (puede ser un tipo definido por el usuario)
        string actualType = s->type;
        
        // Por ahora, manejar tipos básicos y arrays
        if (actualType == "Integer") {
            v = ImpValue(0);
        } else if (actualType == "Boolean") {
            v = ImpValue(false);
        } else if (actualType.find("array") != std::string::npos) {
            // Inicialización simple para arrays unidimensionales
            // Ejemplo: array[1..10] of Integer
            size_t pos1 = actualType.find('[');
            size_t pos2 = actualType.find("..", pos1);
            size_t pos3 = actualType.find(']', pos2);
            int from = 1, to = 10;
            if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos) {
                from = std::stoi(actualType.substr(pos1+1, pos2-pos1-1));
                to = std::stoi(actualType.substr(pos2+2, pos3-pos2-2));
            }
            int size = to - from + 1;
            v.type = TARRAY;
            v.array_value = std::vector<ImpValue>(size, ImpValue(0));
        } else if (!actualType.empty() && actualType[0] == '^') {
            v.type = TPOINTER;
            v.ptr_value = nullptr;
        } else {
            // Tipo definido por el usuario - por ahora, asumir que es un array
            // En una implementación completa, deberías tener una tabla de tipos
            v.type = TARRAY;
            v.array_value = std::vector<ImpValue>(4, ImpValue(0)); // Tamaño por defecto
        }
        env.add_var(var, v);
    }
}

void ImpInterpreter::visit(VarDecList* s) {
    for (VarDec* vardec : s->vardecs) {
        vardec->accept(this);
    }
}

void ImpInterpreter::visit(StatementList* s) {
    for (Stm* stm : s->stms) {
        stm->accept(this);
    }
}

void ImpInterpreter::visit(Body* s) {
    s->vardecs->accept(this);
    s->slist->accept(this);
}

void ImpInterpreter::visit(Program* s) {
    // Procesar las funciones primero
    if (s->functions) {
        s->functions->accept(this);
    }
    // Luego procesar el cuerpo principal
    s->body->accept(this);
}

void ImpInterpreter::visit(ExitStatement* s) {
    // En una implementación completa, esto debería salir de la función/procedimiento
    // Por ahora, simplemente imprimimos un mensaje
    // Nota: Para una implementación completa, necesitarías manejar el control de flujo
    // para salir de la función/procedimiento
}

void ImpInterpreter::visit(NewStatement* s) {
    if (!env.check(s->pointerName)) {
        cout << "Error: puntero no declarado: " << s->pointerName << endl;
        exit(1);
    }
    ImpValue pointer = env.lookup(s->pointerName);
    if (pointer.type != TPOINTER) {
        cout << "Error: " << s->pointerName << " no es un puntero" << endl;
        exit(1);
    }
    // Asignar memoria dinámicamente
    pointer.ptr_value = std::make_shared<ImpValue>(0);
    env.update(s->pointerName, pointer);
}

void ImpInterpreter::visit(DisposeStatement* s) {
    if (!env.check(s->pointerName)) {
        cout << "Error: puntero no declarado: " << s->pointerName << endl;
        exit(1);
    }
    ImpValue pointer = env.lookup(s->pointerName);
    if (pointer.type != TPOINTER) {
        cout << "Error: " << s->pointerName << " no es un puntero" << endl;
        exit(1);
    }
    // Liberar memoria (en una implementación real, esto llamaría a free)
    pointer.ptr_value = nullptr;
    env.update(s->pointerName, pointer);
}

void ImpInterpreter::visit(FunctionDef* func) {
    // Guardar la función en la tabla de funciones para poder llamarla después
    functions[func->name] = func;
}

void ImpInterpreter::visit(FunctionList* funcs) {
    for (auto func : funcs->functions) {
        func->accept(this);
    }
}



