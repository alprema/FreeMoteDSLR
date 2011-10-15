#include "stdafx.h"
#include "ApertureMapping.h"


ApertureMapping::ApertureMapping(void)
{
	(*this)[0x08] = new CString("1");
	(*this)[0x0B] = new CString("1.1");
	(*this)[0x0C] = new CString("1.2");
	(*this)[0x0D] = new CString("1.2 (1/3)");
	(*this)[0x10] = new CString("1.4");
	(*this)[0x13] = new CString("1.6");
	(*this)[0x14] = new CString("1.8");
	(*this)[0x15] = new CString("1.8 (1/3)");
	(*this)[0x18] = new CString("2");
	(*this)[0x1B] = new CString("2.2");
	(*this)[0x1C] = new CString("2.5");
	(*this)[0x1D] = new CString("2.5 (1/3)");
	(*this)[0x20] = new CString("2.8");
	(*this)[0x23] = new CString("3.2");
	(*this)[0x24] = new CString("3.5");
	(*this)[0x25] = new CString("3.5 (1/3)");
	(*this)[0x28] = new CString("4");
	(*this)[0x2B] = new CString("4.5");
	(*this)[0x2C] = new CString("4.5");
	(*this)[0x2D] = new CString("5.0");
	(*this)[0x30] = new CString("5.6");
	(*this)[0x33] = new CString("6.3");
	(*this)[0x34] = new CString("6.7");
	(*this)[0x35] = new CString("7.1");
	(*this)[0x38] = new CString("8");
	(*this)[0x3B] = new CString("9");
	(*this)[0x3C] = new CString("9.5");
	(*this)[0x3D] = new CString("10");
	(*this)[0x40] = new CString("11");
	(*this)[0x43] = new CString("13 (1/3)");
	(*this)[0x44] = new CString("13");
	(*this)[0x45] = new CString("14");
	(*this)[0x48] = new CString("16");
	(*this)[0x4B] = new CString("18");
	(*this)[0x4C] = new CString("19");
	(*this)[0x4D] = new CString("20");
	(*this)[0x50] = new CString("22");
	(*this)[0x53] = new CString("25");
	(*this)[0x54] = new CString("27");
	(*this)[0x55] = new CString("29");
	(*this)[0x58] = new CString("32");
	(*this)[0x5B] = new CString("36");
	(*this)[0x5C] = new CString("38");
	(*this)[0x5D] = new CString("40");
	(*this)[0x60] = new CString("45");
	(*this)[0x63] = new CString("51");
	(*this)[0x64] = new CString("54");
	(*this)[0x65] = new CString("57");
	(*this)[0x68] = new CString("64");
	(*this)[0x6B] = new CString("72");
	(*this)[0x6C] = new CString("76");
	(*this)[0x6D] = new CString("80");
	(*this)[0x70] = new CString("91");
	(*this)[0xffffffff] = new CString("INVALID");
}