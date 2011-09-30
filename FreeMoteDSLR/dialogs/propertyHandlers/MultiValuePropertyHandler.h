#pragma once
#include "PropertyHandler.h"
#include "textMappings\TextMapping.h"
#include <hash_map>

// Maps a list of possible values to a Combobox
class MultiValuePropertyHandler : public PropertyHandler
{
public:
	MultiValuePropertyHandler(CComboBox valuesCombo, TextMapping* textMapping);
	~MultiValuePropertyHandler();
	void SetPossibleValues(LPARAM value);
	void SetValue(LPARAM value);
private:
	CComboBox values_combo_;
	// Mapping from a property value to its index
	std::hash_map<int, int> property_values_;
	// Handles mappings between raw values and text
	TextMapping* text_mapping_;
};

