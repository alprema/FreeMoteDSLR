#include "stdafx.h"
#include "IsoSpeedMapping.h"

IsoSpeedMapping::IsoSpeedMapping(void)
{
	(*this)[0x0] = new CString("Auto");
	(*this)[0x28] = new CString("ISO 6");
	(*this)[0x30] = new CString("ISO 12");
	(*this)[0x38] = new CString("ISO 25");
	(*this)[0x40] = new CString("ISO 50");
	(*this)[0x48] = new CString("ISO 100");
	(*this)[0x4b] = new CString("ISO 125");
	(*this)[0x4d] = new CString("ISO 160");
	(*this)[0x50] = new CString("ISO 200");
	(*this)[0x53] = new CString("ISO 250");
	(*this)[0x55] = new CString("ISO 320");
	(*this)[0x58] = new CString("ISO 400");
	(*this)[0x5b] = new CString("ISO 500");
	(*this)[0x5d] = new CString("ISO 640");
	(*this)[0x60] = new CString("ISO 800");
	(*this)[0x63] = new CString("ISO 1000");
	(*this)[0x65] = new CString("ISO 1250");
	(*this)[0x68] = new CString("ISO 1600");
	(*this)[0x70] = new CString("ISO 3200");
	(*this)[0x78] = new CString("ISO 6400");
	(*this)[0x80] = new CString("ISO 12800");
	(*this)[0x88] = new CString("ISO 25600");
	(*this)[0x90] = new CString("ISO 512000");
	(*this)[0x98] = new CString("ISO 102400");
	(*this)[0xffffffff] = new CString("INVALID");
}