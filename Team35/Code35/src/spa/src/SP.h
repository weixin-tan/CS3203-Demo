#ifndef SPA_SP_H
#define SPA_SP_H

#include "Tokeniser.h"
#include "ConcreteSyntaxWithValidation.h"
#include "Convertor.h"
#include <PKB/PkbSetter.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

class SP {

public:
    Tokeniser tokeniser;
    ConcreteSyntaxWithValidation concrete;
    Convertor convertor;
    
    SP(PkbSetter* pkb_setter);

    void Parse(std::string filename);
    
    Program program; 
    std::queue<Token> tokenQueue;
};

#endif

