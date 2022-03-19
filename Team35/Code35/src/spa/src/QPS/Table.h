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
    int getIndexElement(std::vector<ProgramElement> v, ProgramElement e);
    void inputElement(Entity input);
    std::vector<std::vector<ProgramElement>> getBody();
    void inputProgramElements(std::vector<ProgramElement> ProgramElements);
    bool isTableEmpty();
    bool doesElementExist(Entity entity);
    void eliminateOneSyn(std::vector<ProgramElement> programElementVector, int pos);
    void crossProductOneSyn(Entity oneSyn, std::vector<ProgramElement> programElementVector);
    void eliminateTwoSynBoth(std::vector<std::pair<ProgramElement, ProgramElement>> v, int pos1, int pos2);
    void eliminateTwoSynOne(std::pair<Entity, Entity> pair, std::vector<std::pair<ProgramElement, ProgramElement>> v, int pos1, int pos2);
    void crossProductTwoSyn(std::pair<Entity, Entity> pair, std::vector<std::pair<ProgramElement, ProgramElement>> v);

    void makeTableEmpty();

    std::vector<std::vector<ProgramElement>> extractPairs(std::vector<std::pair<ProgramElement, ProgramElement>> v);

    int
    getPairIndex(std::vector<std::pair<ProgramElement, ProgramElement>> v, std::pair<ProgramElement, ProgramElement> e);
};

#endif //SPA_TABLE_H
