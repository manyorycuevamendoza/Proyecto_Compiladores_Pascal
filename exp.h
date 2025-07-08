#ifndef EXP_H
#define EXP_H
#include "imp_value.hh"
#include "imp_type.hh"
#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
using namespace std;
enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, MOD_OP, LT_OP, LE_OP, EQ_OP, GT_OP, GE_OP, NE_OP, AND_OP, OR_OP };
enum UnaryOp { NOT_OP, NEG_OP };

class Body;
class ImpValueVisitor;
class TypeVisitor;

class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ImpValue accept(ImpValueVisitor* v) = 0;
    virtual ImpType accept(TypeVisitor* v) = 0;
    virtual ~Exp() = 0;
    static string binopToChar(BinaryOp op);
};



class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~BinaryExp();
};

class UnaryExp : public Exp {
public:
    Exp* operand;
    UnaryOp op;
    UnaryExp(Exp* operand, UnaryOp op);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~UnaryExp();
};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~NumberExp();
};

class BoolExp : public Exp {
public:
    int value;
    BoolExp(bool v); //constructor que recibe un booleano
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~BoolExp();
};

class IdentifierExp : public Exp { //representa un identificador
public:
    std::string name;
    IdentifierExp(const std::string& n);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~IdentifierExp();
};

// Nuevas expresiones para arrays y punteros
class ArrayAccessExp : public Exp {
public:
    std::string arrayName;
    Exp* index;
    ArrayAccessExp(std::string name, Exp* idx);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~ArrayAccessExp();
};

class PointerDerefExp : public Exp {
public:
    std::string pointerName;
    PointerDerefExp(std::string name);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~PointerDerefExp();
};

class AddressOfExp : public Exp {
public:
    std::string variableName;
    AddressOfExp(std::string name);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~AddressOfExp();
};

class FunctionCallExp : public Exp {
public:
    std::string funcName;
    std::vector<Exp*> args;
    FunctionCallExp(const std::string& name, const std::vector<Exp*>& args);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ImpType accept(TypeVisitor* v);
    ~FunctionCallExp();
};

// Nueva clase para representar funciones
class FunctionDef { //representa una función
public:
    //atributos
    std::string name;
    std::vector<std::string> params;
    std::string returnType;
    Body* body;
    FunctionDef(const std::string& name, const std::vector<std::string>& params, 
                const std::string& returnType, Body* body);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~FunctionDef();
};

class FunctionList { //representa una lista de funciones
public:
    std::vector<FunctionDef*> functions;
    FunctionList();
    void add(FunctionDef* func);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~FunctionList();
};

class Stm { //representa una sentencia
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
    virtual void accept(ImpValueVisitor* v) = 0;
    virtual void accept(TypeVisitor* v) = 0;
};


class AssignStatement : public Stm {
public:
    Exp* lhs;  // Lado izquierdo como expresión (puede ser ID, ArrayAccess, PointerDeref)
    Exp* rhs;  // Lado derecho como expresión
    AssignStatement(Exp* left, Exp* right);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    std::vector<Exp*> exprs;
    PrintStatement(const std::vector<Exp*>& exprs);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~PrintStatement();
};


class IfStatement : public Stm {
public:
    Exp* condition;
    Body* then;
    Body* els;
    IfStatement(Exp* condition, Body* then, Body* els);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~IfStatement();
};

class WhileStatement : public Stm {
public:
    Exp* condition;
    Body* b;
    WhileStatement(Exp* condition, Body* b);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~WhileStatement();
};

// Nueva sentencia para bucles for
class ForStatement : public Stm {
public:
    std::string var;
    Exp* start;
    Exp* end;
    Body* body;
    ForStatement(std::string var, Exp* start, Exp* end, Body* body);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~ForStatement();
};

// Nueva sentencia para break
class BreakStatement : public Stm {
public:
    BreakStatement();
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~BreakStatement();
};



// Nueva sentencia para exit (Pascal)
class ExitStatement : public Stm {
public:
    ExitStatement();
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~ExitStatement();
};

// Nueva sentencia para new (Pascal)
class NewStatement : public Stm {
public:
    string pointerName;
    NewStatement(string name);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~NewStatement();
};

// Nueva sentencia para dispose (Pascal)
class DisposeStatement : public Stm {
public:
    string pointerName;
    DisposeStatement(string name);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~DisposeStatement();
};

class VarDec {
public:
    string type;
    list<string> vars;
    VarDec(string type, list<string> vars);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~VarDec();
};

class VarDecList{
public:
    list<VarDec*> vardecs;
    VarDecList();
    void add(VarDec* vardec);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~VarDecList();
};

class StatementList {
public:
    list<Stm*> stms;
    StatementList();
    void add(Stm* stm);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~StatementList();
};


class Body{
public:
    VarDecList* vardecs;
    StatementList* slist;
    Body(VarDecList* vardecs, StatementList* stms);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
    ~Body();
};


class Program {
public:
    FunctionList* functions;
    Body* body;
    Program(FunctionList* functions, Body* body);
    ~Program();
    int accept(Visitor* v);
    void accept(ImpValueVisitor* v);
    void accept(TypeVisitor* v);
};

#endif // EXP_H