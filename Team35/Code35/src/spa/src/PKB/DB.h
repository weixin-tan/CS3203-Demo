#ifndef SPA_DB_H
#define SPA_DB_H

#include <map>
#include <set>
#include <string>

// just a dataclass to hold all the tables
struct DB {
public:
  std::map<int, std::set<std::string>> modifiesStmtTable;
  std::map<int, std::set<std::string>> modifiesProcTable;

public:
  DB();
};


#endif //SPA_DB_H
