#pragma once
#include "afxwin.h"
#include "stdafx.h"

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#define		MAX_EMPRESAS		20
#define		MAX_TIPO_ELEMENTOS	20

#define		MAX_VABLES			50
#define		MAX_ROW_GRID		50

#define		ID_EMPRESAS			0
#define		ID_TIPO_ELEMENTOS	1

#define		CAMPO_VACIO			"<introduce valor>"

#define		COLUMNA_VISIBLE		100
#define		COLUMNA_VISIBLE_1	101
#define		COLUMNA_VISIBLE_2	102


class CGlobalData {

	DECLARE_DYNAMIC (CGlobalData) 

public:

	CGlobalData ();
	~CGlobalData ();

	CString m_RDEmpresas[MAX_EMPRESAS];
	CString m_RDTipoElementos[MAX_TIPO_ELEMENTOS];

	// Contiene los datos de conexion de la base de datos
	CString m_DataBaseConnection ;
	
	CString m_dbuser;
	CString m_dbname;
	CString m_dbpwd;
	CString m_dbserver;

	BOOL m_DBAbierta ;


	CDatabase m_dnInv; // Base de Datos

	BOOL DBOpen(  ) ;
	BOOL DBClose (  ) ;
	BOOL DBOpen_New(  ) ;
	BOOL DBClose_New (  ) ;
	BOOL DBGetConfiguration(  ) ;

	BOOL HasBeenModifyed ;

	int GetIDCodigo (CString sCodigo, int tipo);

	BOOL InsertLog( CString szOperacion , CString szElement, CString szOldData, CString szNewData );

};