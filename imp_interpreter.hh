#ifndef IMP_INTERPRETER
#define IMP_INTERPRETER

#include <unordered_map>

#include "exp.h"
#include "imp_value_visitor.hh"
#include "environment.hh"

using namespace std;

class ImpInterpreter : public ImpValueVisitor {
private:
  Environment<ImpValue> env;
  std::unordered_map<std::string, FunctionDef*> functions;
  void update_multi_array(Exp* exp, ImpValue v);
  ImpValue get_multi_array(Exp* exp);

public:
  ImpInterpreter();
  ~ImpInterpreter();
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
  void visit(ForStatement* s);
  void visit(BreakStatement* s);
  void visit(ExitStatement* s);
  void visit(NewStatement* s);
  void visit(DisposeStatement* s);
};


#endif

