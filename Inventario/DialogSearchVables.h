#pragma once

#include "ColumnTreeCtrl.h"
#include "DialogNewElement.h"
#include "afxbutton.h"
#include "afxwin.h"
#include "GlobalData.h"
#include "stdAfx.h" 

#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"

#include "gridctrl_src\gridctrl.h"


// Cuadro de diálogo de CDialogSearchVables


class CDialogSearchVables : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSearchVables)

public:
	CDialogSearchVables(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~CDialogSearchVables();
	
	BOOL OnInitDialog();

// Datos del cuadro de diálogo
	enum { IDD = IDD_SEARCH_VABLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	DECLARE_MESSAGE_MAP()



public:
	CComboBox m_cboTipoElemento;
	CGridCtrl	m_grid_control;

	CButton m_butExit;
	
	void InitGridComponent() ;
	void FillGrid();
	void WaitingCursor			(BOOLEAN bCursor);
	
	void FillInitData() ;

	int SetOrdenEnColumna ( CString sOrdenVable , int iTipoElemento ) ;
	
	CString  GetTitulo ( CString sOrdenVable , int iTipoElemento) ;

	afx_msg void OnCbnSelendokTipoElemento1();

	// Ayuda al maantener el cursor parado en un control
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR* pNMHDR,LRESULT* pResult );


};
