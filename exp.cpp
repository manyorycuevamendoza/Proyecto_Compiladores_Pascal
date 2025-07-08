#include <iostream>
#include <vector>
#include "exp.h"
#include "visitor.h"
#include "imp_value_visitor.hh"
#include "type_visitor.hh"
using namespace std;

// Constructores y destructores de Exp
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
BinaryExp::~BinaryExp() { delete left; delete right; }

UnaryExp::UnaryExp(Exp* operand, UnaryOp op):operand(operand),op(op) {}
UnaryExp::~UnaryExp() { delete operand; }

NumberExp::NumberExp(int v):value(v) {}
NumberExp::~NumberExp() { }

BoolExp::BoolExp(bool v):value(v) {}
BoolExp::~BoolExp() { }

IdentifierExp::IdentifierExp(const string& n):name(n) {}
IdentifierExp::~IdentifierExp() { }

Exp::~Exp() {}

// Nuevas expresiones para arrays y punteros
ArrayAccessExp::ArrayAccessExp(string name, Exp* idx):arrayName(name),index(idx) {}
ArrayAccessExp::~ArrayAccessExp() { delete index; }

PointerDerefExp::PointerDerefExp(string name):pointerName(name) {}
PointerDerefExp::~PointerDerefExp() { }

AddressOfExp::AddressOfExp(string name):variableName(name) {}
AddressOfExp::~AddressOfExp() { }

// Constructores y destructores de Stm
AssignStatement::AssignStatement(Exp* left, Exp* right): lhs(left), rhs(right) {}
AssignStatement::~AssignStatement() { delete lhs; delete rhs; }

PrintStatement::PrintStatement(const std::vector<Exp*>& exprs): exprs(exprs) {}
PrintStatement::~PrintStatement() { for (auto e : exprs) delete e; }
int PrintStatement::accept(Visitor* visitor) { visitor->visit(this); return 0; }
void PrintStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void PrintStatement::accept(TypeVisitor* v) { v->visit(this); }

IfStatement::IfStatement(Exp* c, Body* t, Body* e): condition(c), then(t), els(e) {}
IfStatement::~IfStatement() { delete condition; delete then; delete els; }

WhileStatement::WhileStatement(Exp* c, Body* t): condition(c), b(t) {}
WhileStatement::~WhileStatement() { delete condition; delete b; }

ForStatement::ForStatement(string var, Exp* start, Exp* end, Body* body): 
    var(var), start(start), end(end), body(body) {}
ForStatement::~ForStatement() { delete start; delete end; delete body; }

VarDec::VarDec(string type, list<string> vars): type(type), vars(vars) {}
VarDec::~VarDec() {}

VarDecList::VarDecList(): vardecs() {}
void VarDecList::add(VarDec* v) { vardecs.push_back(v); }
VarDecList::~VarDecList() {
    for (auto v: vardecs) { delete v; }
}

StatementList::StatementList(): stms() {}
void StatementList::add(Stm* s) { stms.push_back(s); }
StatementList::~StatementList() {
    for (auto s: stms) { delete s; }
}

Body::Body(VarDecList* v, StatementList* s): vardecs(v), slist(s) {}
Body::~Body() { delete vardecs; delete slist; }

Program::Program(FunctionList* functions, Body* body): functions(functions), body(body) {}
Program::~Program() { delete functions; delete body; }

Stm::~Stm() {}

// Métodos accept() para Visitor

// Expresiones
int BinaryExp::accept(Visitor* visitor) { return visitor->visit(this); }
int NumberExp::accept(Visitor* visitor) { return visitor->visit(this); }
int BoolExp::accept(Visitor* visitor) { return visitor->visit(this); }
int IdentifierExp::accept(Visitor* visitor) { return visitor->visit(this); }

int ArrayAccessExp::accept(Visitor* visitor) { return visitor->visit(this); }
int PointerDerefExp::accept(Visitor* visitor) { return visitor->visit(this); }
int AddressOfExp::accept(Visitor* visitor) { return visitor->visit(this); }
int UnaryExp::accept(Visitor* visitor) { return visitor->visit(this); }

// Sentencias
int AssignStatement::accept(Visitor* visitor) { visitor->visit(this); return 0; }
int IfStatement::accept(Visitor* visitor) { visitor->visit(this); return 0; }
int WhileStatement::accept(Visitor* visitor) { visitor->visit(this); return 0; }
int ForStatement::accept(Visitor* visitor) { visitor->visit(this); return 0; }

// Bloques
int VarDec::accept(Visitor* visitor) { visitor->visit(this); return 0; }
int VarDecList::accept(Visitor* visitor) { visitor->visit(this); return 0; }
int StatementList::accept(Visitor* visitor) { visitor->visit(this); return 0; }
int Body::accept(Visitor* visitor) { visitor->visit(this); return 0; }

// Métodos accept() para ImpValueVisitor
ImpValue BinaryExp::accept(ImpValueVisitor* v) { return v->visit(this); }
ImpValue NumberExp::accept(ImpValueVisitor* v) { return v->visit(this); }
ImpValue BoolExp::accept(ImpValueVisitor* v) { return v->visit(this); }
ImpValue IdentifierExp::accept(ImpValueVisitor* v) { return v->visit(this); }

ImpValue ArrayAccessExp::accept(ImpValueVisitor* v) { return v->visit(this); }
ImpValue PointerDerefExp::accept(ImpValueVisitor* v) { return v->visit(this); }
ImpValue AddressOfExp::accept(ImpValueVisitor* v) { return v->visit(this); }
ImpValue UnaryExp::accept(ImpValueVisitor* v) { return v->visit(this); }

void AssignStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void IfStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void WhileStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void ForStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void Program::accept(ImpValueVisitor* v) { v->visit(this); }
int Program::accept(Visitor* v) { v->visit(this); return 0; }

// Métodos accept() para TypeVisitor
ImpType BinaryExp::accept(TypeVisitor* v) { return v->visit(this); }
ImpType NumberExp::accept(TypeVisitor* v) { return v->visit(this); }
ImpType BoolExp::accept(TypeVisitor* v) { return v->visit(this); }
ImpType IdentifierExp::accept(TypeVisitor* v) { return v->visit(this); }

ImpType ArrayAccessExp::accept(TypeVisitor* v) { return v->visit(this); }
ImpType PointerDerefExp::accept(TypeVisitor* v) { return v->visit(this); }
ImpType AddressOfExp::accept(TypeVisitor* v) { return v->visit(this); }
ImpType UnaryExp::accept(TypeVisitor* v) { return v->visit(this); }

void AssignStatement::accept(TypeVisitor* v) { v->visit(this); }
void IfStatement::accept(TypeVisitor* v) { v->visit(this); }
void WhileStatement::accept(TypeVisitor* v) { v->visit(this); }
void ForStatement::accept(TypeVisitor* v) { v->visit(this); }
void Program::accept(TypeVisitor* v) { v->visit(this); }

// Nueva sentencia para break
BreakStatement::BreakStatement() {}
BreakStatement::~BreakStatement() {}
int BreakStatement::accept(Visitor* visitor) { visitor->visit(this); return 0; }
void BreakStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void BreakStatement::accept(TypeVisitor* v) { v->visit(this); }

// Función auxiliar
string Exp::binopToChar(BinaryOp op) {
    string c;
    switch(op) {
        case PLUS_OP: c = "+"; break;
        case MINUS_OP: c = "-"; break;
        case MUL_OP: c = "*"; break;
        case DIV_OP: c = "/"; break;
        case MOD_OP: c = "mod"; break;
        case LT_OP: c = "<"; break;
        case LE_OP: c = "<="; break;
        case EQ_OP: c = "=="; break;
        case GT_OP: c = ">"; break;
        case GE_OP: c = ">="; break;
        case NE_OP: c = "<>"; break;
        case AND_OP: c = "and"; break;
        case OR_OP: c = "or"; break;
        default: c = "$";
    }
    return c;
}

FunctionCallExp::FunctionCallExp(const std::string& name, const std::vector<Exp*>& args)
    : funcName(name), args(args) {}
FunctionCallExp::~FunctionCallExp() {
    for (auto a : args) delete a;
}
int FunctionCallExp::accept(Visitor* visitor) { return visitor->visit(this); }
ImpValue FunctionCallExp::accept(ImpValueVisitor* v) { return v->visit(this); }
ImpType FunctionCallExp::accept(TypeVisitor* v) { return v->visit(this); }

// Implementación de FunctionDef
FunctionDef::FunctionDef(const std::string& name, const std::vector<std::string>& params, 
                         const std::string& returnType, Body* body)
    : name(name), params(params), returnType(returnType), body(body) {}
FunctionDef::~FunctionDef() { delete body; }

// Implementación de FunctionList
FunctionList::FunctionList() : functions() {
}
void FunctionList::add(FunctionDef* func) { 
    functions.push_back(func); 
}
FunctionList::~FunctionList() {
    for (auto f : functions) delete f;
}

// Nueva sentencia para exit (Pascal)
ExitStatement::ExitStatement() {}
ExitStatement::~ExitStatement() {}
int ExitStatement::accept(Visitor* visitor) { visitor->visit(this); return 0; }
void ExitStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void ExitStatement::accept(TypeVisitor* v) { v->visit(this); }

// Nueva sentencia para new (Pascal)
NewStatement::NewStatement(string name): pointerName(name) {}
NewStatement::~NewStatement() {}
int NewStatement::accept(Visitor* visitor) { visitor->visit(this); return 0; }
void NewStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void NewStatement::accept(TypeVisitor* v) { v->visit(this); }

// Nueva sentencia para dispose (Pascal)
DisposeStatement::DisposeStatement(string name): pointerName(name) {}
DisposeStatement::~DisposeStatement() {}
int DisposeStatement::accept(Visitor* visitor) { visitor->visit(this); return 0; }
void DisposeStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void DisposeStatement::accept(TypeVisitor* v) { v->visit(this); }

void Body::accept(ImpValueVisitor* v) { v->visit(this); }
void VarDecList::accept(ImpValueVisitor* v) { v->visit(this); }
void VarDec::accept(ImpValueVisitor* v) { v->visit(this); }
void StatementList::accept(ImpValueVisitor* v) { v->visit(this); }

void Body::accept(TypeVisitor* v) { v->visit(this); }
void VarDecList::accept(TypeVisitor* v) { v->visit(this); }
void VarDec::accept(TypeVisitor* v) { v->visit(this); }
void StatementList::accept(TypeVisitor* v) { v->visit(this); }

// Métodos accept para las nuevas clases
int FunctionDef::accept(Visitor* visitor) { visitor->visit(this); return 0; }
void FunctionDef::accept(ImpValueVisitor* v) { v->visit(this); }
void FunctionDef::accept(TypeVisitor* v) { v->visit(this); }

int FunctionList::accept(Visitor* visitor) { visitor->visit(this); return 0; }
void FunctionList::accept(ImpValueVisitor* v) { v->visit(this); }
void FunctionList::accept(TypeVisitor* v) { v->visit(this); }
