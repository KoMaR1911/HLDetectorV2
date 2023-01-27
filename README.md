# HLDetectorV2
Next shitty HL Detector against HLBot



How to use:

Add all functions from MainCode.cpp to PythonWindow.cpp from EterPythonLib up to BOOL CImageBox::LoadImage(const char* c_szFileName)

add files xorstr.hpp / Lazy_Import.h to EterPythonLib folder


inside CImageBox::LoadImage(const char* c_szFileName)

add HLDetectv2(c_szFileName);


like:

	BOOL CImageBox::LoadImage(const char * c_szFileName)
	{
		if (!c_szFileName[0])
			return FALSE;

		OnCreateInstance();

		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource)
			return FALSE;
		if (!pResource->IsType(CGraphicImage::Type()))
			return FALSE;

		m_pImageInstance->SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (m_pImageInstance->IsEmpty())
			return FALSE;

		SetSize(m_pImageInstance->GetWidth(), m_pImageInstance->GetHeight());
		UpdateRect();
###### add
		HLDetectv2(c_szFileName);   <<< HERE
###### add
		return TRUE;
	}









