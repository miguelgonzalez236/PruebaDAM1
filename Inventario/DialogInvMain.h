#pragma once
#include "ColumnTreeCtrl.h"
#include "DialogNewElement.h"
#include "gridctrl_src\gridctrl.h"
#include "afxbutton.h"
#include "afxwin.h"
#include "GlobalData.h"
#include "DialogVablesTipo.h"
#include "DialogSearchVables.h"
#include "afxcmn.h"


// Cuadro de diálogo de CDialogInvMain

extern CGlobalData m_GlobalData;
extern CGlobalData m_GlobalData_1;
extern CGlobalData m_GlobalData_2;

class CDialogInvMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogInvMain)

public:
	CDialogInvMain(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~CDialogInvMain();

	// Funciones Varias
	void SetIcons();
	void WaitingCursor (BOOLEAN bCursor);
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR* pNMHDR,LRESULT* pResult );

	// Inicialización del diálogo
	virtual BOOL OnInitDialog();
	void SetUpButtons();

	// Inicialización de datos
	void InitTree ();
	BOOL CheckUser ();
	
	// Datos del cuadro de diálogo	
	enum { IDD = IDD_INV_DLG };

	CColumnTreeCtrl m_InvTree;
	CImageList m_imgList;
	

	// Devuelve el Icono Correspondiente al tipo
	int GetIconType (int iType);

	// Funciones de bases de Datos
	void LoadInvTree() ;
	BOOL GetConfigData();
	void DBGetEmpresas();
	void DBGetTipoElementos();
	BOOL DBGetConfiguration(  ) ;
	void DBLoadDataElement (HTREEITEM *hItem1, CString sIDElemento,CString sType) ;


	// Controles del diálogo
	CButton m_boton_tools;
	CButton m_loadButton;
	CButton m_OKButton;
	CButton m_newElement;
	CButton m_butModifyElement;
	CButton m_butSearchVables;


	CString m_sIdentificador, m_sLevel, m_sType, m_sEmpresa;
	
		// Mensajes de Eventos del diálogo
	afx_msg void OnBnClickedLoad();
	afx_msg void OnStnClickedStTitleComponent();
	afx_msg void OnBnClickedNewElement();
	afx_msg void OnBnClickedTools();	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedModify();
	afx_msg void OnTvnItemChangedInvTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkInvTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSearchVables();

;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()


	int nIconFloppy ;
	int nIconPC ;
	int nIconSRV;
	int nIconUser;
	int nIconPRN ;
	int nIconLP;
	int nIconEmpresa;
	int nIconWeb;
	int nIconADSL;
	int nIconBackUp;
	int nIconMonitor;
	int nIconServidorCorreo ;




public:
	afx_msg void OnPaint();
	CProgressCtrl m_spProgress;
	CStatic m_Maximo;
	CStatic m_EnCurso;
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedPicture();
	CStatic m_LogoSarriena;
	afx_msg void OnBnClickedPc();
	afx_msg void OnBnClickedLog();
};
