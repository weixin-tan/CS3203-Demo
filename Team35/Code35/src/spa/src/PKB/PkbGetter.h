#ifndef SPA_PKBGETTER_H
#define SPA_PKBGETTER_H

#include <vector>
#include "ProgramElement.h"
#include "DB.h"

class PkbGetter {
private:
    DB* db;

private:
    bool isExists(const ProgramElement& elementToCheck) const;
    std::set<int> getChildStmtNos(int parentStmtNo) const;
    std::set<std::string> getModifiedVars(int parentStmtNo) const;
    std::set<int> getModifiesStmtNosGivenVariable(const std::string& var) const;
    std::set<std::string> getModifiesProcs(const std::string& var) const;
    std::set<std::string> getVariableGivenModifyProc(const std::string& proc) const;
    std::set<std::string> getUsesProcs(const std::string& var) const;
    std::set<std::string> getVariableGivenUsesProc(const std::string& proc) const;
    std::set<std::string> getUsedVars(int parentStmtNo) const;
    std::set<int> getUsesStmtNosGivenVariable(const std::string& var) const;
    std::set<int> getUsesStmtNosGivenConstant(const std::string& c) const;
    int getParentStmtNo(int childStmtNo) const;
    int getFollowingStmtNo(int stmtNo) const;
    int getPrecedingStmtNo(int stmtNo) const;
    ElementType getStmtType(int stmtNo) const;
    std::set<std::string> getUsedConstants(int stmtNo) const;

public:
    explicit PkbGetter(DB* db);
    bool isRelationship(const RelationshipType& r, const ProgramElement& leftSide, const ProgramElement& rightSide) const;
    std::set<ProgramElement> getEntity(const ElementType& typeToGet) const;
    std::set<ProgramElement> getRelationshipStatements(const RelationshipType& r) const;
    std::set<ProgramElement> getLeftSide(const RelationshipType& r, const ProgramElement& rightSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getRightSide(const RelationshipType& r, const ProgramElement& leftSide, const ElementType& typeToGet) const;
    std::set<ProgramElement> getAssignmentGivenExpression(const ProgramElement& expression) const; // TODO: temporary pattern
    std::set<ProgramElement> getAssignmentGivenVariableAndExpression(const ProgramElement& variable, const ProgramElement& expression) const;
};

#endif //SPA_PKBGETTER_H
