#include "AST/Expr.h"
#include "AST/ASTDumper.h"
#include "AST/ASTVisitor.h"
#include "AST/Decl.h"
#include <cassert>
#include <utility>

std::shared_ptr<Type> Expr::getType() const
{
    return type;
}

void Expr::setType(const std::shared_ptr<Type>& t)
{
    assert(t);
    type = t;
}

void IntegerLiteral::traverse(ASTVisitor& visitor)
{
    visitor.visit(shared_from_this());
}

std::shared_ptr<IntegerLiteral> IntegerLiteral::make(const Location& loc, int64_t v)
{
    auto expr = std::make_shared<IntegerLiteral>();
    expr->location = loc;
    expr->value = v;
    return expr;
}

void DoubleLiteral::traverse(ASTVisitor& visitor)
{
    visitor.visit(shared_from_this());
}

std::shared_ptr<DoubleLiteral> DoubleLiteral::make(const Location& loc, double v)
{
    auto expr = std::make_shared<DoubleLiteral>();
    expr->location = loc;
    expr->value = v;
    return expr;
}

void BoolLiteral::traverse(ASTVisitor& visitor)
{
    visitor.visit(shared_from_this());
}

std::shared_ptr<BoolLiteral> BoolLiteral::make(const Location& loc, bool v)
{
    auto expr = std::make_shared<BoolLiteral>();
    expr->location = loc;
    expr->value = v;
    return expr;
}

void StringLiteral::traverse(ASTVisitor& visitor)
{
    visitor.visit(shared_from_this());
}

std::shared_ptr<StringLiteral> StringLiteral::make(const Location& loc, const std::string& v)
{
    auto expr = std::make_shared<StringLiteral>();
    expr->location = loc;
    expr->value = v;
    return expr;
}

void CallExpr::traverse(ASTVisitor& visitor)
{
    assert(callee);
    visitor.visit(shared_from_this(), [&] {
        callee->traverse(visitor);
        for (const auto& arg : arguments) {
            arg->traverse(visitor);
        }
    });
}

std::shared_ptr<CallExpr> CallExpr::make(
    const Location& loc,
    const std::shared_ptr<Expr>& fn,
    const std::vector<std::shared_ptr<Expr>>& args)
{
    auto expr = std::make_shared<CallExpr>();
    expr->location = loc;
    expr->callee = fn;
    expr->arguments = args;
    return expr;
}

void FunctionExpr::traverse(ASTVisitor& visitor)
{
    assert(compoundStmt);
    visitor.visit(shared_from_this(), [&] {
        if (namedDecl) {
            namedDecl->traverse(visitor);
        }
        for (const auto& param : parameters) {
            param->traverse(visitor);
        }
        compoundStmt->traverse(visitor);
    });
}

std::shared_ptr<NamedDecl> FunctionExpr::getNamedDecl() const
{
    return namedDecl;
}

const std::vector<std::shared_ptr<ParmVarDecl>>& FunctionExpr::getParameters() const
{
    return parameters;
}

std::shared_ptr<NamedDecl> FunctionExpr::getReturnType() const
{
    return returnType;
}

std::shared_ptr<CompoundStmt> FunctionExpr::getBody() const
{
    return compoundStmt;
}

std::shared_ptr<FunctionExpr> FunctionExpr::make(
    const Location& loc,
    const std::shared_ptr<NamedDecl>& n,
    const std::vector<std::shared_ptr<ParmVarDecl>>& parameters,
    const std::shared_ptr<NamedDecl>& returnType,
    const std::shared_ptr<CompoundStmt>& s)
{
    auto expr = std::make_shared<FunctionExpr>();
    expr->location = loc;
    expr->namedDecl = n;
    expr->parameters = parameters;
    expr->returnType = returnType;
    expr->compoundStmt = s;
    return expr;
}

