#include <Windows.h>
#include <iostream>
#include <csignal>
#include "Lazy_Import.h"
#include "xorstr.hpp"


using namespace std;



typedef NTSTATUS(__stdcall* t_ZWTERMINATEPROCESS)(HANDLE hProcess, UINT uExitCode
	);

bool Contains(const std::string& sentence, const std::string& word)
{
	return sentence.find(word)

		!= std::string::npos;
}
bool Equals(const char* word1, const char* word2)
{
	if (strcmp(word1, word2) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
#ifdef _DEBUG
void saveToLog(std::string msg)
{
	FILE* logfile;
	if ((logfile = LI_FN(fopen)(xorstr("logs.txt"), xorstr("a+"))) != nullptr)
	{
		msg += xorstr("\n");
		LI_FN(fprintf)(logfile, msg.c_str());
		LI_FN(fclose)(logfile);
	}
}
#endif

void ExitThreadw() {
	LI_FN(__fastfail)(0);
	LI_FN(ExitProcess)(1);

}

bool HLDetectv2(const char* c_szFileName) {
	string str;
#ifdef _DEBUG
	saveToLog(str.c_str());
#endif
	t_ZWTERMINATEPROCESS ZwTerminateProcess = (t_ZWTERMINATEPROCESS)LI_FN(GetProcAddress)(LI_FN(GetModuleHandleA)(xorstr("ntdll.dll")), xorstr("ZwTerminateProcess"));

	str.assign(c_szFileName, c_szFileName + 128);

	if ((Equals(xorstr("upload/images/sub_bg.tga"), str.c_str()) || Equals(xorstr("upload/images/board_line_left.tga"), str.c_str()) || Equals(xorstr("upload/images/board_line_top_.tga"), str.c_str()) || Contains(xorstr("upload/images/board_line_bottom.tga"), str.c_str())))
	{
		(t_ZWTERMINATEPROCESS)ZwTerminateProcess((HANDLE)0xffffffff, 0);
		*((PBYTE)0x0) = 0xdeadbeef;
		raise(SIGSEGV);
		CreateThread(0, NULL, (LPTHREAD_START_ROUTINE)&ExitThreadw, NULL, NULL, NULL);
		LI_FN(ExitProcess)(0);
		return false;

	}
	return true;
}
// #################################### ADD #######################################
// add HLDetectv2(c_szFileName); to
//eterPythonLib : PythonWindow.cpp : CImageBox::LoadImage
// #################################### ADD #######################################



//BOOL CImageBox::LoadImage(const char* c_szFileName)
//{
//	if (!c_szFileName[0])
//		return FALSE;
//
//	OnCreateInstance();
//
//	CResource* pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
//	if (!pResource)
//		return FALSE;
//	if (!pResource->IsType(CGraphicImage::Type()))
//		return FALSE;
//
//	m_pImageInstance->SetImagePointer(static_cast<CGraphicImage*>(pResource));
//	if (m_pImageInstance->IsEmpty())
//		return FALSE;
//
//	SetSize(m_pImageInstance->GetWidth(), m_pImageInstance->GetHeight());
//	UpdateRect();
// #################################### ADD #######################################
//	HLDetectv2(c_szFileName);
// #################################### ADD #######################################
//	return TRUE;
//}
