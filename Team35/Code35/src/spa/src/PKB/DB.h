#ifndef SPA_DB_H
#define SPA_DB_H

#include <map>
#include <set>
#include <string>
#include "../ParsedStatement.h"
#include "ElementType.h"

// just a dataclass to hold all the tables
struct DB {
public:
    // relationship tables
    std::map<int, ParsedStatement> stmtTable;
    std::map<int, ElementType> stmtTypeTable;
    std::map<std::string, std::set<int>> constantToStmtTable;
    std::map<int, std::set<std::string>> usesStmtToConstantTable;
    std::map<int, std::set<int>> parentTable;
    std::map<int, std::set<int>> parentTTable;
    std::map<int, std::set<int>> followsTable;
    std::map<int, std::set<int>> followsTTable;
    std::map<std::string, std::set<std::string>> callsTable;
    std::map<std::string, std::set<std::string>> callsTTable;
    std::map<std::string, std::set<std::string>> modifiesPTable;
    std::map<int, std::set<std::string>> modifiesSTable;
    std::map<std::string, std::set<std::string>> usesPTable;
    std::map<int, std::set<std::string>> usesSTable;

    // entity tables
    std::set<std::string> variables;
    std::set<std::string> procedures;
    std::set<std::string> constants;

public:
    DB();
};


#endif //SPA_DB_H
