#include "gencode.hh"

// Archivo de generación de código. Aquí NO deben estar los métodos accept del AST.

std::string current_function_name;

// Nuevos métodos para verificación de límites
void ImpCODE::generate_bounds_check(const string& arrayName, int label) {
    // Verificar límite inferior
    // Obtener los límites del array
    if (array_bounds.find(arrayName) == array_bounds.end()) {
        cerr << "Error: límites no encontrados para array " << arrayName << endl;
        exit(1);
    }
    
    ArrayBounds bounds = array_bounds[arrayName];
    
}



void ImpCODE::interpret(Program* p) {
    env.clear();
    etiquetas = 0;
    current_offset = 0;
    array_bounds.clear(); // Limpiar límites de arrays

    cout << ".data" << endl;
    cout << "print_fmt: .string \"%ld\\n\"" << endl;
    cout << ".text" << endl;
    
    // Generar funciones primero
    if (p->functions) {
        p->functions->accept(this);
    }
    
    // Luego generar main
    cout << ".globl main" << endl;
    cout << "main:" << endl;
    cout << "  pushq %rbp" << endl;  // Guardar el puntero base
    cout << "  movq %rsp, %rbp" << endl; // Establecer el nuevo puntero base
    
    // Generar el cuerpo del programa principal usando la misma lógica que las funciones
    env.add_level();
    long old_offset = current_offset;
    
    // Procesar declaraciones de variables
    if (p->body->vardecs) {
        p->body->vardecs->accept(this);
    }
    
    // Reservar espacio en la pila para todas las variables locales
    long locals_size = -current_offset;
    if (locals_size > 0) {
        cout << "  subq $" << locals_size << ", %rsp" << endl;
    } else {
        cout << "  subq $0, %rsp" << endl;  // Sin variables locales
    }
    
    // Procesar sentenciasReservado
    if (p->body->slist) {
        p->body->slist->accept(this);
    }
    

    
    current_offset = old_offset;
    env.remove_level();
    
    cout << "  movl $0, %eax" << endl;
    cout << "  jmp .end_main" << endl;
    cout << ".end_main:" << endl;
    cout << "  leave" << endl;
    cout << "  ret" << endl;
    cout << ".section .note.GNU-stack,\"\",@progbits" << endl;
}
    
void ImpCODE::visit(Program* p) {
    // Las funciones se procesan en interpret(), no aquí
    // Solo procesar el cuerpo principal
    p->body->accept(this);
}

void ImpCODE::visit(Body* b) {
    env.add_level();
    long old_offset = current_offset;
    
    // Procesar declaraciones de variables
    if (b->vardecs) {
        b->vardecs->accept(this);
    }
    
    // Solo reservar espacio si realmente hay variables locales nuevas
    long locals_size = -current_offset;
    if (locals_size > 0 && b->vardecs && !b->vardecs->vardecs.empty()) {
        cout << "  subq $" << locals_size << ", %rsp" << endl;
    }

    // Procesar sentencias
    if (b->slist) {
        b->slist->accept(this);
    }
    
    
    current_offset = old_offset;
    env.remove_level();
}

void ImpCODE::visit(VarDecList* decs) {
    for (auto it : decs->vardecs) {
        it->accept(this);
    }
}

void ImpCODE::visit(VarDec* vd) {
    ImpVType tt = ImpValue::get_basic_type(vd->type);
    for (const auto& var : vd->vars) {
        ImpValue v;
        if (vd->type.find("array") != std::string::npos) {
            // Es un array
            v.type = TARRAY;
            // Extraer el rango del array, por ejemplo: array[1..5] of Integer
            size_t pos1 = vd->type.find('[');
            size_t pos2 = vd->type.find("..", pos1);
            size_t pos3 = vd->type.find(']', pos2);
            int from = 1, to = 1;
            if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos) {
                from = std::stoi(vd->type.substr(pos1+1, pos2-pos1-1));
                to = std::stoi(vd->type.substr(pos2+2, pos3-pos2-2));
            }
            int size = to - from + 1;
            v.array_value = std::vector<ImpValue>(size, ImpValue(0));
            current_offset -= size * 8; // Reservar espacio para todo el array
            stack_offsets[var] = current_offset + (size - 1) * 8; // Offset del primer elemento
            
            // Almacenar límites del array para verificación
            array_bounds[var] = ArrayBounds(from, to);
        } else if (!vd->type.empty() && vd->type[0] == '^') {
            // Es un puntero
            v.type = TPOINTER;
            v.ptr_value = nullptr;
            current_offset -= 8; // <-- Mueve esto ANTES de asignar el offset
            stack_offsets[var] = current_offset;
        } else {
            // Es un tipo básico
            v.set_default_value(tt);
            current_offset -= 8; // <-- Mueve esto ANTES de asignar el offset
            stack_offsets[var] = current_offset;
        }
        env.add_var(var, v);
    }
}

