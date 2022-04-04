//
// Created by Prerthan Munireternam on 04/04/2022.
//

#ifndef SPA_HASHING_H
#define SPA_HASHING_H

#include "Entity.h"

class Hashing{
public:
    //constructor
    Entity entity; //entity to be hashed
    int totalEntitiesEstimate; //estimate how many entites would be present

    //methods
    int hashFunction(Entity entity);
    int getTotalEntitiesEstimate();
    void setTotalEntitiesEstimate(int input);
};

#endif //SPA_HASHING_H
