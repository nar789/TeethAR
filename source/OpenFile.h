#pragma once
#include<Windows.h>
#include<string>

using namespace std;

string OpenFileDialogA(const char *filter = "All files(*.*)\0*.*\0")
{
	string ret;
	OPENFILENAMEA ofn;
	char lpstrFilePath[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = lpstrFilePath;
	ofn.nMaxFile = 256;

	if (GetOpenFileNameA(&ofn)) {
		ret = lpstrFilePath;
		return ret;
	}
	return ret;
}