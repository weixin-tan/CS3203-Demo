#ifndef SPA_DB_H
#define SPA_DB_H

#include <map>
#include <set>
#include <string>
#include "../SP/ParsedStatement.h"
#include "ElementType.h"
#include "ProgramElement.h"

// just a dataclass to hold all the tables
// TODO: package the table and reverse table nicely?
struct DB {
public:
    // relationship tables
    std::map<int, ParsedStatement> stmtTable;
    std::map<int, ProgramElement> elementStmtTable;
    std::map<int, std::set<int>> parentTable;
    std::map<int, std::set<int>> parentTableR;
    std::map<int, std::set<int>> parentTTable;
    std::map<int, std::set<int>> parentTTableR;
    std::map<int, std::set<int>> followsTable;
    std::map<int, std::set<int>> followsTableR;
    std::map<int, std::set<int>> followsTTable;
    std::map<int, std::set<int>> followsTTableR;
    std::map<std::string, std::set<std::string>> callsTable;
    std::map<std::string, std::set<std::string>> callsTableR;
    std::map<std::string, std::set<std::string>> callsTTable;
    std::map<std::string, std::set<std::string>> callsTTableR;
    std::map<std::string, std::set<std::string>> modifiesPTable;
    std::map<std::string, std::set<std::string>> modifiesPTableR;
    std::map<int, std::set<std::string>> modifiesSTable;
    std::map<std::string, std::set<int>> modifiesSTableR;
    std::map<std::string, std::set<std::string>> usesPTable;
    std::map<std::string, std::set<std::string>> usesPTableR;
    std::map<int, std::set<std::string>> usesSTable;
    std::map<std::string, std::set<int>> usesSTableR;
    std::map<int, std::set<int>> nextTable;
    std::map<int, std::set<int>> nextTableR;
    std::map<int, Expr> exprTable;

    // table for caching, to be cleared between queries
    std::map<int, std::set<int>> nextTTable;
    std::map<int, std::set<int>> nextTTableR;
    std::set<int> computedNextTSrc;
    std::set<int> computedNextTRSrc;
    std::map<int, std::set<int>> affectsTable;
    std::map<int, std::set<int>> affectsTableR;
    std::set<int> computedAffectsSrc;
    std::set<int> computedAffectsRSrc;
    std::map<int, std::set<int>> affectsTTable;
    std::map<int, std::set<int>> affectsTTableR;
    std::set<int> computedAffectsTSrc;
    std::set<int> computedAffectsTRSrc;

    // entity tables
    std::set<std::string> variables;
    std::set<std::string> procedures;
    std::set<std::string> constants;

public:
    DB();
    void clearCache();
};


#endif //SPA_DB_H
