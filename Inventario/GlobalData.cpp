
#include "stdafx.h"
#include "Inventario.h"
#include "afxdialogex.h"
#include "DialogInvMain.h"
#include "globaldata.h"
#include "afxwin.h"

IMPLEMENT_DYNAMIC ( CGlobalData, CObject ) 


CGlobalData::CGlobalData() {

	HasBeenModifyed = FALSE;
	m_DBAbierta = FALSE; 
};

CGlobalData::~CGlobalData() {


};

int CGlobalData::GetIDCodigo(CString sCodigo, int iTipo ) {

	int i = 0 ;

	switch (iTipo ) {

	case ID_EMPRESAS : // Empresa
		for ( i = 1 ; i <= MAX_EMPRESAS ; i++ ) 
			if ( m_RDEmpresas[i] == sCodigo) return i;
		return 0;
		break ;
	case ID_TIPO_ELEMENTOS : // Tipo de elemento
		for ( i = 0 ; i <= MAX_TIPO_ELEMENTOS ; i++ ) 
			if ( m_RDTipoElementos[i] == sCodigo) return i;
		return 0;
		break ;
		
		break ; 
	default : return -1;
		break;

	}

	return -1;
};          
	  

BOOL CGlobalData::DBOpen_New(  ) 
{
	
	if (!m_dnInv.Open(NULL,false,false,m_DataBaseConnection)) {
		return FALSE; 
	}
	return TRUE;

}

BOOL CGlobalData::DBClose_New (  ) 
{

	if (m_DBAbierta == FALSE) return true ;

	m_dnInv.Close();
    m_DBAbierta = FALSE ;

	return TRUE;
	
}


BOOL CGlobalData::DBOpen(  ) 
{
	
	return true;

	if (m_DBAbierta == TRUE) return true ;

	if (!m_dnInv.Open(NULL,false,false,m_DataBaseConnection)) {
		return FALSE; 
	}
	m_DBAbierta = TRUE ;
	return TRUE;
	
}

BOOL CGlobalData::DBClose (  ) 
{

   return true;

	if (m_DBAbierta == FALSE) return true ;

	m_dnInv.Close();
    m_DBAbierta = FALSE ;

	return TRUE;
	
}

BOOL CGlobalData::DBGetConfiguration(  ) 
{
	CString CItem;
	// Base de Datos

	// CItem = _T("ODBC;DRIVER={MySQL ODBC 5.1 Driver};Server= 95.215.61.36;Port=3306;Database=") + m_GlobalData.m_dbname + _T(";User=") + m_GlobalData.m_dbuser + _T("; Password=pqrpqr33;");
	// CItem = _T("ODBC;DRIVER={MySQL ODBC 5.1 Driver};Server= 69.195.140.154;Port=3306;Database=") + m_GlobalData.m_dbname + _T(";User=") + m_GlobalData.m_dbuser + _T("; Password=pqrpqr33;");

	// (CItem = _T("ODBC;DRIVER={MySQL ODBC 5.1 Driver};Server= 69.195.140.154;Port=3306;Database=") + m_GlobalData.m_dbname + _T(";User=") + m_GlobalData.m_dbuser + _T("; Password=mxr00ranewnew;");

	// (CItem = _T("ODBC=ODBC_Inventario") );
		
	CItem = _T("Driver={SQL Server}; Server=") +  m_GlobalData.m_dbserver + _T(";Database=") + m_GlobalData.m_dbname + _T(";Uid=") + m_GlobalData.m_dbuser + _T(";Pwd=") + m_GlobalData.m_dbpwd + _T(";") ;
	

	m_DataBaseConnection.Format(CItem);

	return TRUE;
}


BOOL CGlobalData::InsertLog( CString szOperacion , CString szElement, CString szOldData, CString szNewData  ) {
	
	CString szTimeStamp =_T(".") ;

	CString SqlString;
	
	TCHAR szComputerName[256];
    DWORD cchComputerName = 256;

   GetComputerName(szComputerName, &cchComputerName);

	CTime t = CTime::GetCurrentTime();   
	szTimeStamp = t.Format(_T("%Y-%m-%d  %H:%M:%S"));

	SqlString.Format(
		_T("INSERT INTO INV_LOG ( OPERACION, ID_ELEMENTO, OLD_DATA, NEW_DATA, FECHA) VALUES ('%s','%s','%s','%s' , GetDate() )"),
		  szOperacion + _T("  (") +CString(szComputerName)+_T(")") , szElement , szOldData, szNewData) ; 

	m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;

	return TRUE;
}