void ImpCODE::visit(StatementList* s) {
    for (auto it : s->stms) {
        it->accept(this);
    }
}
void ImpCODE::visit(AssignStatement* s) {
    ImpValue val = s->rhs->accept(this); 
    
    // Evaluar el lado izquierdo para obtener la referencia
    if (IdentifierExp* id = dynamic_cast<IdentifierExp*>(s->lhs)) {
        // Si es asignación al nombre de la función (valor de retorno)
        if (id->name == current_function_name) {
            // No generar movq, el valor ya está en %rax
            env.update(id->name, val);
        } else if (stack_offsets.find(id->name) != stack_offsets.end()) {
            // Asignación a variable local
            cout << "  movq %rax, " << stack_offsets[id->name] << "(%rbp)" << endl;
            env.update(id->name, val);
        } else {
            cerr << "Error: variable " << id->name << " no encontrada en stack_offsets" << endl;
            exit(1);
        }
    }
    else if (ArrayAccessExp* arr = dynamic_cast<ArrayAccessExp*>(s->lhs)) {
        // Generar código para asignación a array
        cout << "  pushq %rax" << endl;  // Guardar el valor
        arr->index->accept(this);
        cout << "  pushq %rax" << endl;  // Guardar el índice
        
        // Verificación de límites
        int bounds_label = etiquetas++;
        cout << "  movq %rax, %rcx" << endl;  // Mover índice a %rcx para verificación
        generate_bounds_check(arr->arrayName, bounds_label);
        
        // Continuar con el acceso al array
        cout << "  leaq " << stack_offsets[arr->arrayName] << "(%rbp), %rax" << endl;
        cout << "  popq %rcx" << endl;  // Recuperar el índice
        cout << "  subq $1, %rcx" << endl;
        cout << "  imulq $8, %rcx" << endl;
        cout << "  subq %rcx, %rax" << endl;  // Restar el offset
        cout << "  popq %rcx" << endl;  // Recuperar el valor
        cout << "  movq %rcx, (%rax)" << endl;  // Asignar el valor
        
        // Etiqueta para continuar después de la verificación

        // NO agregar subq $X, %rsp aquí
    }
    else if (PointerDerefExp* ptr = dynamic_cast<PointerDerefExp*>(s->lhs)) {
        // Generar código para asignación a través de puntero
        // Evaluar el valor a asignar
        cout << "  pushq %rax" << endl;  // Guardar el valor
        
        // Obtener la dirección del puntero
        cout << "  movq " << stack_offsets[ptr->pointerName] << "(%rbp), %rax" << endl;
        cout << "  popq %rcx" << endl;  // Recuperar el valor
        cout << "  movq %rcx, (%rax)" << endl;  // Asignar el valor
        
        // Actualizar el entorno
        ImpValue pointer = env.lookup(ptr->pointerName);
        if (pointer.ptr_value) {
            *(pointer.ptr_value) = val;
        }
    }
    else {
        cerr << "Error: tipo de asignación no soportado en generación de código" << endl;
        exit(1);
    }
}

void ImpCODE::visit(PrintStatement* s) {
    for (size_t i = 0; i < s->exprs.size(); ++i) {
        s->exprs[i]->accept(this);
        cout << "  movq %rax, %rsi" << endl;
        cout << "  leaq print_fmt(%rip), %rdi" << endl;
        cout << "  movl $0, %eax" << endl;
        cout << "  call printf@PLT" << endl;
    }
}

void ImpCODE::visit(IfStatement* s) {
    int lbl = etiquetas++;
    s->condition->accept(this);
    cout << "  cmpq $0, %rax" << endl;
    cout << "  je else_" << lbl << endl;
    s->then->accept(this);
    // Si la rama then termina la función, saltar a la etiqueta de finalización
    cout << "  jmp .end_" << current_function_name << endl;
    cout << "  jmp endif_" << lbl << endl;
    cout << "else_" << lbl << ":" << endl;
    if (s->els) s->els->accept(this); 
    cout << "  jmp .end_" << current_function_name << endl;
    cout << "endif_" << lbl << ":" << endl;
}

