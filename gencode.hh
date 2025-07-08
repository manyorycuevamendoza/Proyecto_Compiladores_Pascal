#ifndef GENCODE
#define GENCODE

#include <unordered_map>

#include "exp.h"
#include "visitor.h"
#include "imp_value_visitor.hh"
#include "environment.hh"

using namespace std;

class GenCodeVisitor : public Visitor {
private:
  Environment<ImpValue> env;
  std::string current_function_name;
  

public:
  int current_offset;
  std::unordered_map<std::string, int> stack_offsets;
  int etiquetas=0;
  void generar(Program* program);
  void visit(Program*);
  void visit(Body*);
  void visit(VarDecList*);
  void visit(VarDec*);
  void visit(StatementList*);
  void visit(AssignStatement*);
  void visit(PrintStatement*);
  void visit(IfStatement*);
  void visit(WhileStatement*);
  void visit(ForStatement*);
  int visit(BinaryExp* e);
  int visit(UnaryExp* e);
  int visit(BoolExp* e);
  int visit(NumberExp* e);
  int visit(IdentifierExp* e);
  int visit(ArrayAccessExp* e);
  int visit(PointerDerefExp* e);
  int visit(AddressOfExp* e);
  int visit(FunctionCallExp* e);
  void visit(FunctionDef* func);
  void visit(FunctionList* funcs);
  void visit(BreakStatement* s);
  void visit(ExitStatement* s);
  void visit(NewStatement* s);
  void visit(DisposeStatement* s);
};

// Mantener la clase original para compatibilidad
class ImpCODE : public ImpValueVisitor {
private:
  Environment<ImpValue> env;
  std::string current_function_name;
  

public:
  int current_offset;
  std::unordered_map<std::string, int> stack_offsets;
  int etiquetas=0;
  void interpret(Program*);
  void visit(Program*);
  void visit(Body*);
  void visit(VarDecList*);
  void visit(VarDec*);
  void visit(StatementList*);
  void visit(AssignStatement*);
  void visit(PrintStatement*);
  void visit(IfStatement*);
  void visit(WhileStatement*);
  void visit(ForStatement*);
  ImpValue visit(BinaryExp* e);
  ImpValue visit(UnaryExp* e);
  ImpValue visit(BoolExp* e);
  ImpValue visit(NumberExp* e);
  ImpValue visit(IdentifierExp* e);
  ImpValue visit(ArrayAccessExp* e);
  ImpValue visit(PointerDerefExp* e);
  ImpValue visit(AddressOfExp* e);
  ImpValue visit(FunctionCallExp* e);
  void visit(FunctionDef* func);
  void visit(FunctionList* funcs);
  void visit(BreakStatement* s);
  void visit(ExitStatement* s);
  void visit(NewStatement* s);
  void visit(DisposeStatement* s);
};


#endif