void BinaryOperator::traverse(ASTVisitor& visitor)
{
    visitor.visit(shared_from_this(), [&] {
        assert(lhs);
        assert(rhs);
        lhs->traverse(visitor);
        rhs->traverse(visitor);
    });
}

std::shared_ptr<BinaryOperator> BinaryOperator::make(
    const Location& loc,
    BinaryOperatorKind k,
    const std::shared_ptr<Expr>& l,
    const std::shared_ptr<Expr>& r)
{
    auto expr = std::make_shared<BinaryOperator>();
    expr->location = loc;
    expr->kind = k;
    expr->lhs = l;
    expr->rhs = r;
    return expr;
}

void UnaryOperator::traverse(ASTVisitor& visitor)
{
    visitor.visit(shared_from_this(), [&] {
        assert(subExpr);
        subExpr->traverse(visitor);
    });
}

std::shared_ptr<UnaryOperator>
UnaryOperator::make(const Location& loc, UnaryOperatorKind k, const std::shared_ptr<Expr>& e)
{
    auto expr = std::make_shared<UnaryOperator>();
    expr->location = loc;
    expr->kind = k;
    expr->subExpr = e;
    return expr;
}

void DeclRefExpr::traverse(ASTVisitor& visitor)
{
    assert(decl);
    visitor.visit(shared_from_this(), [&] { decl->traverse(visitor); });
}

std::shared_ptr<DeclRefExpr>
DeclRefExpr::make(const Location& loc, const std::shared_ptr<NamedDecl>& d)
{
    auto expr = std::make_shared<DeclRefExpr>();
    expr->location = loc;
    expr->decl = d;
    return expr;
}

void MemberExpr::traverse(ASTVisitor& visitor)
{
    visitor.visit(shared_from_this(), [&] {
        assert(base);
        assert(memberDecl);
        base->traverse(visitor);
        memberDecl->traverse(visitor);
    });
}

std::shared_ptr<Expr> MemberExpr::getBase() const
{
    return base;
}

std::shared_ptr<NamedDecl> MemberExpr::getMemberDecl() const
{
    return memberDecl;
}

std::shared_ptr<MemberExpr> MemberExpr::make(
    const Location& loc, const std::shared_ptr<Expr>& base, const std::shared_ptr<NamedDecl>& d)
{
    auto expr = std::make_shared<MemberExpr>();
    expr->location = loc;
    expr->base = base;
    expr->memberDecl = d;
    return expr;
}


std::string ASTHelper::toString(BinaryOperatorKind kind)
{
    switch (kind) {
    case BinaryOperatorKind::Assign: return "=";
    case BinaryOperatorKind::Add: return "+";
    case BinaryOperatorKind::Subtract: return "-";
    case BinaryOperatorKind::Divide: return "/";
    case BinaryOperatorKind::Multiply: return "*";
    case BinaryOperatorKind::Mod: return "%";
    case BinaryOperatorKind::Equal: return "==";
    case BinaryOperatorKind::NotEqual: return "!=";
    case BinaryOperatorKind::LogicalAnd: return "&&";
    case BinaryOperatorKind::LogicalOr: return "||";
    case BinaryOperatorKind::GreaterThan: return ">";
    case BinaryOperatorKind::GreaterThanOrEqual: return ">=";
    case BinaryOperatorKind::LessThan: return "<";
    case BinaryOperatorKind::LessThanOrEqual: return "<=";
    }
    return "<unknown>";
}

std::string ASTHelper::toString(UnaryOperatorKind kind)
{
    switch (kind) {
    case UnaryOperatorKind::LogicalNot: return "!";
    case UnaryOperatorKind::Plus: return "+";
    case UnaryOperatorKind::Minus: return "-";
    case UnaryOperatorKind::PreDec: return "prefix --";
    case UnaryOperatorKind::PreInc: return "prefix ++";
    case UnaryOperatorKind::PostDec: return "postfix --";
    case UnaryOperatorKind::PostInc: return "postfix ++";
    }
    return "<unknown>";
}