void ImpCODE::visit(WhileStatement* s) {
    int lbl_cond = etiquetas++;
    int lbl_end  = etiquetas++;
    cout << "while_" << lbl_cond << ":" << endl;
    s->condition->accept(this);
    cout << "  cmpq $0, %rax" << endl;
    cout << "  je endwhile_" << lbl_end - 1<< endl;
    s->b->accept(this);
    cout << "  jmp while_" << lbl_cond << endl;
    cout << "endwhile_" << lbl_end - 1 << ":" << endl;
}

void ImpCODE::visit(ForStatement* s) {
    int lbl_cond = etiquetas++;
    int lbl_end  = etiquetas++;
    
    // Inicializar la variable del bucle
    s->start->accept(this);
    cout << "  movq %rax, " << stack_offsets[s->var] << "(%rbp)" << endl;
    
    cout << "for_" << lbl_cond << ":" << endl;
    
    // Evaluar condición: i <= end
    cout << "  movq " << stack_offsets[s->var] << "(%rbp), %rax" << endl;  // %rax ← i
    cout << "  pushq %rax" << endl;  // guardar i
    s->end->accept(this);  // cargar valor final
    cout << "  movq %rax, %rcx" << endl;  // %rcx ← valor final
    cout << "  popq %rax" << endl;  // recuperar i en %rax
    cout << "  cmpq %rcx, %rax" << endl;  // compara i con valor final → i - valor_final
    cout << "  movl $0, %eax" << endl;  // limpiar %eax (parte baja de %rax)
    cout << "  setle %al" << endl;  // %al = 1 si i <= valor_final, si no 0
    cout << "  movzbq %al, %rax" << endl;  // extiende %al a 64 bits → %rax = 0 o 1
    cout << "  cmpq $0, %rax" << endl;  // ¿la condición es falsa?
    cout << "  je endfor_" << lbl_end - 1 << endl;  // si i > valor_final (falsa), salta al final del for
    
    // Ejecutar el cuerpo del bucle
    s->body->accept(this);
    
    // Incrementar la variable
    cout << "  incq " << stack_offsets[s->var] << "(%rbp)" << endl;
    cout << "  jmp for_" << lbl_cond << endl;
    
    cout << "endfor_" << lbl_end - 1 << ":" << endl;
}

ImpValue ImpCODE::visit(BinaryExp* e) {
    ImpValue result;
    e->left->accept(this);        
    cout << "  pushq %rax" << endl;
    e->right->accept(this);       
    cout << "  movq %rax, %rcx" << endl; 
    cout << "  popq %rax" << endl;    

    switch (e->op) {
        case PLUS_OP:
            cout << "  addq %rcx, %rax" << endl;
            result.type = TINT;
            break;

        case MINUS_OP:
            cout << "  subq %rcx, %rax" << endl;
            result.type = TINT;
            break;

        case MUL_OP:
            cout << "  imulq %rcx, %rax" << endl;
            result.type = TINT;
            break;

        case DIV_OP:
            cout << "  cqto" << endl;
            cout << "  idivq %rcx" << endl;
            result.type = TINT;
            break;

        case MOD_OP:
            cout << "  cqto" << endl;
            cout << "  idivq %rcx" << endl;
            cout << "  movq %rdx, %rax" << endl;  // El resto está en %rdx
            result.type = TINT;
            break;

        case LT_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setl %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            result.type = TBOOL;
            break;

        case LE_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setle %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            result.type = TBOOL;
            break;

        case EQ_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  sete %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            result.type = TBOOL;
            break;

        case GT_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setg %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            result.type = TBOOL;
            break;

        case GE_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setge %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            result.type = TBOOL;
            break;

        case NE_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setne %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            result.type = TBOOL;
            break;

        case AND_OP:
            cout << "  andq %rcx, %rax" << endl;
            result.type = TBOOL;
            break;

        case OR_OP:
            cout << "  orq %rcx, %rax" << endl;
            result.type = TBOOL;
            break;

        default:
            cerr << "Operador binario no soportado." << endl;
            exit(1);
    }

    return result;
}

