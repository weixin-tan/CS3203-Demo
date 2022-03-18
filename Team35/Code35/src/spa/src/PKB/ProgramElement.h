#ifndef SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PROGRAMELEMENT_H_
#define SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PROGRAMELEMENT_H_

#include "../Type.h"
#include "ElementType.h"
#include <string>

class ProgramElement {
private:
    std::tuple<ElementType, int, std::string, std::string, std::string> tuple;

public:
    static const int nullIntegerValue = -1;
    static const std::string nullStringValue;

public:
    // TODO: put back const
    ElementType elementType;
    int stmtNo;
    std::string procName;
    std::string varName;
    std::string value;

private:
    ProgramElement(ElementType elementType, int stmtNo, const std::string& procName, const std::string& varName, const std::string& value);

public:
    static ProgramElement createStatement(ElementType elementType, int stmtNo, const std::string& procOrVarName = ProgramElement::nullStringValue);
    static ProgramElement createProcedure(const std::string& procName);
    static ProgramElement createVariable(const std::string& varName);
    static ProgramElement createConstant(const std::string& value);  // use string to avoid overflow

    std::string toString(EntityAttributeType entityAttributeType = EntityAttributeType::STMT) const;

    bool operator<(const ProgramElement& r) const;
    bool operator==(const ProgramElement& r) const;
};

#endif //SPA_TEAM35_CODE35_SRC_SPA_SRC_PKB_PROGRAMELEMENT_H_
