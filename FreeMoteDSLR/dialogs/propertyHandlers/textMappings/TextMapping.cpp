#include "stdafx.h"
#include "TextMapping.h"


TextMapping::~TextMapping(void)
{
	for (auto ite = begin(); ite != end(); ++ite)
		delete(ite->second);
}