ImpValue ImpCODE::visit(UnaryExp* e) {
    ImpValue result;
    e->operand->accept(this);
    
    switch (e->op) {
        case NOT_OP:
            cout << "  cmpq $0, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  sete %al" << endl;  // Set if equal (to 0)
            result.type = TBOOL;
            break;
        case NEG_OP:
            cout << "  negq %rax" << endl;
            result.type = TINT;
            break;
        default:
            cerr << "Operador unario no soportado." << endl;
            exit(1);
    }
    
    return result;
}

ImpValue ImpCODE::visit(NumberExp* e) {
    ImpValue v;
    v.set_default_value(TINT);
    v.int_value = e->value;
    cout << "  movq $" << e->value << ", %rax" << endl;
    return v;
}

ImpValue ImpCODE::visit(BoolExp* e) {
    ImpValue v;
    v.set_default_value(TBOOL);
    v.bool_value = e->value;
    cout << "  movq $" << (e->value ? 1 : 0) << ", %rax" << endl;
    return v;
}

ImpValue ImpCODE::visit(IdentifierExp* e) {
    if (env.check(e->name)) {
        if (stack_offsets.find(e->name) != stack_offsets.end()) {
            cout << "  movq " << stack_offsets[e->name] << "(%rbp), %rax" << endl;
            return env.lookup(e->name);
        } else {
            cerr << "Error: variable " << e->name << " no encontrada en stack_offsets" << endl;
            exit(1);
        }
    } else {
        cerr << "Error: variable " << e->name << " no declarada" << endl;
        exit(1);
    }
}

ImpValue ImpCODE::visit(ArrayAccessExp* e) {
    // Generar código para acceso a array unidimensional
    e->index->accept(this);
    cout << "  pushq %rax" << endl;  // Guardar el índice
    
    // Verificación de límites
    int bounds_label = etiquetas++;
    cout << "  movq %rax, %rcx" << endl;  // Mover índice a %rcx para verificación
    generate_bounds_check(e->arrayName, bounds_label);
    
    // Obtener la dirección base del array (primer elemento)
    cout << "  leaq " << stack_offsets[e->arrayName] << "(%rbp), %rax" << endl;
    cout << "  popq %rcx" << endl;  // Recuperar el índice
    // Ajustar el índice para base 1
    cout << "  subq $1, %rcx" << endl;
    cout << "  imulq $8, %rcx" << endl;
    cout << "  subq %rcx, %rax" << endl;  // Restar el offset para crecer hacia abajo
    cout << "  movq (%rax), %rax" << endl;  // Cargar el valor
    

    
    
    // NO sobrescribir %rax después de cargar el valor
    return ImpValue(0); // Retornar valor dummy, el valor real está en %rax
}

ImpValue ImpCODE::visit(PointerDerefExp* e) {
    if (env.check(e->pointerName)) {
        cout << "  movq " << stack_offsets[e->pointerName] << "(%rbp), %rax" << endl;
        cout << "  movq (%rax), %rax" << endl;  // Desreferenciar el puntero
        
        ImpValue pointer = env.lookup(e->pointerName);
        if (pointer.ptr_value) {
            return *(pointer.ptr_value);
        }
    }
    
    cerr << "Error: puntero " << e->pointerName << " no declarado" << endl;
    exit(1);
}

ImpValue ImpCODE::visit(AddressOfExp* e) {
    if (env.check(e->variableName)) {
        cout << "  leaq " << stack_offsets[e->variableName] << "(%rbp), %rax" << endl;
        
        ImpValue var = env.lookup(e->variableName);
        ImpValue result;
        result.type = TPOINTER;
        result.ptr_value = std::make_shared<ImpValue>(var);
        return result;
    }
    
    cerr << "Error: variable " << e->variableName << " no declarada" << endl;
    exit(1);
}

void ImpCODE::visit(BreakStatement* s) {
    cout << "  # break statement - salto al final del bucle más interno" << endl;

}

