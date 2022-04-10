#ifndef SPA_HASHING_H
#define SPA_HASHING_H

#include <random>
#include <vector>
#include <algorithm>

#include "../PKB/ProgramElement.h"
#include "Entity.h"

class Hashing {
public:
    //constructor
    Entity entity; //entity to be hashed
    int totalEntitiesEstimate; //estimate how many entites would be present
    int totalProgramElementsEstimate; //estimate how many programelements would be present

    //methods
    int hashFunctionForEntities(Entity entity);
    int hashFunctionForProgramElements(ProgramElement programElement);
    int getTotalEntitiesEstimate();
    int getTotalProgramElementsEstimate();
    void setTotalEntitiesEstimate(int input);
    void setTotalProgramElementsEstimate(int input);
    int cantorPairingFunction(int a, int b);
};

#endif //SPA_HASHING_H
