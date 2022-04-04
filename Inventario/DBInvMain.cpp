// DialogInvMain.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Inventario.h"
#include "afxdialogex.h"
#include "DialogInvMain.h"
#include "globaldata.h"

#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"

/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* DBGetConfiguration :
/* Cadena de inicio de base de datos 
/* ******************************************************************************* */

BOOL CDialogInvMain::DBGetConfiguration(  ) 
{
	
	return m_GlobalData.DBGetConfiguration();


}


/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* LoadInvTree :
/* Recupera los datos de la tabla de elementos 
/* ******************************************************************************* */

void CDialogInvMain::LoadInvTree() 
{
	CString SqlString,CEmpresa,CLevel,CType,CIdElemento,CTotalFilas,CProgress; // SQL
	CString sIdElemento,CValorVable,COrdenVable; // SQL
	HTREEITEM hRoot,hItem, hItem1;
	
	int iIconType =0 , iType = 0 , iProgress=0,iMaximo=0;

	WaitingCursor (TRUE) ;

	m_GlobalData.DBOpen ();
	/* INICIO : INICIALIZA EL CONTROL DE PROGESO */
	CRecordset recset1 ( &m_GlobalData.m_dnInv );	
	SqlString =  "SELECT COUNT(*) as TOTAL_FILAS FROM INV_EMPRESA";
	recset1.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;
	recset1.GetFieldValue(_T("TOTAL_FILAS"),	CTotalFilas);
	m_Maximo.SetWindowText(CTotalFilas);
	iMaximo = _wtoi ( CTotalFilas) ;
	m_spProgress.SetBarColor(RGB(255, 100, 25));
	m_spProgress.SetRange(0, iMaximo);
	/* FIN : INICIALIZA EL CONTROL DE PROGESO */


	CRecordset recset ( &m_GlobalData.m_dnInv );	
	SqlString =  "SELECT ID_EMPRESA,NIVEL,TYPE,ID_ELEMENTO FROM INV_EMPRESA ORDER BY ID_EMPRESA,TYPE,NIVEL,ID_ELEMENTO ASC";

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;
	
	CRecordset recset2 ( &m_GlobalData_2.m_dnInv );	

	while( !recset.IsEOF() )
	{
		// Copy each column into a variable
		recset.GetFieldValue(_T("ID_EMPRESA"),			CEmpresa);
		recset.GetFieldValue(_T("NIVEL"),				CLevel);
		recset.GetFieldValue(_T("TYPE"),				CType);
		recset.GetFieldValue(_T("ID_ELEMENTO"),			CIdElemento);

		iType = _wtoi(CType);
		iIconType = GetIconType(iType);
		
		switch (_wtoi(CLevel)) {

		// EMPRESA
		case 0 :hRoot = m_InvTree.GetTreeCtrl().InsertItem(m_GlobalData.m_RDEmpresas[_wtoi(CEmpresa)],nIconEmpresa,nIconEmpresa);
				m_InvTree.SetItemText(hRoot,1,CType);
				m_InvTree.SetItemText(hRoot,2,CLevel);
				m_InvTree.SetItemText(hRoot,3,CEmpresa);
			break;

		// TIPO DE ELEMENTO
		case 1 :hItem = m_InvTree.GetTreeCtrl().InsertItem(m_GlobalData.m_RDTipoElementos[iType],iIconType,iIconType,hRoot);
				m_InvTree.SetItemText(hItem,1,CType);
				m_InvTree.SetItemText(hItem,2,CLevel);
				m_InvTree.SetItemText(hItem,3,CEmpresa);
			break;

		// ELEMENTO
		case 2 :hItem1 = m_InvTree.GetTreeCtrl().InsertItem(CIdElemento,iIconType,iIconType,hItem);
				m_InvTree.SetItemText(hItem1,1,CType);
				m_InvTree.SetItemText(hItem1,2,CLevel);
				m_InvTree.SetItemText(hItem1,3,CEmpresa);
				
				// Por cada elemento de Nivel 2 (PC's, Servidores, etc...) es necesario buscar en la tabla INV_DETAILS_ELEMENT 
				// los campos que se quieren visualizar en las columnas del Arbol inicial
				// DBLoadDataElement (&hItem1, CIdElemento, CType) ;

				//	recset2.MoveFirst();
	           SqlString.Format(_T("SELECT ID_ELEMENTO,VALOR_VABLE,ORDEN_VABLE FROM INV_DETAILS_ELEMENT WHERE ID_ELEMENTO = '%s' ORDER BY ID_ELEMENTO ASC "),CIdElemento); 	
	           recset2.Open(CRecordset::snapshot ,SqlString,CRecordset::readOnly) ;

			   while ( !recset2.IsEOF() ) {

						recset2.GetFieldValue(_T("ID_ELEMENTO"),sIdElemento);
						recset2.GetFieldValue(_T("VALOR_VABLE"),CValorVable);
						recset2.GetFieldValue(_T("ORDEN_VABLE"),COrdenVable);

						if ( (COrdenVable == _T("100")) || (COrdenVable == _T("101")) || (COrdenVable == _T("102")) ){ 

							switch ( _wtoi(COrdenVable) ) {
								case COLUMNA_VISIBLE_1 :
									m_InvTree.SetItemText(hItem1,4,CValorVable);
								break;

								case COLUMNA_VISIBLE :
									m_InvTree.SetItemText(hItem1,5,CValorVable);
								break;

								case COLUMNA_VISIBLE_2 :
									m_InvTree.SetItemText(hItem1,6,CValorVable);
								break;
							}

						} 
						recset2.MoveNext();
					}
                    recset2.Close();					
					
			break;
		}

		/* CONTROL DE PROGESO */
		m_spProgress.SetPos(iProgress);
		CProgress.Format(_T("%i"), iProgress ) ; 
		m_EnCurso.SetWindowText(CProgress);
		iProgress = iProgress + 1 ;
		if ( iProgress > iMaximo ) iProgress = 0 ;



		// go to next record
		recset.MoveNext();
	}
	
	WaitingCursor (FALSE) ;

	recset.Close();
	m_GlobalData.DBClose ();

	/* CONTROL DE PROGESO */
	m_spProgress.SetPos(0);
	m_EnCurso.SetWindowText(_T(""));

}




