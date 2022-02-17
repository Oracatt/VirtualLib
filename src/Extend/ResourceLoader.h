#pragma once
#include "ManagedStream.h"
#include "SmxFile.h"
#include <vector>
#include <io.h>

using namespace std;

struct RCInfo
{
	bool isPackaged;

	bool exist;

	bool isZipped;

	LONG64 nbByteUnzip;

	LONG64 nbByteZipped;

	LPSRCINFO source;

	LPSMXFILE package;

	string fileName;
};

void MountPackageFile(LPCSTR filename);

void UnmountPackageFile(LPCSTR filename);
 
RCInfo GetResourceInfo(LPCSTR filename);

void LoadResourceByInfo(const RCInfo& info, BYTE* pOut, void(*progressReport)(float percent) = 0);

void LoadResourceFromFile(LPCSTR filename, BYTE* pOut, RCInfo* pInfoOut, void(*progressReport)(float percent) = 0);

void LoadResourceFromFile(LPCSTR filename, vector<BYTE>& pOut, RCInfo* pInfoOut, void(*progressReport)(float percent) = 0);