#ifndef SPA_SRC_SPA_SRC_WITHHANDLER_H_
#define SPA_SRC_SPA_SRC_WITHHANDLER_H_

#include "PkbGetter.h"
#include "Result.h"
#include "RelationshipRef.h"

class WithHandler {
 public:
  explicit WithHandler(PkbGetter* pg);
  void handlePattern(const Entity& entityToGet, const RelationshipRef& relRef);
 private:
  void handleBothSidesFixed(const Entity& leftEntity, const Entity& rightEntity);
  void handleOneSideFixed(const Entity& leftEntity, const Entity& rightEntity);
  void handleNoSideFixed(const Entity& leftEntity, const Entity& rightEntity);

  void getAllProgramElements(const RelationshipType& rtype);
  void matchProgramElements();
};

#endif //SPA_SRC_SPA_SRC_WITHHANDLER_H_
