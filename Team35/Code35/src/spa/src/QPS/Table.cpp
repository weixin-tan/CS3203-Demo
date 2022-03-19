//
// Created by Prerthan Munireternam on 14/03/2022.
//

#include "Table.h"
#include "Entity.h"
#include "../PKB/ProgramElement.h"

Table::Table() {
    Table::header = {};
    Table::body = {};
}

int Table::getIndexElement(std::vector<ProgramElement> v, ProgramElement e) {
    auto posIterator = find(v.begin(), v.end(), e);
    if (posIterator != v.end()) {
        int pos = posIterator - v.begin();
        return pos;
    } else {
        return -1;
    }
}

//methods for header
std::vector<Entity> Table::getHeader() {
    return Table::header;
}

void Table::inputElement(Entity input) {
    Table::header.push_back(input);
}

//methods for body
std::vector<std::vector<ProgramElement>> Table::getBody() {
    return Table::body;
}
void Table::inputProgramElements(std::vector<ProgramElement> ProgramElements) {
    Table::body.push_back(ProgramElements);
}

bool Table::isTableEmpty() {
    return Table::header.empty();
}

void Table::makeTableEmpty() {
    Table::header = std::vector<Entity>{};
}

bool Table::doesElementExist(Entity entity) {
    std::vector<Entity> header = Table::header;
    if (std::find(header.begin(), header.end(), entity) != header.end()) {
        return true;
    } else {
        return false;
    }
}

void Table::eliminateOneSyn(std::vector<ProgramElement> programElementVector, int pos) {
    //preprocessing
    std::vector<ProgramElement> toBeChecked = Table::body[pos];
    std::vector<int> toBeDeleted;

    //checking
    for (int i = toBeChecked.size() - 1; i >= 0; i--) {
        ProgramElement check = toBeChecked[i];
        int checkPos = getIndexElement(programElementVector, check);
        if (checkPos == -1) {
            toBeDeleted.push_back(i);
        }
    }

    for (std::vector<ProgramElement> vecP : Table::body) {
        for (int index : toBeDeleted) {
            //delete
            vecP.erase(vecP.begin() + index);
        }
    }

    //Make table empty if there are no elements left
    if (body[0].empty()) {
        makeTableEmpty();
    }
}

void Table::crossProductOneSyn(Entity oneSyn, std::vector<ProgramElement> programElementVector) {
    if (isTableEmpty()) {
        header.push_back(oneSyn);
        body.push_back(programElementVector);
        return ;
    }

    int originalSize = body[0].size();
    int numberOfRepetition = programElementVector.size();
    for (std::vector<ProgramElement> p : body) {
        for (int i = 0; i < numberOfRepetition; i++) {
            for (int j = 0; j < originalSize; j++) {
                p.push_back(p[j]);
            }
        }
    }

    std::vector<ProgramElement> temp;
    for (ProgramElement p : programElementVector) {
        for (int i = 0; i < originalSize; i++) {
            temp.push_back(p);
        }
    }

    header.push_back(oneSyn);
    body.push_back(temp);
}

void Table::eliminateTwoSynBoth(std::vector<std::pair<ProgramElement, ProgramElement>> v, int leftPos, int rightPos) {
    std::vector<ProgramElement> temp1 = Table::body[leftPos];
    std::vector<ProgramElement> temp2 = Table::body[rightPos];
    std::vector<int> toBeDeleted;

    for (int i = temp1.size() - 1; i >= 0; i--) {
        std::pair<ProgramElement, ProgramElement> pairToCheck(temp1[i], temp2[i]);
        int checkPos = getPairIndex(v, pairToCheck);
        if (checkPos == -1) {
            toBeDeleted.push_back(i);
        }
    }

    for (std::vector<ProgramElement> v : body) {
        for (int index : toBeDeleted) {
            v.erase(v.begin() + index);
        }
    }

    if (body[0].empty()) {
        makeTableEmpty();
    }
}

void Table::eliminateTwoSynOne(std::pair<Entity, Entity> pair, std::vector<std::pair<ProgramElement, ProgramElement>> v, int leftPos, int rightPos) {
    std::vector<std::vector<ProgramElement>> vectors = extractPairs(v);

    std::vector<ProgramElement> checkVector;
    std::vector<ProgramElement> resultVector;
    std::vector<ProgramElement> otherVector;
    if (leftPos == -1) {
        checkVector = body[rightPos];
        resultVector = vectors[1];
        otherVector = vectors[0];
        header.push_back(pair.first);
    } else {
        checkVector = body[leftPos];
        resultVector = vectors[0];
        otherVector = vectors[1];
        header.push_back(pair.second);
    }

    std::vector<ProgramElement> vectorToBeInserted;
    for (int i = 0; i < checkVector.size(); i++) {
        for (int j = 0; j < resultVector.size(); j++) {
            if (checkVector[i] == resultVector[j]) {
                vectorToBeInserted.push_back(otherVector[j]);
                for (std::vector<ProgramElement> v : body) {
                    v.push_back(v[i]);
                }
            }
        }
    }

    for (std::vector<ProgramElement> p: body) {
        for (int i = checkVector.size() - 1; i >= 0; i--) {
            p.erase(p.begin() + i);
        }
    }

    body.push_back(vectorToBeInserted);
}

void Table::crossProductTwoSyn(std::pair<Entity, Entity> pair, std::vector<std::pair<ProgramElement, ProgramElement>> v) {
    std::vector<std::vector<ProgramElement>> pairVectors = extractPairs(v);
    std::vector<ProgramElement> left = pairVectors[0];
    std::vector<ProgramElement> right = pairVectors[1];

    int originalSize = body[0].size();
    int numberOfRepetition = left.size();
    for (std::vector<ProgramElement> p : body) {
        for (int i = 0; i < numberOfRepetition; i++) {
            for (int j = 0; j < originalSize; j++) {
                p.push_back(p[j]);
            }
        }
    }

    std::vector<ProgramElement> tempLeft;
    for (ProgramElement p : left) {
        for (int i = 0; i < originalSize; i++) {
            tempLeft.push_back(p);
        }
    }

    std::vector<ProgramElement> tempRight;
    for (ProgramElement p : right) {
        for (int i = 0; i < originalSize; i++) {
            tempRight.push_back(p);
        }
    }

    header.push_back(pair.first);
    header.push_back(pair.second);
    body.push_back(tempLeft);
    body.push_back(tempRight);
}

std::vector<std::vector<ProgramElement>> Table::extractPairs(std::vector<std::pair<ProgramElement, ProgramElement>> v) {
    std::vector<ProgramElement> left;
    std::vector<ProgramElement> right;
    for (std::pair<ProgramElement, ProgramElement> p : v) {
        left.push_back(p.first);
        right.push_back(p.second);
    }

    std::vector<std::vector<ProgramElement>> results;
    results.push_back(left);
    results.push_back(right);
    return results;
}

int Table::getPairIndex(std::vector<std::pair<ProgramElement, ProgramElement>> v, std::pair<ProgramElement, ProgramElement> e) {
    auto posIterator = find(v.begin(), v.end(), e);
    if (posIterator != v.end()) {
        int pos = posIterator - v.begin();
        return pos;
    } else {
        return -1;
    }
}