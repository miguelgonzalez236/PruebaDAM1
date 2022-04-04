#pragma once
#include "afxwin.h"
#include "DialogInvMain.h"
#include "stdAfx.h" 
#include "GlobalData.h"

#include "ColumnTreeCtrl.h"
#include "gridctrl_src\gridctrl.h"
// Cuadro de diálogo de CDialogNewElement


class CDialogNewElement : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogNewElement)

public:
	CDialogNewElement(BOOL bIsNew, CWnd* pParent, CString sEmpresa , CString sLevel ,CString sIdentificador, CString sType );   // Constructor estándar
	virtual ~CDialogNewElement();


	void FillInitData() ;

// Datos del cuadro de diálogo
	enum { IDD = IDD_NEW_ELEMENT };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV
	BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()

public:
	CComboBox	m_cboEmpresa;
	CComboBox	m_cboTipoElemento;
	CButton		m_butNewElement;
	CEdit		m_edIdentificador;
	CButton		m_butExit;
	CStatic		m_staticTipoElemento;

	CString		m_sIdentificador;
	CString		m_sType;
	CString		m_sEmpresa;
	CString		m_sLevel;

	CGridCtrl	m_grid_control;

	BOOL		m_IsNew;
	BOOL		m_bCambios;


	void SetUpButtons			(  );
	void WaitingCursor			(BOOLEAN bCursor);
	void InitGridComponent		();
	void DBLoadVablesPorTipo	(CString sTipoElemento);
	void DBModifyElement		(  ) ;
	void DBNewElement			(  ) ;
	void DBLoadValuesPorElemento(  ) ;
	BOOL DBExisteID				( CString sID ) ;
	int	DBValorNuevo			( CString szIDElement, CString szOrden , CString szValor , CString * szValorOld ) ;

	// Ayuda al maantener el cursor parado en un control
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR* pNMHDR,LRESULT* pResult );

	afx_msg void OnBnClickedInsert();
	afx_msg void OnCbnSelendokTipo();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeEmpresa();

	afx_msg void OnGridStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);


	
};
