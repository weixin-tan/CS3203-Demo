#pragma once

#include "Tokeniser.h"
#include "ConcreteSyntaxBasic.h"
#include "Convertor.h"
#include <PKB/PkbSetter.h>
#include <iostream>
#include <fstream>

class SP {

public:
	Tokeniser tokeniser;
	ConcreteSyntaxBasic concrete;
	Convertor convertor;
	
	SP::SP(PkbSetter* pkb_setter);

	void SP::Parse(std::string filename);
	
};

