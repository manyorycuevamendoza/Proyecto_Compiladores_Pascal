#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include <list>

class BinaryExp;
class NumberExp;
class BoolExp;
class IdentifierExp;
class UnaryExp;
class ArrayAccessExp;
class PointerDerefExp;
class AddressOfExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class WhileStatement;
class ForStatement;
class BreakStatement;
class ExitStatement;
class NewStatement;
class DisposeStatement;
class VarDec;
class VarDecList;
class StatementList;
class Body;
class Program;
class FunctionCallExp;
class FunctionDef;
class FunctionList;

class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(BoolExp* exp) = 0;
    virtual int visit(IdentifierExp* exp) = 0;
    virtual int visit(UnaryExp* exp) = 0;
    virtual int visit(ArrayAccessExp* exp) = 0;
    virtual int visit(PointerDerefExp* exp) = 0;
    virtual int visit(AddressOfExp* exp) = 0;
    virtual void visit(AssignStatement* stm) = 0;
    virtual void visit(PrintStatement* stm) = 0;
    virtual void visit(IfStatement* stm) = 0;
    virtual void visit(WhileStatement* stm) = 0;
    virtual void visit(ForStatement* stm) = 0;
    virtual void visit(BreakStatement* stm) = 0;
    virtual void visit(ExitStatement* stm) = 0;
    virtual void visit(NewStatement* stm) = 0;
    virtual void visit(DisposeStatement* stm) = 0;
    virtual void visit(VarDec* stm) = 0;
    virtual void visit(VarDecList* stm) = 0;
    virtual void visit(StatementList* stm) = 0;
    virtual void visit(Body* b) = 0;
    virtual int visit(FunctionCallExp* exp) = 0;
    virtual void visit(FunctionDef* func) = 0;
    virtual void visit(FunctionList* funcs) = 0;
    virtual void visit(Program* program) = 0;
};

class PrintVisitor : public Visitor {
public:
    void imprimir(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    int visit(UnaryExp* exp) override;
    int visit(ArrayAccessExp* exp) override;
    int visit(PointerDerefExp* exp) override;
    int visit(AddressOfExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(BreakStatement* stm) override;
    void visit(ExitStatement* stm) override;
    void visit(NewStatement* stm) override;
    void visit(DisposeStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(VarDecList* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
    int visit(FunctionCallExp* exp) override;
    void visit(FunctionDef* func) override;
    void visit(FunctionList* funcs) override;
    void visit(Program* program) override;
};

#endif