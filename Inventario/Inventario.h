
// Inventario.h: archivo de encabezado principal para la aplicación PROJECT_NAME
//

#pragma once
#include "globaldata.h"
#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"		// Símbolos principales

// CInventarioApp:
// Consulte la sección Inventario.cpp para obtener información sobre la implementación de esta clase
//
class CInventarioApp : public CWinApp
{
public:
	CInventarioApp();
	
// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementación

	DECLARE_MESSAGE_MAP()
};

extern CInventarioApp theApp;