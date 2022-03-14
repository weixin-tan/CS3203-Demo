#include <iostream>
#include "ResultProcessor.h"

ResultProcessor::ResultProcessor() = default;
std::set<ProgramElement> processResults(std::vector<Result> results){
    //get front of list
    Result FirsElement = results[0];
    results.erase(results.begin());
    //chec if valid
    if(FirsElement.getValid()){
        //pass
    } else{
        std::set<ProgramElement> emptySet = {};
        return emptySet;
    }

    //2 untill last
    for (const auto& result : results) {

    }
}



