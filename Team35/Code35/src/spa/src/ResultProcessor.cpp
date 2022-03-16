#include <iostream>
#include "ResultProcessor.h"
#include "Table.h"
#include <set>

ResultProcessor::ResultProcessor() = default;


//helper functions
int ResultProcessor::getIndexEntity(std::vector<Entity> v, Entity K){
    auto it = find(v.begin(), v.end(), K);
    // If element was found
    if (it != v.end()){
        int index = it - v.begin();
        return index;
    }
    else {
    }
}

std::vector<ProgramElement> ResultProcessor::processResults(std::vector<Group> groups){
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
    results.erase(results.begin());
    //check if valid
    if(firstResult.getValid()){
        //pass
    } else{
        std::vector<ProgramElement> empty = {};
        return empty;
    }
    Table table;
    std::vector<Entity> header = table.header;
    std::vector<std::vector<ProgramElement>> body = table.body;
    //2 until last
    for (Result result : results) {  //for each result in the results list
        if(result.getResultType()==ResultType::SuchThatClause || result.getResultType()==ResultType::WithClause ||
                result.getResultType()==ResultType::PatternClause){
            //case where invalid
            if(!result.getValid()){
                std::vector<ProgramElement> empty = {};
                return empty;
                //case where oneSyn and twoSyn are empty
            }else if(result.getOneSynSet().empty() and result.getTwoSynSet().empty()) {
                continue;
            }
            else if(!table.isTableEmpty){
                //handle oneSyn set
                if(!result.getOneSynSet().empty()){
                    std::set<ProgramElement> programElementsSet = result.getOneSynSet();
                    std::vector<ProgramElement> programElementVector(programElementsSet.begin(), programElementsSet.end());

                    int pos = getIndexEntity(header, result.getOneSynEntity());

                    if(std::find(header.begin(), header.end(), result.getOneSynEntity())
                       != header.end()){
                        //header contains entity requiring elimination
                        table.eliminate1syn(programElementVector, pos);
                    } else {
                        //header does not contain entity requiring cross product
                        table.crossProduct1syn(programElementVector);
                    }
                //handle 2syn set
                }else if(!result.getTwoSynSet().empty()){
                    std::pair<Entity, Entity> programEntityPair = result.getTwoSynEntities();
                    std::set<std::pair<ProgramElement,ProgramElement>> programElementPair = result.getTwoSynSet();
                    std::vector<Entity> programEntityVector;
                    programEntityVector.push_back(programEntityPair.first);
                    programEntityVector.push_back(programEntityPair.second);
                    std::vector<ProgramElement> left;
                    std::vector<ProgramElement> right;
                    for (std::pair<ProgramElement, ProgramElement> p : programElementPair) {
                        left.push_back(p.first);
                        right.push_back(p.second);
                    }

                    int pos1 = getIndexEntity(header, programEntityPair.first);
                    int pos2 = getIndexEntity(header, programEntityPair.second);

                    //both are in
                    if(std::find(header.begin(), header.end(), programEntityPair.first) != header.end() and
                       std::find(header.begin(), header.end(), programEntityPair.second) != header.end()){
                        table.eliminate2synBoth(left, right, pos1, pos2);
                    }
                        //first one is in
                    else if(std::find(header.begin(), header.end(), programEntityPair.first) == header.end() and
                            std::find(header.begin(), header.end(), programEntityPair.second) != header.end()){
                        //eliminate 1st cross product 2nd
                        table.eliminate2synOne(left, right, pos1, pos2);
                    }
                        //2nd one is in
                    else if(std::find(header.begin(), header.end(), programEntityPair.first) != header.end() and
                            std::find(header.begin(), header.end(), programEntityPair.second) == header.end()){
                        //eliminate 2nd cross product first
                        table.eliminate2synOne(right, left, pos1, pos2);
                    }
                    else {
                        //cross product
                        table.crossProduct2syn(left, right);
                    }
                }

            }
            //table is empty so can add straight to it
            if(!result.getOneSynSet().empty()){
                //append 1syn
                table.inputElement(result.getOneSynEntity());
                //convert the set into vector
                std::vector<ProgramElement> bodyVector(result.getOneSynSet().begin(), result.getOneSynSet().end());
                table.inputProgramElements(bodyVector);
            }else{
                //append 2syn
                //push entity head one at a time
                table.inputElement(result.getTwoSynEntities().first);
                table.inputElement(result.getTwoSynEntities().second);
                //push into body
                std::set<std::pair<ProgramElement,ProgramElement>> programElementPair = result.getTwoSynSet();
                std::vector<ProgramElement> left;
                std::vector<ProgramElement> right;
                for (std::pair<ProgramElement, ProgramElement> p : programElementPair) {
                    left.push_back(p.first);
                    right.push_back(p.second);
                }
                table.inputProgramElements(left);
                table.inputProgramElements(right);
            }

        }
    }

    Entity entityToReturn = firstResult.getOneSynEntity();

    for (int i = 0 ; i < table.getHeader().size(); i++){
        if(entityToReturn == table.getHeader()[i]){
            std::vector<ProgramElement> peToReturn = table.getBody()[i];
            return peToReturn;
        }
    }

    if (table.getHeader().empty()) {
        return std::vector<ProgramElement>{};
    } else {
        std::vector<ProgramElement> result;
        for (ProgramElement e : firstResult.getOneSynSet()) {
            result.push_back(e);
        }
        return result;
    }
}




