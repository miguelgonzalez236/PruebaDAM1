#pragma once
#include "gridctrl_src\gridctrl.h"

// Cuadro de di�logo de CLog
extern CGlobalData m_GlobalData;

class CLog : public CDialogEx
{
	DECLARE_DYNAMIC(CLog)

public:
	CLog(CWnd* pParent = NULL);   // Constructor est�ndar
	virtual ~CLog();
	
	void InitGridLog ( );

	BOOL OnInitDialog();

	void WaitingCursor			(BOOLEAN bCursor);


	CGridCtrl	m_grid_log;


// Datos del cuadro de di�logo
	enum { IDD = IDD_DIALOG_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	DECLARE_MESSAGE_MAP()
};
