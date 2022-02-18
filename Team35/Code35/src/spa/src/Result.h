#ifndef SPA_RESULT_H
#define SPA_RESULT_H

#include <set>
#include "ProgramElement.h"

class Result {
 private:
  std::set<ProgramElement> programElements = {};
 public:
  Result();
  explicit Result(std::set<ProgramElement> programElements);
  void insertProgramElement(ProgramElement e);
  std::set<ProgramElement> getProgramElements();
};


#endif //SPA_RESULT_H
