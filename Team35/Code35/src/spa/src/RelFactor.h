#include "Expr.h"

#ifndef RELFACTOR_H
#define RELFACTOR_H

enum Type{VAR, CONST, EXPR};

class RelFactor {
private:
	Type type;
	Token varName;
	Token constValue;
	Expr expr;
public:
	RelFactor();
	Type getType() const;
	Token getVarName() const;
	Token getConstValue() const;
	Expr getExpr() const;
	void setType(Type type);
	void setVarName(Token varName);
	void setConstValue(Token constValue);
	void setExpr(Expr expr);
};

#endif