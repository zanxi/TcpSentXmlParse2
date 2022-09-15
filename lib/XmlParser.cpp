#include "XmlParser.h"
#include "Functions.h"

namespace LibParser {

	std::string XmlParser::Read(std::string xmltext)
	{
		//std::string xml = Functions::Valstr();
		xml_document doc;
		//doc.load_string(static_cast<char_t>((char*)xml))
		doc.load_string(xmltext.c_str());
		xml_node tools = doc.child("data").child("block");
		std::string str = "block: ";
		for (xml_node_iterator it = tools.begin();
			it != tools.end();
			++it)
		{

			for (xml_attribute_iterator ait = it->attributes_begin();
				ait != it->attributes_end();
				++ait)
			{
				str = str + ait->name() + " = " + ait->value();
			}
		}

		return str;
	}
}