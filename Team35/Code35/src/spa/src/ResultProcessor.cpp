#include <iostream>
#include "ResultProcessor.h"
#include "Table.h"
#include <set>

ResultProcessor::ResultProcessor() = default;
std::set<ProgramElement> processResults(std::vector<Result> results){
    //get front of list
    Result FirsElement = results[0];
    results.erase(results.begin());
    //check if valid
    if(FirsElement.getValid()){
        //pass
    } else{
        std::set<ProgramElement> emptySet = {};
        return emptySet;
    }
    Table table;
    std::vector<Entity> header = table.header;
    std::vector<std::vector<ProgramElement>> body = table.body;
    //2 until last
    for (Result result : results) {  //for each result in the results list
        //convert the programElement set into a programElement vector
        //get the set
        std::set<ProgramElement> programElementsSet = result.getOneSynSet();
        //convert set to vector
        std::vector<ProgramElement> programElementVector(programElementsSet.begin(), programElementsSet.end());
        //handle such that clause
        if(result.getResultType()==ResultType::SuchThatClause){
            //case where invalid
            if(!result.getValid()){
                std::set<ProgramElement> emptySet = {};
                return emptySet;
            //case where oneSyn and twoSyn are empty
            }else if(result.getOneSynSet().empty() and result.getTwoSynSet().empty()){
                continue;
            //case where we handle oneSyn
            } else if(!result.getOneSynSet().empty()){
                //check if entity is in header of table
                if(std::find(header.begin(), header.end(), result.getOneSynEntity())
                    != header.end()){
                    //header contains entity requiring cross product
                } else {
                    //header does not contain entity
                    Entity entity = result.getOneSynEntity();
                    table.inputElement(entity,table.getHeader());
                }
                //do similar thing for body of table
                if(std::find(body.begin(), body.end(), programElementVector)
                   != body.end()){
                    //body contains program entities requiring cross product
                } else{
                    //body does not contain programElement
                    table.inputProgramElements(programElementVector,table.getBody());
                }
            //case where we handle twoSyn
            }else if(!result.getTwoSynSet().empty()){
                //check if entity is in header of table
                if(std::find(header.begin(), header.end(), result.getTwoSynEntities())
                    != header.end()){
                    //header contains entity requiring cross product
                } else {
                    //header does not contain entity
                    std::pair<Entity, Entity> entity = result.getTwoSynEntities();
                    table.inputElementPair(entity,table.getHeader());
                }
                //do similar thing for body of table
                

            } else{
                //continue
            }


        }



    }
}



