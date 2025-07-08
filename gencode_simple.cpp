#include "gencode.hh"

std::string current_function_name;

void GenCodeVisitor::generar(Program* program) {
    program->accept(this);
}

void GenCodeVisitor::visit(Program* p) {
    env.clear();
    etiquetas = 0;
    current_offset = 0;

    cout << ".data" << endl;
    cout << "print_fmt: .string \"%ld\\n\"" << endl;
    cout << ".text" << endl;
    
    // Generar funciones primero
    if (p->functions) {
        p->functions->accept(this);
    }
    
    // Luego generar main
    cout << endl;
    cout << "# Función main" << endl;
    cout << ".globl main" << endl;
    cout << "main:" << endl;
    cout << "  pushq %rbp" << endl;
    cout << "  movq %rsp, %rbp" << endl;
    
    env.add_level();
    long old_offset = current_offset;
    
    // Procesar declaraciones de variables
    if (p->body->vardecs) {
        p->body->vardecs->accept(this);
    }
    
    // Reservar espacio en la pila
    long locals_size = -current_offset;
    if (locals_size > 0) {
        cout << "  subq $" << locals_size << ", %rsp" << endl;
    } else {
        cout << "  subq $0, %rsp" << endl;
    }
    
    // Procesar sentencias
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
    
void GenCodeVisitor::visit(Body* b) {
    env.add_level();
    long old_offset = current_offset;
    
    // Procesar declaraciones de variables
    if (b->vardecs) {
        b->vardecs->accept(this);
    }
    
    // Reservar espacio en la pila
    long locals_size = -current_offset;
    if (locals_size > 0) {
        cout << "  subq $" << locals_size << ", %rsp" << endl;
    }

    // Procesar sentencias
    if (b->slist) {
        b->slist->accept(this);
    }
    
    current_offset = old_offset;
    env.remove_level();
}

void GenCodeVisitor::visit(VarDecList* decs) {
    for (auto it : decs->vardecs) {
        it->accept(this);
    }
}

void GenCodeVisitor::visit(VarDec* vd) {
    for (const auto& var : vd->vars) {
        if (vd->type.find("array") != std::string::npos) {
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
            current_offset -= size * 8; // Reservar espacio para todo el array
            stack_offsets[var] = current_offset;
        } else {
            current_offset -= 8;
            stack_offsets[var] = current_offset;
        }
        env.add_var(var, ImpValue(0));
    }
}

void GenCodeVisitor::visit(StatementList* s) {
    for (auto it : s->stms) {
        it->accept(this);
    }
}

void GenCodeVisitor::visit(AssignStatement* s) {
    s->rhs->accept(this); 
    
    if (IdentifierExp* id = dynamic_cast<IdentifierExp*>(s->lhs)) {
        // Si es asignación al nombre de la función (valor de retorno)
        if (id->name == current_function_name) {
            // No generar movq, el valor ya está en %rax
            env.update(id->name, ImpValue(0));
        } else if (stack_offsets.find(id->name) != stack_offsets.end()) {
            // Asignación a variable local
            cout << "  movq %rax, " << stack_offsets[id->name] << "(%rbp)" << endl;
            env.update(id->name, ImpValue(0));
        } else {
            cerr << "Error: variable " << id->name << " no encontrada en stack_offsets" << endl;
            exit(1);
        }
    }
    else if (PointerDerefExp* ptr = dynamic_cast<PointerDerefExp*>(s->lhs)) {
        // Generar código para asignación a través de puntero
        cout << "  pushq %rax" << endl;  // Guardar el valor
        cout << "  movq " << stack_offsets[ptr->pointerName] << "(%rbp), %rax" << endl;
        cout << "  popq %rcx" << endl;  // Recuperar el valor
        cout << "  movq %rcx, (%rax)" << endl;  // Asignar el valor
    }
    else if (ArrayAccessExp* arr = dynamic_cast<ArrayAccessExp*>(s->lhs)) {
        // Guardar el valor a asignar
        cout << "  pushq %rax" << endl;
        // Evaluar el índice
        arr->index->accept(this);
        cout << "  pushq %rax" << endl;
        // Obtener la dirección base del array
        cout << "  leaq " << stack_offsets[arr->arrayName] << "(%rbp), %rax" << endl;
        cout << "  popq %rcx" << endl;  // Recuperar el índice
        // Ajustar el índice para base 1
        cout << "  subq $1, %rcx" << endl;
        cout << "  imulq $8, %rcx" << endl;
        cout << "  addq %rcx, %rax" << endl;  // Sumar el offset
        cout << "  popq %rcx" << endl;  // Recuperar el valor
        cout << "  movq %rcx, (%rax)" << endl;  // Asignar el valor
    }
    else {
        cerr << "Error: tipo de asignación no soportado en generación de código" << endl;
        exit(1);
    }
}

void GenCodeVisitor::visit(PrintStatement* s) {
    for (size_t i = 0; i < s->exprs.size(); ++i) {
        s->exprs[i]->accept(this);
        cout << "  movq %rax, %rsi" << endl;
        cout << "  leaq print_fmt(%rip), %rdi" << endl;
        cout << "  movl $0, %eax" << endl;
        cout << "  call printf@PLT" << endl;
    }
}

void GenCodeVisitor::visit(IfStatement* s) {
    int lbl = etiquetas++;
    s->condition->accept(this);
    cout << "  cmpq $0, %rax" << endl;
    cout << "  je else_" << lbl << endl;
    s->then->accept(this);
    cout << "  jmp .end_" << current_function_name << endl;
    cout << "else_" << lbl << ":" << endl;
    if (s->els) s->els->accept(this); 
    cout << "endif_" << lbl << ":" << endl;
}

void GenCodeVisitor::visit(WhileStatement* s) {
    int lbl_cond = etiquetas++;
    int lbl_end  = etiquetas++;
    cout << "while_" << lbl_cond << ":" << endl;
    s->condition->accept(this);
    cout << "  cmpq $0, %rax" << endl;
    cout << "  je endwhile_" << lbl_end << endl;
    s->b->accept(this);
    cout << "  jmp while_" << lbl_cond << endl;
    cout << "endwhile_" << lbl_end << ":" << endl;
}

void GenCodeVisitor::visit(ForStatement* s) {
    int lbl_cond = etiquetas++;
    int lbl_end  = etiquetas++;
    
    // Inicializar la variable del bucle
    s->start->accept(this);
    cout << "  movq %rax, " << stack_offsets[s->var] << "(%rbp)" << endl;
    
    cout << "for_" << lbl_cond << ":" << endl;
    
    // Comparar con el valor final
    cout << "  movq " << stack_offsets[s->var] << "(%rbp), %rax" << endl;
    s->end->accept(this);
    cout << "  cmpq %rax, " << stack_offsets[s->var] << "(%rbp)" << endl;
    cout << "  jg endfor_" << lbl_end << endl;
    
    // Ejecutar el cuerpo del bucle
    s->body->accept(this);
    
    // Incrementar la variable
    cout << "  movq " << stack_offsets[s->var] << "(%rbp), %rax" << endl;
    cout << "  addq $1, %rax" << endl;
    cout << "  movq %rax, " << stack_offsets[s->var] << "(%rbp)" << endl;
    cout << "  jmp for_" << lbl_cond << endl;
    
    cout << "endfor_" << lbl_end << ":" << endl;
}

int GenCodeVisitor::visit(BinaryExp* e) {
    e->left->accept(this);        
    cout << "  pushq %rax" << endl;
    e->right->accept(this);       
    cout << "  movq %rax, %rcx" << endl; 
    cout << "  popq %rax" << endl;    

    switch (e->op) {
        case PLUS_OP:
            cout << "  addq %rcx, %rax" << endl;
            break;

        case MINUS_OP:
            cout << "  subq %rcx, %rax" << endl;
            break;

        case MUL_OP:
            cout << "  imulq %rcx, %rax" << endl;
            break;

        case DIV_OP:
            cout << "  cqto" << endl;
            cout << "  idivq %rcx" << endl;
            break;

        case MOD_OP:
            cout << "  cqto" << endl;
            cout << "  idivq %rcx" << endl;
            cout << "  movq %rdx, %rax" << endl;
            break;

        case LT_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setl %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            break;

        case LE_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setle %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            break;

        case EQ_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  sete %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            break;

        case GT_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setg %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            break;

        case GE_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setge %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            break;

        case NE_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setne %al" << endl;
            cout << "  movzbq %al, %rax" << endl;
            break;

        case AND_OP:
            cout << "  andq %rcx, %rax" << endl;
            break;

        case OR_OP:
            cout << "  orq %rcx, %rax" << endl;
            break;

        default:
            cerr << "Operador binario no soportado." << endl;
            exit(1);
    }

    return 0;
}

int GenCodeVisitor::visit(UnaryExp* e) {
    e->operand->accept(this);
    
    switch (e->op) {
        case NOT_OP:
            cout << "  cmpq $0, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  sete %al" << endl;
            break;
        case NEG_OP:
            cout << "  negq %rax" << endl;
            break;
        default:
            cerr << "Operador unario no soportado." << endl;
            exit(1);
    }
    
    return 0;
}

int GenCodeVisitor::visit(NumberExp* e) {
    cout << "  movq $" << e->value << ", %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(BoolExp* e) {
    cout << "  movq $" << (e->value ? 1 : 0) << ", %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(IdentifierExp* e) {
    if (env.check(e->name)) {
        if (stack_offsets.find(e->name) != stack_offsets.end()) {
            cout << "  movq " << stack_offsets[e->name] << "(%rbp), %rax" << endl;
            return 0;
        } else {
            cerr << "Error: variable " << e->name << " no encontrada en stack_offsets" << endl;
            exit(1);
        }
    } else {
        cerr << "Error: variable " << e->name << " no declarada" << endl;
        exit(1);
    }
}

// Corrige el acceso a arrays para base 1
enum { ARRAY_BASE = 1 };

int GenCodeVisitor::visit(ArrayAccessExp* e) {
    e->index->accept(this);
    cout << "  pushq %rax" << endl;
    cout << "  leaq " << stack_offsets[e->arrayName] << "(%rbp), %rax" << endl;
    cout << "  popq %rcx" << endl;
    // Ajustar el índice para base 1
    cout << "  subq $" << ARRAY_BASE << ", %rcx" << endl;
    cout << "  imulq $8, %rcx" << endl;
    cout << "  addq %rcx, %rax" << endl;
    cout << "  movq (%rax), %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(PointerDerefExp* e) {
    if (env.check(e->pointerName)) {
        cout << "  movq " << stack_offsets[e->pointerName] << "(%rbp), %rax" << endl;
        cout << "  movq (%rax), %rax" << endl;
        return 0;
    }
    
    cerr << "Error: puntero " << e->pointerName << " no declarado" << endl;
    exit(1);
}

int GenCodeVisitor::visit(AddressOfExp* e) {
    if (env.check(e->variableName)) {
        cout << "  leaq " << stack_offsets[e->variableName] << "(%rbp), %rax" << endl;
        return 0;
    }
    
    cerr << "Error: variable " << e->variableName << " no declarada" << endl;
    exit(1);
}

void GenCodeVisitor::visit(BreakStatement* s) {
    cout << "  # break statement" << endl;
}

int GenCodeVisitor::visit(FunctionCallExp* e) {
    const char* arg_regs[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int n = e->args.size();
    if (n > 6) {
        cerr << "Error: solo se soportan hasta 6 argumentos en llamadas a función" << endl;
        exit(1);
    }
    
    for (int i = n - 1; i >= 0; --i) {
        e->args[i]->accept(this);
        cout << "  movq %rax, " << arg_regs[i] << endl;
    }
    
    cout << "  call " << e->funcName << endl;
    return 0;
}

void GenCodeVisitor::visit(ExitStatement* s) {
    cout << "  # exit statement" << endl;
}

void GenCodeVisitor::visit(NewStatement* s) {
    cout << "  # new statement - asignando memoria para puntero: " << s->pointerName << endl;
    cout << "  movq $8, %rdi" << endl;
    cout << "  call malloc@PLT" << endl;
    cout << "  movq %rax, " << stack_offsets[s->pointerName] << "(%rbp)" << endl;
}

void GenCodeVisitor::visit(DisposeStatement* s) {
    cout << "  # dispose statement - liberando memoria para puntero: " << s->pointerName << endl;
    cout << "  movq " << stack_offsets[s->pointerName] << "(%rbp), %rdi" << endl;
    cout << "  call free@PLT" << endl;
}

void GenCodeVisitor::visit(FunctionDef* func) {
    current_function_name = func->name;
    cout << endl;
    cout << "# Función: " << func->name << endl;
    cout << ".globl " << func->name << endl;
    cout << func->name << ":" << endl;
    
    cout << "  # Prologue" << endl;
    cout << "  pushq %rbp" << endl;
    cout << "  movq %rsp, %rbp" << endl;
    
    int n = func->params.size();
    long old_offset = current_offset;
    env.add_level();
    
    // Guardar argumentos en el stack
    const char* arg_regs[6] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    for (int i = 0; i < n; ++i) {
        int offset = -8 * (i + 1);
        stack_offsets[func->params[i]] = offset;
        cout << "  movq " << arg_regs[i] << ", " << offset << "(%rbp)" << endl;
        env.add_var(func->params[i], ImpValue(0));
    }
    
    // Reservar espacio para parámetros
    int total_arg_space = n * 8;
    cout << "  subq $" << (total_arg_space > 0 ? total_arg_space : 0) << ", %rsp" << endl;
    
    // Procesar variables locales
    if (func->body->vardecs) {
        func->body->vardecs->accept(this);
    }
    
    // Procesar el cuerpo de la función
    if (func->body->slist) {
        func->body->slist->accept(this);
    }
    
    // Epilogue de función
    cout << ".end_" << func->name << ":" << endl;
    cout << "  leave" << endl;
    cout << "  ret" << endl;
    
    current_offset = old_offset;
    env.remove_level();
    current_function_name = "";
}

void GenCodeVisitor::visit(FunctionList* funcs) {
    cout << "  # procesando " << funcs->functions.size() << " funciones" << endl;
    for (auto func : funcs->functions) {
        func->accept(this);
    }
} 