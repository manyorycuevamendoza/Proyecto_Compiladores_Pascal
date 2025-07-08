#ifndef __IMP_VALUE_VISITOR_HH__
#define __IMP_VALUE_VISITOR_HH__

#include "exp.h"
#include "imp_value.hh"

class ImpValueVisitor {
public:
  virtual void visit(Program* p) = 0;
  virtual void visit(Body* b) = 0; 
  virtual void visit(VarDecList* e) = 0;
  virtual void visit(VarDec* e) = 0;
  virtual void visit(StatementList* e) = 0;
  virtual void visit(AssignStatement* e) = 0;
  virtual void visit(PrintStatement* e) = 0;
  virtual void visit(IfStatement* e) = 0;
  virtual void visit(WhileStatement* e) = 0;
  virtual void visit(ForStatement* e) = 0;
  virtual void visit(BreakStatement* e) = 0;
  virtual void visit(ExitStatement* e) = 0;
  virtual void visit(NewStatement* e) = 0;
  virtual void visit(DisposeStatement* e) = 0;
  virtual ImpValue visit(BinaryExp* e) = 0;
  virtual ImpValue visit(BoolExp* e) = 0;
  virtual ImpValue visit(NumberExp* e) = 0;
  virtual ImpValue visit(IdentifierExp* e) = 0;
  virtual ImpValue visit(UnaryExp* e) = 0;
  virtual ImpValue visit(ArrayAccessExp* e) = 0;
  virtual ImpValue visit(PointerDerefExp* e) = 0;
  virtual ImpValue visit(AddressOfExp* e) = 0;
  virtual ImpValue visit(FunctionCallExp* e) = 0;
  virtual void visit(FunctionDef* func) = 0;
  virtual void visit(FunctionList* funcs) = 0;
};


#endif
