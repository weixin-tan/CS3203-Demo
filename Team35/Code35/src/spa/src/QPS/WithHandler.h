#ifndef SPA_SRC_SPA_SRC_WITHHANDLER_H_
#define SPA_SRC_SPA_SRC_WITHHANDLER_H_

#include "../PKB/PkbGetter.h"
#include "Result.h"
#include "RelationshipRef.h"
#include "../QpsTypeToPkbTypeConvertor.h"

class WithHandler {
public:
    explicit WithHandler(PkbGetter* pg);
    Result handleWith(const RelationshipRef& relRef);

private:
    PkbGetter* pg;
    static bool isFixedEntity(const Entity& entity);
    std::set<ProgramElement> getProgramElements(const EntityType& eType);
    static std::string getStringToMatch(const ProgramElement& element, EntityAttributeType elementType);
    static std::set<ProgramElement> matchProgramElements(const std::set<ProgramElement>& setA,
                                                         const Entity& fixedEntity,
                                                         EntityAttributeType aType);
    static std::set<std::pair<ProgramElement,
                              ProgramElement>> matchProgramElements(const std::set<ProgramElement>& setA,
                                                                    const std::set<ProgramElement>& setB,
                                                                    EntityAttributeType aType,
                                                                    EntityAttributeType bType);

    bool handleBothSidesFixed(const Entity& leftEntity, const Entity& rightEntity);
    std::set<ProgramElement> handleOneSideFixed(const Entity& fixedSide, const Entity& notFixedSide);
    std::set<std::pair<ProgramElement, ProgramElement>> handleNoSideFixed(const Entity& leftEntity,
                                                                          const Entity& rightEntity);
};

#endif //SPA_SRC_SPA_SRC_WITHHANDLER_H_
