#include "Result.h"

Result::Result() = default;

Result::Result(std::set <ProgramElement> programElements) {
    Result::programElements = programElements;
}

void Result::insertProgramElement(ProgramElement e) {
    programElements.insert(e);
}

std::set<ProgramElement> Result::getProgramElements() {
    return programElements;
}
