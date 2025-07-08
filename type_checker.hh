#ifndef TYPE_CHECKER_HH
#define TYPE_CHECKER_HH

#include "type_visitor.hh"
#include "environment.hh"
#include <unordered_map>
#include <string>

class TypeChecker : public TypeVisitor {
private:
    Environment<ImpType> type_env;
    std::unordered_map<std::string, FunctionDef*> functions;
    std::unordered_map<std::string, ImpType> type_definitions;
    
    // Métodos auxiliares
    ImpType get_type_from_string(const std::string& type_name);
    bool is_compatible(ImpType t1, ImpType t2);
    void check_expression_type(Exp* exp, ImpType expected_type, const std::string& context);

public:
    TypeChecker();
    ~TypeChecker();
    
    // Métodos principales
    void check(Program* program);
    
    // Implementación de TypeVisitor
    void visit(Program* p) override;
    void visit(Body* b) override;
    void visit(VarDecList* e) override;
    void visit(VarDec* e) override;
    void visit(StatementList* e) override;
    void visit(AssignStatement* e) override;
    void visit(PrintStatement* e) override;
    void visit(IfStatement* e) override;
    void visit(WhileStatement* e) override;
    void visit(ForStatement* s);
    void visit(BreakStatement* s);
    void visit(ExitStatement* s);
    void visit(NewStatement* s);
    void visit(DisposeStatement* s);
    void visit(FunctionDef* func);
    void visit(FunctionList* funcs);
    
    ImpType visit(BinaryExp* e) override;
    ImpType visit(UnaryExp* e);
    ImpType visit(BoolExp* e) override;
    ImpType visit(NumberExp* e) override;
    ImpType visit(IdentifierExp* e) override;
    ImpType visit(ArrayAccessExp* e);
    ImpType visit(PointerDerefExp* e);
    ImpType visit(AddressOfExp* e);
    ImpType visit(FunctionCallExp* e);
};

#endif 