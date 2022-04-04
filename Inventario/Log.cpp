// Log.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Inventario.h"
#include "Log.h"
#include "afxdialogex.h"


#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"

#include "gridctrl_src\gridctrl.h"
#include "globaldata.h"

// Cuadro de diálogo de CLog

IMPLEMENT_DYNAMIC(CLog, CDialogEx)

CLog::CLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLog::IDD, pParent)
{

}

CLog::~CLog()
{
}

void CLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GRID_LOG, m_grid_log);

}


BEGIN_MESSAGE_MAP(CLog, CDialogEx)
END_MESSAGE_MAP()



BOOL CLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitGridLog();


	return TRUE;
}


void CLog::InitGridLog()
{
	int tano, mes, dia, hora,min,tseg;

	m_grid_log.EnableDragAndDrop(FALSE);
	m_grid_log.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));
	m_grid_log.SetEditable(FALSE);
	m_grid_log.SetListMode(FALSE);
	
	m_grid_log.AutoFill();

	m_grid_log.SetColumnCount(5);
	m_grid_log.SetFixedRowCount(1);
	m_grid_log.SetFixedColumnCount(0);
	m_grid_log.SetRowCount(1); 

	m_grid_log.SetFixedBkColor(RGB(240, 240, 240));
	m_grid_log.SetGridColor (RGB(230, 243, 235));
	m_grid_log.SetGridLineColor (RGB(235, 235, 235));
	
	m_grid_log.SetHeaderSort(TRUE); // ordenar por dato de columna


	CString SqlString,szTemp; // SQL
	WaitingCursor (TRUE) ;

	m_GlobalData.DBOpen ();
	CRecordset recset ( &m_GlobalData.m_dnInv );	

	SqlString.Format(_T("SELECT OPERACION,ID_ELEMENTO, OLD_DATA,NEW_DATA, FECHA FROM INV_LOG order by fecha"));	
	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;
	
	m_grid_log.SetItemText(0,0,_T("OPERACION") );
	m_grid_log.SetItemText(0,1,_T("ID") );
	m_grid_log.SetItemText(0,2,_T("Variable") );
	m_grid_log.SetItemText(0,3,_T("Valor Nuevo") );
	m_grid_log.SetItemText(0,4,_T("Fecha de Operacion") );

	while( !recset.IsEOF() )
	{
	
			m_grid_log.SetRowCount(m_grid_log.GetRowCount() +1 ); 

			recset.GetFieldValue(_T("OPERACION"),		szTemp );
			m_grid_log.SetItemText(m_grid_log.GetRowCount()-1,0,szTemp);

			recset.GetFieldValue(_T("ID_ELEMENTO"),		szTemp );
			m_grid_log.SetItemText(m_grid_log.GetRowCount()-1,1,szTemp);

			recset.GetFieldValue(_T("OLD_DATA"),		szTemp );
			m_grid_log.SetItemText(m_grid_log.GetRowCount()-1,2,szTemp);

			recset.GetFieldValue(_T("NEW_DATA"),		szTemp );
			m_grid_log.SetItemText(m_grid_log.GetRowCount()-1,3,szTemp);

			recset.GetFieldValue(_T("FECHA"),		szTemp );
			m_grid_log.SetItemText(m_grid_log.GetRowCount()-1,4,szTemp);

		// go to next record
		recset.MoveNext();



	}
		
    m_grid_log.AutoSize();
	m_grid_log.Refresh();

	WaitingCursor (FALSE) ;
	m_GlobalData.DBClose ();


}

void CLog::WaitingCursor (BOOLEAN bCursor)
{
	if (bCursor) 	AfxGetApp()->DoWaitCursor(1); // 1->>display the hourglass cursor
	else 	AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor

}
