#pragma once

// #include <string>

// #include <windows.h>
// #include <commdlg.h>

// std::string abc()
// {
// 	OPENFILENAME ofn;

// 	char szFile[512];

// 	ofn.lStructSize = sizeof(ofn);
// 	ofn.hwndOwner = NULL;
// 	ofn.lpstrFile = szFile;
// 	ofn.lpstrFile[0] = '\0';
// 	ofn.nMaxFile = sizeof(szFile);
// 	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
// 	ofn.nFilterIndex = 1;
// 	ofn.lpstrFileTitle = NULL;
// 	ofn.nMaxFileTitle = 0;
// 	ofn.lpstrInitialDir = NULL;
// 	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

// 	GetOpenFileName(&ofn);

// 	MessageBox ( NULL , ofn.lpstrFile , "File Name" , MB_OK);
// 	return std::string(szFile);
// }