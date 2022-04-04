#pragma once
#include "afxwin.h"
#include "DialogInvMain.h"
#include "stdAfx.h" 
#include "GlobalData.h"
#include "ColumnTreeCtrl.h"
#include "DialogNewElement.h"
#include "gridctrl_src\gridctrl.h"
#include "gridctrl_src\gridctrl.h"

// Cuadro de diálogo de CVablesTipo

class CVablesTipo : public CDialogEx
{
	DECLARE_DYNAMIC(CVablesTipo)

public:
	CVablesTipo(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~CVablesTipo();

// Datos del cuadro de diálogo
	enum { IDD = IDD_VABLES_TIPO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cboTipoElemento;
	CComboBox m_cboTipoVable;
	CComboBox m_cbo_tipo_asociado;

	CButton m_butNewVable;
	CButton m_butExit;
	CButton m_butModify;

	CEdit m_edOrden;
	CEdit m_edNombreVable;

	int m_RowSelected;

	CGridCtrl m_grid_Component;

	void SetUpButtons();
	void WaitingCursor (BOOLEAN bCursor);

	// Ayuda al maantener el cursor parado en un control
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR* pNMHDR,LRESULT* pResult );


	void FillInitData();
	void DBLoadVablesPorTipo(CString sTipoElemento) ;	
	BOOL DBInsertVablesPorTipoElemento() ;
	void InitGridComponent();
	BOOL IsNewVariable( CString sOrden, CString sNombreVariable);
	BOOL ExisteEnelGRID( int iRowSelected , CString sOrden , CString sNombreVariable );


	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelendokTipoElemento();
	afx_msg void OnBnClickedModify();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);

};
