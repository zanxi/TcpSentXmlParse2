#pragma once
#include "stdafx.h"

namespace LibParser {

	class XmlParser
	{
	public: std::string Read(char* filename);
		    std::string Read(std::string xmltext);

	};
}

