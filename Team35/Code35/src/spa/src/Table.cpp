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

int Table::getIndexEntity(std::vector<ProgramElement> v, ProgramElement K){
    auto it = find(v.begin(), v.end(), K);
    // If element was found
    if (it != v.end()){
        int index = it - v.begin();
        return index;
    }
    else {
        return -1;
    }
}

//methods for header
std::vector<Entity> Table::getHeader(){
    return Table::header;
}

void Table::inputElement(Entity input){
    Table::header.push_back(input);
}

//methods for body
std::vector<std::vector<ProgramElement>> Table::getBody(){
    return Table::body;
}
void Table::inputProgramElements(std::vector<ProgramElement> ProgramElements){
    Table::body.push_back(ProgramElements);
}

bool Table::isTableEmpty(){
    return Table::header.empty();
}

bool Table::doesElementExist(Entity entity){
    std::vector<Entity> header = Table::header;
    if(std::find(header.begin(), header.end(), entity) != header.end()){
        return true;
    } else{
        return false;
    }
}

void Table::eliminate1syn(std::vector<ProgramElement> programElementVector, int pos){
    //preprocessing
    std::vector<ProgramElement> toBeChecked = Table::body[pos];
    std::vector<int> toBeDeleted;
    std::vector<ProgramElement> cache;
    //checking
    for(ProgramElement p : toBeChecked){
        //if p is in cahce continue
        if(std::find(cache.begin(), cache.end(), p) != cache.end()){
            continue;
        } else{
            for(ProgramElement q : programElementVector){
                if(p==q){
                    //put the position into tobeDeleted
                    int positionToBeDeleted = getIndexEntity(programElementVector, p);
                    toBeDeleted.push_back(positionToBeDeleted);
                    //put pe into cahce
                    cache.push_back(p);
                }
            }
        }
    }
    //delete those elements from the back
    std::reverse(toBeDeleted.begin(), toBeDeleted.end());
    for (std::vector<ProgramElement> vecP : Table::body){
        for(int index : toBeDeleted){
            //delete
            vecP.erase(vecP.begin() + index);
        }
    }
}

void Table::crossProduct1syn(std::vector<ProgramElement> programElementVector){
    int originalSize = Table::body[0].size();
    int numberOfRepetition = programElementVector.size();
    for (std::vector<ProgramElement> p : Table::body){
        for(int i = 0; i< numberOfRepetition; i++){
            for(int j = 0; j < originalSize; j++){
                p.push_back(p[j]);
            }
        }
    }

    std::vector<ProgramElement> temp;
    for (ProgramElement p : programElementVector){
        for(int i = 0; i < originalSize; i++){
            temp.push_back(p);
        }
    }
    Table::body.push_back(temp);
}

void Table::eliminate2synBoth(std::vector<ProgramElement> left, std::vector<ProgramElement> right, int pos1, int pos2){
    std::vector<ProgramElement> temp1 = Table::body[pos1];
    std::vector<ProgramElement> temp2 = Table::body[pos2];
    //checking
    for(int i = left.size()-1; i <= 0; i--){
        if(temp1[i] == left[i] && temp2[i] == right[i] ){
            continue;
        }else{
            for(std::vector<ProgramElement> p : Table::body){
                p.erase(p.begin()+i);
            }
        }
    }
}


//left is the one to be eliminated
void Table::eliminate2synOne(std::vector<ProgramElement> left, std::vector<ProgramElement> right, int pos1, int pos2){
    std::vector<ProgramElement> temp1 = Table::body[pos1];
    std::vector<ProgramElement> temp2 = Table::body[pos2];
    int originalSize = Table::body[0].size();
    std::vector<ProgramElement> tempRight;
    int originalSizeRight = right.size();
    //place elements
    for (int i = 0; i < originalSize; i++){
        for(int j = 0; j < left.size(); j++){
            if(temp2[i] == left[j]){
                tempRight.push_back(right[j]);
                for(std::vector<ProgramElement> p : Table::body){
                    p.push_back(p[i]);
                }
            }
        }
    }
    //trim table
    for (int i = 0; i < originalSize; i++){
        for (std::vector<ProgramElement> p : Table::body){
            p.erase((p.begin()+i));
        }
    }
    //add right to table
    Table::body.push_back(tempRight);
}

void Table::crossProduct2syn(std::vector<ProgramElement> left, std::vector<ProgramElement> right){
    int originalSize = Table::body[0].size();
    int numberOfRepetition = left.size();
    for (std::vector<ProgramElement> p : Table::body){
        for(int i = 0; i< numberOfRepetition; i++){
            for(int j = 0; j < originalSize; j++){
                p.push_back(p[j]);
            }
        }
    }
    std::vector<ProgramElement> tempLeft;
    for (ProgramElement p : left){
        for(int i = 0; i < originalSize; i++){
            tempLeft.push_back(p);
        }
    }
    Table::body.push_back(tempLeft);

    std::vector<ProgramElement> tempRight;
    for (ProgramElement p : right){
        for(int i = 0; i < originalSize; i++){
            tempRight.push_back(p);
        }
    }
    Table::body.push_back(tempRight);
}
