#pragma once

#include "Forward.h"
#include "Stmt.h"
#include "location.hh"
#include <memory>
#include <string>
#include <vector>

class Expr : public Stmt {
protected:
    std::shared_ptr<Type> type;
    yy::location location;

public:
    virtual ~Expr() = default;

    std::shared_ptr<Type> getType() const;
    void setType(const std::shared_ptr<Type>& t);

    yy::location getLocation() const;
};

class IntegerLiteral final
    : public Expr
    , public std::enable_shared_from_this<IntegerLiteral> {
private:
    int64_t value;

public:
    void traverse(ASTVisitor& visitor) override;

    int64_t getValue() const noexcept { return value; }

    static std::shared_ptr<IntegerLiteral> make(const yy::location& loc, int64_t v);
};

class DoubleLiteral final
    : public Expr
    , public std::enable_shared_from_this<DoubleLiteral> {
private:
    double value;

public:
    void traverse(ASTVisitor& visitor) override;

    double getValue() const noexcept { return value; }

    static std::shared_ptr<DoubleLiteral> make(const yy::location& loc, double v);
};

class BoolLiteral final
    : public Expr
    , public std::enable_shared_from_this<BoolLiteral> {
private:
    bool value;

public:
    void traverse(ASTVisitor& visitor) override;

    bool getValue() const noexcept { return value; }

    static std::shared_ptr<BoolLiteral> make(const yy::location& loc, bool v);
};

class StringLiteral final
    : public Expr
    , public std::enable_shared_from_this<StringLiteral> {
private:
    std::string value;

public:
    void traverse(ASTVisitor& visitor) override;

    std::string getValue() const noexcept { return value; }

    static std::shared_ptr<StringLiteral> make(const yy::location& loc, const std::string& v);
};

class CallExpr final
    : public Expr
    , public std::enable_shared_from_this<CallExpr> {
private:
    std::shared_ptr<Expr> callee;
    std::vector<std::shared_ptr<Expr>> arguments;

public:
    void traverse(ASTVisitor& visitor) override;

    std::shared_ptr<Expr> getCallee() const { return callee; }
    std::vector<std::shared_ptr<Expr>> getArguments() const { return arguments; }

    static std::shared_ptr<CallExpr> make(
        const yy::location& loc,
        const std::shared_ptr<Expr>& fn,
        const std::vector<std::shared_ptr<Expr>>& args);
};

enum class BinaryOperatorKind {
    Add,
    Subtract,
    Multiply,
    Divide,
    Mod,
    Assign,
    Equal,
    NotEqual,
    LogicalAnd, // &&
    LogicalOr,  // ||

    // LogicalNot, // !
    // And, // &
    // Xor, // ^
    // Or, // |
};

class BinaryOperator final
    : public Expr
    , public std::enable_shared_from_this<BinaryOperator> {
private:
    BinaryOperatorKind kind;
    std::shared_ptr<Expr> lhs;
    std::shared_ptr<Expr> rhs;

public:
    void traverse(ASTVisitor& visitor) override;

    BinaryOperatorKind getKind() const { return kind; }
    std::shared_ptr<Expr> getLHS() const { return lhs; }
    std::shared_ptr<Expr> getRHS() const { return rhs; }

    static std::shared_ptr<BinaryOperator> make(
        const yy::location& loc,
        BinaryOperatorKind k,
        const std::shared_ptr<Expr>& l,
        const std::shared_ptr<Expr>& r);
};

class DeclRefExpr final
    : public Expr
    , public std::enable_shared_from_this<DeclRefExpr> {
private:
    std::shared_ptr<NamedDecl> decl;

public:
    void traverse(ASTVisitor& visitor) override;

    std::shared_ptr<NamedDecl> getNamedDecl() const { return decl; }

    static std::shared_ptr<DeclRefExpr>
    make(const yy::location& loc, const std::shared_ptr<NamedDecl>& d);
};

class MemberExpr final
    : public Expr
    , public std::enable_shared_from_this<MemberExpr> {
private:
    std::shared_ptr<Expr> base;
    std::shared_ptr<NamedDecl> memberDecl;

public:
    void traverse(ASTVisitor& visitor) override;

    std::shared_ptr<Expr> getBase() const;

    std::shared_ptr<NamedDecl> getMemberDecl() const;

    static std::shared_ptr<MemberExpr> make(
        const yy::location& loc,
        const std::shared_ptr<Expr>& base,
        const std::shared_ptr<NamedDecl>& d);
};
