#include "Token.h"
#include "FactorType.h"

#ifndef FACTOR_H
#define FACTOR_H

class Expr;
class Factor {
private:
	FactorType type;
	Token varName;
	Token constValue;
	Expr* expr;
public:
	Factor();
	FactorType getType() const;
	Token getVarName() const;
	Token getConstValue() const;
	Expr* getExpr() const;
	void setType(FactorType type);
	void setVarName(Token varName);
	void setConstValue(Token constValue);
	void setExpr(Expr* expr);
};

#endif