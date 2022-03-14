//
// Created by Prerthan Munireternam on 14/03/2022.
//

#ifndef SPA_TABLE_H
#define SPA_TABLE_H


#include <vector>
#include "Entity.h"
#include "ProgramElement.h"


class Table {
public:
    //constructor
    Table();
    //attributes
    static std::vector<Entity> header;
    static std::vector<std::vector<ProgramElement>> body;
    std::vector<Entity> getHeader();
    void inputElement(Entity input,std::vector<Entity> header);
    void inputElementPair(std::pair<Entity, Entity> input,std::vector<Entity> header);
    std::vector<std::vector<ProgramElement>> getBody();
    void inputProgramElements(std::vector<ProgramElement> ProgramElements,std::vector<std::vector<ProgramElement>> body);
};


#endif //SPA_TABLE_H
