//
// Created by Prerthan Munireternam on 14/03/2022.
//

#include "Table.h"
#include "Entity.h"
#include "ProgramElement.h"

Table::Table() {
    Table::header = {};
    Table::body = {};
}
 //methods for header
 std::vector<Entity> getHeader(){
    return Table::header;
}
void inputElement(Entity input,std::vector<Entity> header){
    header.push_back(input);
}
void inputElementPair(std::pair<Entity, Entity> input,std::vector<Entity> header){
    //check with viv
    header.push_back(input.first);
    header.push_back(input.second);
}
//methods for body
std::vector<std::vector<ProgramElement>> getBody(){
    return Table::body;
}
void inputProgramElements(std::vector<ProgramElement> ProgramElements,std::vector<std::vector<ProgramElement>> body){
    body.push_back(ProgramElements);
}

void inputProgramElementsPair(std::vector<ProgramElement> ProgramElements,std::vector<std::vector<ProgramElement>> body){
    body.push_back(ProgramElements);
}
