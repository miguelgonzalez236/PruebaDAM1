
// Inventario.h: archivo de encabezado principal para la aplicaci�n PROJECT_NAME
//

#pragma once
#include "globaldata.h"
#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"		// S�mbolos principales

// CInventarioApp:
// Consulte la secci�n Inventario.cpp para obtener informaci�n sobre la implementaci�n de esta clase
//
class CInventarioApp : public CWinApp
{
public:
	CInventarioApp();
	
// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementaci�n

	DECLARE_MESSAGE_MAP()
};

extern CInventarioApp theApp;