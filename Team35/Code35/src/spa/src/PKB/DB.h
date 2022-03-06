#ifndef SPA_DB_H
#define SPA_DB_H

#include <map>
#include <set>
#include <string>
#include "../ParsedStatement.h"
#include "ElementType.h"

// just a dataclass to hold all the tables
struct DB {
    // TODO: do setter/getter for the table as well
    // TODO: deprecate unnecessary tables
public:
    // relationship tables
    std::map<int, ParsedStatement> stmtTable;
    std::map<int, std::set<std::string>> modifyStmtToVarTable;
    std::map<std::string, std::set<std::string>> modifyProcToVarTable;
    std::map<std::string, std::set<int>> varToModifyStmtTable;
    std::map<std::string, std::set<std::string>> varToModifyProcTable;
    std::map<int, std::set<std::string>> usesStmtToVarTable;
    std::map<std::string, std::set<std::string>> usesProcToVarTable;
    std::map<std::string, std::set<int>> varToUsesStmtTable;
    std::map<std::string, std::set<std::string>> varToUsesProcTable;
    std::map<int, ElementType> stmtTypeTable;
    std::map<int, std::set<int>> parentToChildTable;
    std::map<int, int> childToParentTable;
    std::map<int, std::string> stmtToProcedureCalled;
    std::map<int, int> stmtPreceding;
    std::map<int, int> stmtFollowing;
    std::map<std::string, std::set<int>> constantToStmtTable;
    std::map<int, std::set<std::string>> usesStmtToConstantTable;
    std::map<int, std::set<int>> followsTable;
    std::map<int, std::set<int>> followsTTable;
    std::map<std::string, std::set<std::string>> callsTable;
    std::map<std::string, std::set<std::string>> callsTTable;

    // entity tables
    std::set<std::string> variables;
    std::set<std::string> procedures;
    std::set<std::string> constants;

public:
    DB();
};


#endif //SPA_DB_H
