#include "Functions.h"
#include <stdlib.h>

namespace LibParser {

	std::string Functions::Valstr(std::string arg1, std::string arg2)
	{
		std::string str1 = "<data> <tvalue arg1=""" + (rand() % 10 + 1);
		std::string str2 = """ arg2=""" + (rand() % 10 + 1);
		std::string str3 = """> </data>";
		//std::string str1 = "<data> <tvalue arg1="""" arg2=""""> </data>";
		//rand() % 10 + 1

		return str1 + str2 + str3;
	}

	//std::string Functions::Valstr()
	//{
	//	srand(time(NULL));
	//	int val1 = (rand() % 10 + 1);
	//	int val2 = (rand() % 10 + 1);

	//	std::string str1 = "<data> <block> <tvalue arg1=""" + to_string(val1);
	//	std::string str2 = """ arg2=""" + to_string(val2);
	//	std::string str3 = "> </block> </data>";
	//	//std::string str1 = "<data> <tvalue arg1="""" arg2=""""> </data>";
	//	//rand() % 10 + 1
	//	std::string res = str1 + str2 + str3;

	//	return res;
	//}

	std::string Functions::Valstr()
	{
		std::string str1 = "<data> <block> <tvalue arg1='" +
			to_string(rand() % 10 + 1) +
			"' arg2='" + to_string(rand() % 10 + 1) + "'" +
			"</block> </data>";
		//rand() % 10 + 1

		std::string res = str1;

		return res;
	}

}