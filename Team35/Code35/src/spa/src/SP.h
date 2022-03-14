#pragma once

#include "Tokeniser.h"
#include "ConcreteSyntaxWithValidation.h"
#include "Convertor.h"
#include <PKB/PkbSetter.h>
#include <iostream>
#include <fstream>
#include <stdexcept>

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

