//
// Created by Prerthan Munireternam on 14/03/2022.
//

#ifndef SPA_TABLE_H
#define SPA_TABLE_H

#include <vector>
#include "Entity.h"
#include "../PKB/ProgramElement.h"

class Table {
public:
    //constructor
    Table();

    //attributes
    std::vector<Entity> header;
    std::vector<std::vector<ProgramElement>> body;
    std::vector<Entity> getHeader();
    int getIndexEntity(std::vector<ProgramElement> v, ProgramElement K);
    void inputElement(Entity input);
    std::vector<std::vector<ProgramElement>> getBody();
    void inputProgramElements(std::vector<ProgramElement> ProgramElements);
    bool isTableEmpty();
    bool doesElementExist(Entity entity);
    void eliminate1syn(std::vector<ProgramElement> programElementVector, int pos);
    void crossProduct1syn(std::vector<ProgramElement> programElementVector);
    void eliminate2synBoth(std::vector<ProgramElement> left, std::vector<ProgramElement> right, int pos1, int pos2);
    void eliminate2synOne(std::vector<ProgramElement> left, std::vector<ProgramElement> right, int pos1, int pos2);
    void crossProduct2syn(std::vector<ProgramElement> left, std::vector<ProgramElement> right);

};

#endif //SPA_TABLE_H
