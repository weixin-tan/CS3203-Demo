#include <stdexcept>
#include "ProgramElement.h"

const std::string ProgramElement::nullStringValue;

ProgramElement::ProgramElement(ElementType elementType,
                               int stmtNo,
                               const std::string& procName,
                               const std::string& varName,
                               const std::string& value)
        : elementType(elementType),
          stmtNo(stmtNo),
          procName(procName),
          varName(varName),
          value(value),
          tuple(elementType, stmtNo, procName, varName, value) {}

ProgramElement ProgramElement::createStatement(ElementType elementType, int stmtNo, const std::string& procOrVarName) {
    if (!isStatementType(elementType))
        throw std::invalid_argument("Wrong element type for statement");
    if (procOrVarName == ProgramElement::nullStringValue)
        return {elementType, stmtNo, ProgramElement::nullStringValue, ProgramElement::nullStringValue,
                ProgramElement::nullStringValue};
    if (elementType == ElementType::CALL)
        return {elementType, stmtNo, procOrVarName, ProgramElement::nullStringValue, ProgramElement::nullStringValue};
    if (elementType == ElementType::PRINT || elementType == ElementType::READ)
        return {elementType, stmtNo, ProgramElement::nullStringValue, procOrVarName, ProgramElement::nullStringValue};
    throw std::logic_error("No viable construction for statement");
}

ProgramElement ProgramElement::createProcedure(const std::string& procName) {
    if (procName.empty())
        throw std::invalid_argument("Empty procName string provided");
    return {ElementType::PROCEDURE, ProgramElement::nullIntegerValue, procName, ProgramElement::nullStringValue,
            ProgramElement::nullStringValue};
}

ProgramElement ProgramElement::createConstant(const std::string& value) {
    if (value.empty())
        throw std::invalid_argument("Empty value string provided");
    return {ElementType::CONSTANT, ProgramElement::nullIntegerValue, ProgramElement::nullStringValue,
            ProgramElement::nullStringValue, value};
}

ProgramElement ProgramElement::createVariable(const std::string& varName) {
    if (varName.empty())
        throw std::invalid_argument("Empty varName string provided");

    return {ElementType::VARIABLE, ProgramElement::nullIntegerValue, ProgramElement::nullStringValue, varName,
            ProgramElement::nullStringValue};
}

// element type takes priority, if statement select with entityAttributeType
std::string ProgramElement::toString(EntityAttributeType entityAttributeType) const {
    if (elementType == ElementType::PROCEDURE)
        return procName;
    if (elementType == ElementType::VARIABLE)
        return varName;
    if (elementType == ElementType::CONSTANT)
        return value;
    switch (entityAttributeType) {
        case EntityAttributeType::STMT:return std::to_string(stmtNo);
        case EntityAttributeType::VARNAME: {
            if (elementType != ElementType::PRINT && elementType != ElementType::READ)
                throw std::invalid_argument("Accessing variable for statements not guaranteed to have variable");
            return varName;
        }
        case EntityAttributeType::PROCNAME: {
            if (elementType != ElementType::CALL)
                throw std::invalid_argument("Accessing procName for statements not guaranteed to have procedure");
            return procName;
        }
        default:throw std::invalid_argument("Bad attribute selector for statement");
    }

}

size_t ProgramElement::getProgramElementHash() const {
    return (std::hash<std::string>()(procName) + std::hash<std::string>()(varName) + std::hash<std::string>()(value))
            ^ std::hash<int>()(stmtNo);
}

bool ProgramElement::operator<(const ProgramElement& r) const {
    return tuple < r.tuple;
}

bool ProgramElement::operator==(const ProgramElement& r) const {
    return tuple == r.tuple;
}


