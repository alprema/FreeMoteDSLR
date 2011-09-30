#pragma once
#include <hash_map>
#include <atlmisc.h>

class TextMapping : public std::hash_map<int, CString*>
{
public:
	virtual ~TextMapping(void);
};

