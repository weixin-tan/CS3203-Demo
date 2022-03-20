#include "ResultProcessor.h"
#include "Table.h"
#include <set>

ResultProcessor::ResultProcessor() = default;

std::vector<ProgramElement> ResultProcessor::processResults(std::vector<Group> groups) {
    std::vector<Result> results;

    Group group1 = groups[0];
    results.push_back(group1.getGroup()[0]);

    if (groups.size() != 1) {
        Group group2 = groups[1];
        for (Result r : groups[1].getGroup()) {
            results.push_back(r);
        }
    }

    //get front of list
    Result firstResult = results[0];

    if (!firstResult.getValid()) {
        return std::vector<ProgramElement>{};
    }

    if (results.size() == 1) {
        std::set<ProgramElement> result = firstResult.getOneSynSet();
        return setToVector(result);
    }

    results.erase(results.begin());

    Table table;

    for (Result result : results) {  //for each result in the results list
        if (result.getResultType() == ResultType::NULL_CLAUSE || !result.getValid()) {
            return std::vector<ProgramElement>{};
        }

        if (result.getOneSynSet().empty() && result.getTwoSynSet().empty()) {
            continue;
        }

        if (!result.getOneSynSet().empty()) {
            Entity oneSyn = result.getOneSynEntity();
            std::set<ProgramElement> programElementSet = result.getOneSynSet();
            std::vector<ProgramElement> programElementVector = setToVector(programElementSet);

            int pos = getIndexEntity(table.getHeader(), oneSyn);
            if (pos == -1) {
                table.crossProductOneSyn(oneSyn, programElementVector);
            } else {
                table.eliminateOneSyn(programElementVector, pos);
            }

        } else if (!result.getTwoSynSet().empty()) {
            std::pair<Entity, Entity> twoSyn = result.getTwoSynEntities();
            Entity left = twoSyn.first;
            Entity right = twoSyn.second;
            std::set<std::pair<ProgramElement, ProgramElement>> programElementPairSet = result.getTwoSynSet();
            std::vector<std::pair<ProgramElement, ProgramElement>> programElementPairVector = setPairToVectorPair(
                    programElementPairSet);

            int leftPos = getIndexEntity(table.getHeader(), left);
            int rightPos = getIndexEntity(table.getHeader(), right);

            if (leftPos == -1 && rightPos == -1) {
                table.crossProductTwoSyn(twoSyn, programElementPairVector);
            } else if (leftPos == -1 || rightPos == -1) {
                table.eliminateTwoSynOne(twoSyn, programElementPairVector, leftPos, rightPos);
            } else {
                table.eliminateTwoSynBoth(programElementPairVector, leftPos, rightPos);
            }
        }

        if (table.isTableEmpty()) {
            return std::vector<ProgramElement>{};
        }
    }

    Entity entityToReturn = firstResult.getOneSynEntity();

    int pos = getIndexEntity(table.getHeader(), entityToReturn);

    if (pos == -1) {
        return setToVector(firstResult.getOneSynSet());
    } else {
        return table.getBody()[pos];
    }

}

int ResultProcessor::getIndexEntity(std::vector<Entity> v, Entity e) {
    auto posIterator = find(v.begin(), v.end(), e);
    if (posIterator != v.end()) {
        int pos = posIterator - v.begin();
        return pos;
    } else {
        return -1;
    }
}

std::vector<ProgramElement> ResultProcessor::setToVector(std::set<ProgramElement> s) {
    std::vector<ProgramElement> results;
    for (ProgramElement e : s) {
        results.push_back(e);
    }
    return results;
}

std::vector<std::pair<ProgramElement, ProgramElement>> ResultProcessor::setPairToVectorPair(std::set<std::pair<ProgramElement, ProgramElement>> s) {
    std::vector<std::pair<ProgramElement, ProgramElement>> results;
    for (std::pair<ProgramElement, ProgramElement> p : s) {
        results.push_back(p);
    }
    return results;
}
