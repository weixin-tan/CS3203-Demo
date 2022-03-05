#include "ExpressionProcessor.h"

Expr ExpressionProcessor::stringToExpr(std::string query) {
	return Expr(); 
}

bool ExpressionProcessor::fullfillsMatching(Expr exp1, Expr exp2, ExpressionIndicator expressionIndicator) {
	if (expressionIndicator == ExpressionIndicator::FULL_MATCH) {
		return areIdenticalExpr(&exp1, &exp2);
	}
	else {
		return isSubtree(&exp1, &exp2);
	}
}

// We can only check whether expressions are similar. 
// returns True or False if the expressions are similar. 
bool ExpressionProcessor::areIdenticalExpr(Expr *root1, Expr *root2) {
	if (root1 == nullptr && root2 == nullptr) {
		return true; 
	}
	if (root1 == nullptr || root2 == nullptr) {
		return false;
	}

	return root1->getOperator() == root2->getOperator() &&
		areIdenticalExpr(root1->getExprPtr(), root2->getExprPtr()) &&
		areIdenticalTerm(root1->getTermPtr(), root2->getTermPtr());
}

bool ExpressionProcessor::areIdenticalTerm(Term* root1, Term* root2) {
	if (root1 == nullptr && root2 == nullptr) {
		return true;
	}
	if (root1 == nullptr || root2 == nullptr) {
		return false;
	}
	return root1->getOperator() == root2->getOperator() &&
		areIdenticalFactor(root1->getFactorPtr(), root2->getFactorPtr()) &&
		areIdenticalTerm(root1->getTermPtr(), root2->getTermPtr());

}

bool ExpressionProcessor::areIdenticalFactor(Factor* root1, Factor* root2) {
	if (root1 == nullptr && root2 == nullptr) {
		return true;
	}
	if (root1 == nullptr || root2 == nullptr) {
		return false;
	}
	return root1->getType() == root2->getType() &&
		root1->getVarName().getId() == root2->getVarName().getId() &&
		root1->getConstValue().getId() == root2->getConstValue().getId() &&
		root1->getType() == root2->getType() &&
		areIdenticalExpr(root1->getExpr().get(), root2->getExpr().get());
}

// Checking if they are partial match or not. 
bool ExpressionProcessor::isSubtree(Expr* root1, Expr* root2) {
	if (root2 == nullptr) {
		return true; 
	}
	if (root1 == nullptr) {
		return false; 
	}
	if (areIdenticalExpr(root1, root2)) {
		return true;
	} else {

		return isSubtree(root1, root2->getExprPtr()) || 
		// We need to get the NEXT expression out. 
		isSubtree(root1, getNestedExpr(root2).get());
	}
}

std::shared_ptr<Expr> ExpressionProcessor::getNestedExpr(Expr* root2)
{
	if (&root2->getTerm() != nullptr) {
		Term* term = root2->getTermPtr();
		if (&term->getFactor() != nullptr) {
			Factor* factor = term->getFactorPtr();
			if (factor->getExpr() != nullptr) {
				return factor->getExpr();
			}
		}
	}
	return nullptr;
}