ImpValue ImpCODE::visit(FunctionCallExp* e) {
    // Soportar hasta 6 argumentos (rdi, rsi, rdx, rcx, r8, r9)
    const char* arg_regs[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int n = e->args.size();
    if (n > 6) {
        cerr << "Error: solo se soportan hasta 6 argumentos en llamadas a función" << endl;
        exit(1);
    }
    // Evaluar argumentos de derecha a izquierda (como en C)
    for (int i = n - 1; i >= 0; --i) {
        e->args[i]->accept(this);
        cout << "  movq %rax, " << arg_regs[i] << endl;
    }
    // Llamar a la función
    cout << "  call " << e->funcName << endl;
    // El resultado está en %rax
    return ImpValue(0);
}

void ImpCODE::visit(ExitStatement* s) {
    // En una implementación completa, esto debería generar un salto al final de la función/procedimiento
    // Por ahora, simplemente generamos un comentario
    cout << "  # exit statement - salto al final de la función/procedimiento" << endl;
    // Nota: Para una implementación completa, necesitarías manejar las etiquetas de las funciones
    // y generar el salto apropiado
}

//malloc reserva memoria en el heap
void ImpCODE::visit(NewStatement* s) {
    // Generar código para asignar memoria dinámicamente usando malloc
    cout << "  # new statement - asignando memoria para puntero: " << s->pointerName << endl;
    cout << "  movq $8, %rdi" << endl;  // Tamaño a asignar (8 bytes para un entero)
    cout << "  call malloc@PLT" << endl;  // Llamar a malloc
    cout << "  movq %rax, " << stack_offsets[s->pointerName] << "(%rbp)" << endl;  // Guardar la dirección en el puntero
    
    // Actualizar el entorno
    if (env.check(s->pointerName)) {
        ImpValue pointer = env.lookup(s->pointerName);
        pointer.ptr_value = std::make_shared<ImpValue>(0);
        env.update(s->pointerName, pointer);
    }
}

void ImpCODE::visit(DisposeStatement* s) {
    // Generar código para liberar memoria dinámicamente usando free
    cout << "  # dispose statement - liberando memoria para puntero: " << s->pointerName << endl;
    cout << "  movq " << stack_offsets[s->pointerName] << "(%rbp), %rdi" << endl;  // Cargar la dirección del puntero
    cout << "  call free@PLT" << endl;  // Llamar a free
    
    // Actualizar el entorno
    if (env.check(s->pointerName)) {
        ImpValue pointer = env.lookup(s->pointerName);
        pointer.ptr_value = nullptr;
        env.update(s->pointerName, pointer);
    }
}

void ImpCODE::visit(FunctionDef* func) {
    current_function_name = func->name;
    cout << endl;
    cout << "# Función: " << func->name << endl;
    cout << ".globl " << func->name << endl;
    cout << func->name << ":" << endl;
    // Prologue de función
    cout << "  # Prologue" << endl;
    cout << "  pushq %rbp" << endl;
    cout << "  movq %rsp, %rbp" << endl;
    int n = func->params.size();
    long old_offset = current_offset;
    env.add_level();
    
    // Guardar argumentos en el stack PRIMERO (en direcciones temporales)
    const char* arg_regs[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    for (int i = 0; i < n; ++i) {
        // Guardar en dirección temporal (antes de reservar espacio)
        cout << "  movq " << arg_regs[i] << ", " << -8 * (i + 1) << "(%rbp)" << endl;
        ImpValue param_value(0);
        env.add_var(func->params[i], param_value);
    }
    
    // LUEGO reservar espacio para parámetros
    int total_arg_space = n * 8;
    cout << "  subq $" << (total_arg_space > 0 ? total_arg_space : 0) << ", %rsp" << endl; // Solo espacio para parámetros
    
    // Ahora establecer los offsets correctos para las variables
    for (int i = 0; i < n; ++i) {
        int offset = -8 * (i + 1);
        stack_offsets[func->params[i]] = offset;
    }
    
    // El valor de retorno se mantiene en %rax, no se guarda en memoria
    ImpValue return_value(0);
    env.add_var(func->name, return_value);
    // Procesar variables locales (si las hay)
    if (func->body->vardecs) {
        func->body->vardecs->accept(this);
    }
    // Procesar el cuerpo de la función
    if (func->body->slist) {
        func->body->slist->accept(this);
    }
    // El valor de retorno ya está en %rax
    // Epilogue de función en una única etiqueta
    cout << ".end_" << func->name << ":" << endl;
    cout << "  leave" << endl;
    cout << "  ret" << endl;
    current_offset = old_offset;
    env.remove_level();
    current_function_name = "";
}

void ImpCODE::visit(FunctionList* funcs) {
    for (auto func : funcs->functions) {
        func->accept(this);
    }
}