/* ******************************************************************************* */
/* 97/06/2012 - Miguel Angel Gonzalez : first write
/* DBLoadDataElement :
/* Coloca en las Columnas del Arbol los datos correspondientes a INV_DETAILS_ELEMENT
/* sin abrir la base de datos. DEBE ESTAR ABIERTA PREVIAMENTE
/* ******************************************************************************* */

void CDialogInvMain::DBLoadDataElement( HTREEITEM *hItem1, CString sIDElement, CString sType) 
{
	CString SqlString,CValorVable,COrdenVable; // SQL
	int iRec=0, iRow=1;

	m_GlobalData.DBOpen ();
	CRecordset recset ( &m_GlobalData.m_dnInv );	
	
/*	SqlString.Format(_T("SELECT * FROM INV_DETAILS_ELEMENT WHERE ID_ELEMENTO = '%s' and ( ORDEN_VABLE = '%i' OR ORDEN_VABLE = '%i' )") , 
		sIDElement , COLUMNA_VISIBLE , COLUMNA_VISIBLE_1 ); 		*/

	SqlString.Format(_T("SELECT VALOR_VABLE,ORDEN_VABLE FROM INV_DETAILS_ELEMENT WHERE ID_ELEMENTO = '%s'") ,	sIDElement ); 		

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);

	while ( !recset.IsEOF() ) {

		recset.GetFieldValue(_T("VALOR_VABLE"),CValorVable);
		recset.GetFieldValue(_T("ORDEN_VABLE"),COrdenVable);
		
		switch ( _wtoi(COrdenVable) ) {

			case COLUMNA_VISIBLE_1 :
				m_InvTree.SetItemText(*hItem1,4,CValorVable);
			break;

			case COLUMNA_VISIBLE :
				m_InvTree.SetItemText(*hItem1,5,CValorVable);
			break;
			case COLUMNA_VISIBLE_2 :
				m_InvTree.SetItemText(*hItem1,6,CValorVable);
			break;
		}
		recset.MoveNext();

	}

	m_GlobalData.DBClose ();
}


/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* OnBnClickedLoad :
/* Recupera los datos de la tabla de elementos 
/* ******************************************************************************* */

void CDialogInvMain::OnBnClickedLoad()
{
	// TODO: Agregue aquí su código de controlador de notificación de control	
	m_InvTree.m_Tree.DeleteAllItems(); // Elimina todos los datos del Control Tree

	LoadInvTree(); // Carga los datos de la base de datos

		
}

/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* CheckUser :
/* Comprueba si el usuario es correcto
/* ******************************************************************************* */
BOOL CDialogInvMain::CheckUser()
{

	return TRUE;

	MessageBox(TEXT("Introduzca Usuario y Clave de la aplicación"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
	return TRUE;

}




/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* DBGetEmpresas :
/* Almacena en la variable Global las empresas junto con su código
/* ******************************************************************************* */

void CDialogInvMain::DBGetEmpresas() 
{
	
	CString SqlString, CItem; // SQL
	int i = 0 ;
	
	WaitingCursor (TRUE) ;

	m_GlobalData.DBOpen ();

	CRecordset recset ( &m_GlobalData.m_dnInv );	
	
	SqlString =  "SELECT ID_EMPRESA, NOMBRE_EMPRESA FROM RD_EMPRESA ORDER BY ID_EMPRESA ASC";

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
	
	while( !recset.IsEOF() )
	{
		// Copy each column into a variable
		recset.GetFieldValue(_T("ID_EMPRESA"),CItem);

		i = _wtoi(CItem);

		recset.GetFieldValue(_T("NOMBRE_EMPRESA"),m_GlobalData.m_RDEmpresas[i]);
						
		recset.MoveNext();
	}

	WaitingCursor (FALSE) ;
	m_GlobalData.DBClose ();
}


/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* DBGetEmpresas :
/* Almacena en la variable Global las empresas junto con su código
/* ******************************************************************************* */

void CDialogInvMain::DBGetTipoElementos() 
{
	CString SqlString,CItem; // SQL
	int iRec=0, iRow=1;
	
	// Base de Datos
	WaitingCursor (TRUE) ;
	m_GlobalData.DBOpen ();
	CRecordset recset ( &m_GlobalData.m_dnInv );	
	
	SqlString =  "SELECT ID_TIPO_ELEMENTO, NOMBRE_TIPO FROM RD_TIPO_ELEMENTO ORDER BY ID_TIPO_ELEMENTO ASC";

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
	
	int i = 0 ;
	while( !recset.IsEOF() )
	{
		// Copy each column into a variable
		recset.GetFieldValue(_T("ID_TIPO_ELEMENTO"),CItem);
		i = _wtoi(CItem);
		
		recset.GetFieldValue(_T("NOMBRE_TIPO"), m_GlobalData.m_RDTipoElementos[i]);						
		recset.MoveNext();
	}
	
	WaitingCursor (FALSE) ;
	m_GlobalData.DBClose ();
}


