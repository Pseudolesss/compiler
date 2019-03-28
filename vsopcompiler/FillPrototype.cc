#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include "ASTnode.hh"
#include "prototype.hh"
#include "FillPrototype.hh"

    std::string FillPrototype::visit(ASTnode *asTnode) { return "useless"; }

    std::string FillPrototype::visit(Expr *expr) { return "useless"; }

    std::string FillPrototype::visit(Type *type) { return "useless"; }

    std::string FillPrototype::visit(Field *field)
    {
        if (field->getExpr() == nullptr)
        {
            return "useless";
        }
        return "useless";
    }

    std::string FillPrototype::visit(Formal *formal) { return "useless"; }

    std::string FillPrototype::visit(Formalx *formalx)
    {
        if (formalx->getFormal() == nullptr)
            return "useless";
        else
            return "useless";
    }

    std::string FillPrototype::visit(Formals *formals)
    {
        if (formals->getFormal() == nullptr)
            return "useless";
        else
            return "useless";
    }

    std::string FillPrototype::visit(Exprx *exprx)
    {
        if (exprx->getExprx() == nullptr)
            return "useless";
        return "useless";
    }

    std::string FillPrototype::visit(Exprxx *exprxx)
    {
        if (exprxx->getExpr() == nullptr)
        {
            return "useless";
        }
        else
        {
            return "useless";
        }
    }

    std::string FillPrototype::visit(Block *block)
    {

        if (block->getExprx()->getExpr() == nullptr)
            return "useless";
        else
            return "useless";
    }

    std::string FillPrototype::visit(Method *method) { return "useless"; }

    std::string FillPrototype::visit(FieldMethod *fieldMethod)
    {
        if (fieldMethod->getFieldMethod() == nullptr)
            return "useless";
        if (fieldMethod->getMethod() == nullptr)
            return "useless";
        else
            return "useless";
    }

    std::string FillPrototype::visit(Body *body)
    {

        std::vector<std::string> m;
        std::vector<std::string> f;
        std::vector<std::string> tokens;

        std::string text = body->getFieldMethod()->accept(this);

        while (!text.empty())
        {
            tokens.push_back(text.substr(0, text.find('$')));
            text = text.erase(0, text.find('$') + 1);
        }

        for (const auto &t : tokens)
        {
            if (t[0] == 'M')
                m.push_back(t);
            else if (t[0] == 'F')
                f.push_back(t);
            else
                m.push_back("ERROR: " + t);
        }

        std::string ret;

        // if no element
        if (f.empty())
            ret += "[], ";
        else
            for (int i = 0; i < f.size(); i++)
            {
                // if one element only
                if (f.size() == 1)
                {
                    ret += "[" + f[i] + "], ";
                    continue;
                }
                // initial elem for size > 1
                if (i == 0)
                {
                    ret += "[" + f[i];
                    continue;
                }
                // last elem for size > 1
                if (i == f.size() - 1)
                {
                    ret += ", " + f[i] + "] ,";
                    continue;
                }
                // intermediate for size > 1
                ret += ", " + f[i];
            }

        // if no element
        if (m.empty())
            ret += "[]";
        else
            for (int i = 0; i < m.size(); i++)
            {
                // if one element only
                if (m.size() == 1)
                {
                    ret += "[" + m[i] + "]";
                    continue;
                }
                // initial elem for size > 1
                if (i == 0)
                {
                    ret += "[" + m[i];
                    continue;
                }
                // last elem for size > 1
                if (i == m.size() - 1)
                {
                    ret += ", " + m[i] + "]";
                    continue;
                }
                // intermediate for size > 1
                ret += ", " + m[i];
            }

        return "useless";
    }

    std::string FillPrototype::visit(Classe *classe)
    {
        if (classe->getParentID().empty())
            return "useless";
        else
            return "useless";
    }

    std::string FillPrototype::visit(Classes *classes)
    {
        if (classes->nextClass() == nullptr)
            return "useless";
        else
            return "useless";
    }

    std::string FillPrototype::visit(Programm *programm) { ::prototype["class"] = ClassPrototype(); ::prototype["Programm"].field["yolo"] = "int32";}

    std::string FillPrototype::visit(Dual *dual) { return "useless"; }

    std::string FillPrototype::visit(Unary *unary) { return "useless"; }

    std::string FillPrototype::visit(If *anIf)
    {
        if (anIf->getElse() == nullptr)
        {
            return "useless";
        }
        return "useless";
    }

    std::string FillPrototype::visit(While *aWhile) { return "useless"; }

    std::string FillPrototype::visit(Let *let)
    {
        if (let->getAssign() == nullptr)
        {
            return "useless";
        }
        return "useless";
    }

    std::string FillPrototype::visit(Assign *assign) { return "useless"; }

    std::string FillPrototype::visit(Not *aNot) { return "useless"; }

    std::string FillPrototype::visit(And *anAnd) { return "useless"; }

    std::string FillPrototype::visit(Equal *equal) { return "useless"; }

    std::string FillPrototype::visit(Lower *lower) { return "useless"; }

    std::string FillPrototype::visit(LowerEqual *lowerEqual) { return "useless"; }

    std::string FillPrototype::visit(Plus *plus) { return "useless"; }

    std::string FillPrototype::visit(Minus *minus) { return "useless"; }

    std::string FillPrototype::visit(Times *times) { return "useless"; }

    std::string FillPrototype::visit(Div *div) { return "useless"; }

    std::string FillPrototype::visit(Pow *pow) { return "useless"; }

    std::string FillPrototype::visit(Minus1 *minus1) { return "useless"; }

    std::string FillPrototype::visit(IsNull *isNull) { return "useless"; }

    std::string FillPrototype::visit(Args *args)
    {
        if (args->getExpr() != nullptr)
        {
            return "useless";
        }
        else
        {
            return "useless";
        }
    }

    std::string FillPrototype::visit(Function *function) { return "useless"; }

    std::string FillPrototype::visit(Dot *dot) { return "useless"; }

    std::string FillPrototype::visit(New *aNew) { return "useless"; }

    std::string FillPrototype::visit(ObjID *objID) { return "useless"; }

    std::string FillPrototype::visit(Literal *literal) { return "useless"; }

    std::string FillPrototype::visit(IntLit *intLit) { return "useless"; }

    std::string FillPrototype::visit(StrLit *strLit) { return "useless"; }

    std::string FillPrototype::visit(BoolLit *boolLit) { return "useless"; }

    std::string FillPrototype::visit(Lpar *lpar) { return "useless"; }

    std::string FillPrototype::visit(Rpar *rpar) { return "useless"; }

    std::string FillPrototype::visit(Parenthese *parenthese) { return "useless"; }
