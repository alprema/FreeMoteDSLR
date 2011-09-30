#include "stdafx.h"
#include "MultiValuePropertyHandler.h"
#include <atlmisc.h>
#include <vector>

MultiValuePropertyHandler::MultiValuePropertyHandler(CComboBox valuesCombo, TextMapping* textMapping)
	: values_combo_(valuesCombo), text_mapping_(textMapping)
{
}

MultiValuePropertyHandler::~MultiValuePropertyHandler()
{
	// TODO: Check memory release (especially for all the strings)
	delete(text_mapping_);
}

void MultiValuePropertyHandler::SetPossibleValues(LPARAM value)
{
	std::vector<int>* possibleValues = (std::vector<int>*)value;
	values_combo_.ResetContent();
	property_values_.clear();
	int i = 0;
	for (auto ite = possibleValues->begin(); ite != possibleValues->end(); ++ite, ++i)
	{
		values_combo_.AddString(*((*text_mapping_)[*ite]));
		values_combo_.SetItemData(i, *ite);
		property_values_[*ite] = i;
	}
	values_combo_.EnableWindow(i != 0);
	delete(possibleValues);
}

void MultiValuePropertyHandler::SetValue(LPARAM value)
{
	int* selectedValue = (int*)value;
	// TODO: Isn't there a prettier way to know if some value is in the hash?
	if (0 != property_values_.count(*selectedValue))
		values_combo_.SetCurSel(property_values_[*selectedValue]);
	delete(selectedValue);
}