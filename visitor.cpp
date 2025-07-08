#include <iostream>
#include "exp.h"
#include "visitor.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
// Aquí solo deben estar los métodos visit, NO los métodos accept

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(BoolExp* exp) {
    if(exp->value) cout << "true";
    else cout << "false";
    return 0;
}

int PrintVisitor::visit(IdentifierExp* exp) {
    cout << exp->name;
    return 0;
}

int PrintVisitor::visit(UnaryExp* exp) {
    if (exp->op == NOT_OP) {
        cout << "not ";
    } else if (exp->op == NEG_OP) {
        cout << "-";
    }
    exp->operand->accept(this);
    return 0;
}

// Nuevos métodos para arrays y punteros
int PrintVisitor::visit(ArrayAccessExp* exp) {
    cout << exp->arrayName << "[";
    exp->index->accept(this);
    cout << "]";
    return 0;
}

int PrintVisitor::visit(PointerDerefExp* exp) {
    cout << exp->pointerName << "^";
    return 0;
}

int PrintVisitor::visit(AddressOfExp* exp) {
    cout << "@" << exp->variableName;
    return 0;
}

// fin


void PrintVisitor::visit(AssignStatement* stm) {
    stm->lhs->accept(this);
    cout << " = ";
    stm->rhs->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStatement* stm) {
    cout << "writeln(";
    for (size_t i = 0; i < stm->exprs.size(); ++i) {
        stm->exprs[i]->accept(this);
        if (i + 1 < stm->exprs.size()) cout << ", ";
    }
    cout << ");";
}

void PrintVisitor::visit(IfStatement* stm) {
    cout << "if ";
    stm->condition->accept(this);
    cout << " then" << endl;
    stm->then->accept(this);
    if(stm->els){
        cout << "else" << endl;
        stm->els->accept(this);
    }
    
}

void PrintVisitor::imprimir(Program* program){
    program->body->accept(this);
};

void PrintVisitor::visit(WhileStatement* stm){
    cout << "while ";
    stm->condition->accept(this);
    cout << " do" << endl;
    stm->b->accept(this);
}

// Nuevo método para bucles for
void PrintVisitor::visit(ForStatement* stm){
    cout << "for " << stm->var << " := ";
    stm->start->accept(this);
    cout << " to ";
    stm->end->accept(this);
    cout << " do" << endl;
    stm->body->accept(this);
    
}

// Nuevo método para break
void PrintVisitor::visit(BreakStatement* stm){
    cout << "break;";
}

void PrintVisitor::visit(VarDec* stm){
    cout << "var ";
    cout << stm->type;
    cout << " ";
    for(auto i: stm->vars){
        cout << i;
        if(i != stm->vars.back()) cout << ", ";
    }
    cout << ";";
}

void PrintVisitor::visit(VarDecList* stm){
    for(auto i: stm->vardecs){
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(StatementList* stm){
    for(auto i: stm->stms){
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(Body* stm){
    stm->vardecs->accept(this);
    cout << endl;
    stm->slist->accept(this);
}

int PrintVisitor::visit(FunctionCallExp* exp) {
    cout << exp->funcName << "(";
    for (size_t i = 0; i < exp->args.size(); ++i) {
        exp->args[i]->accept(this);
        if (i + 1 < exp->args.size()) cout << ", ";
    }
    cout << ")";
    return 0;
}

// Nuevo método para exit (Pascal)
void PrintVisitor::visit(ExitStatement* stm){
    cout << "exit;";
}

// Nuevo método para new (Pascal)
void PrintVisitor::visit(NewStatement* stm){
    cout << "new(" << stm->pointerName << ");";
}

// Nuevo método para dispose (Pascal)
void PrintVisitor::visit(DisposeStatement* stm){
    cout << "dispose(" << stm->pointerName << ");";
}

void PrintVisitor::visit(FunctionDef* func){
    cout << "function " << func->name << "(";
    for (size_t i = 0; i < func->params.size(); ++i) {
        cout << func->params[i];
        if (i + 1 < func->params.size()) cout << ", ";
    }
    cout << "): " << func->returnType << ";" << endl;
    func->body->accept(this);
    cout << "end;";
}

void PrintVisitor::visit(FunctionList* funcs){
    for (auto func : funcs->functions) {
        func->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(Program* program){
    if (program->functions) {
        program->functions->accept(this);
    }
    program->body->accept(this);
}

