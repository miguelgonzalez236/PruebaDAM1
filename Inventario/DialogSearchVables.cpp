// DialogSearchVables.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Inventario.h"
#include "DialogSearchVables.h"
#include "afxdialogex.h"

#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"

#include "globaldata.h"

// Cuadro de diálogo de CDialogSearchVables

IMPLEMENT_DYNAMIC(CDialogSearchVables, CDialogEx)

CDialogSearchVables::CDialogSearchVables(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogSearchVables::IDD, pParent)
{

}

CDialogSearchVables::~CDialogSearchVables()
{
}

void CDialogSearchVables::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TIPO_ELEMENTO_5, m_cboTipoElemento);
	DDX_Control(pDX, IDC_GRID_SEARCH_VABLES, m_grid_control);
	DDX_Control(pDX, IDCANCEL, m_butExit);
}


BEGIN_MESSAGE_MAP(CDialogSearchVables, CDialogEx)


	ON_CBN_SELENDOK(IDC_TIPO_ELEMENTO_5, &CDialogSearchVables::OnCbnSelendokTipoElemento1)

	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)

END_MESSAGE_MAP()


// Controladores de mensajes de CDialogSearchVables



BOOL CDialogSearchVables::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitGridComponent();

	FillInitData ( ) ;
	  
	// SET UP BUTTONS
	m_butExit.SetWindowText (_T("EXIT"));
	m_butExit.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                            MAKEINTRESOURCE(IDI_EXIT),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));
	

	return TRUE;
}


/* ******************************************************************************* */
/* 04/06/2012 - Miguel Angel Gonzalez : first write
/* OnToolTipNotify :
/* Contiene la ayuda por cada control sensible al ratón 
/* ******************************************************************************* */
BOOL CDialogSearchVables::OnToolTipNotify( UINT id,
                                  NMHDR * pNMHDR,
                                  LRESULT * pResult )
{
    // Get the tooltip structure.
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

    // Actually the idFrom holds Control's handle.
    UINT CtrlHandle = pNMHDR->idFrom;

    // Check once again that the idFrom holds handle itself.
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // Get the control's ID.
        UINT nID = ::GetDlgCtrlID( HWND( CtrlHandle ));

        // Now you have the ID. depends on control,
        // set your tooltip message.
        switch( nID )
        {
 	
        case IDC_TIPO_ELEMENTO_5:
                // Set the tooltip text.
                pTTT->lpszText = _T("Seleciona el tipo de Elemento del que se quieren ver los valores");
            break;
        case IDCANCEL:
                // Set the tooltip text.
                pTTT->lpszText = _T("Salir !!!");
            break;
        case IDC_GRID_SEARCH_VABLES:
                // Set the tooltip text.
                pTTT->lpszText = _T("Contiene el valor de las variables asociadas al tipo de elemento.");
            break;	
			
        default:
            // Set the tooltip text.
            pTTT->lpszText = _T("Ayuda sin definir!!!");
            break;
        }

        return TRUE;
    }

    // Not handled.
    return FALSE;
}


void CDialogSearchVables::InitGridComponent()
{
	m_grid_control.EnableDragAndDrop(FALSE);
	m_grid_control.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));
	m_grid_control.SetEditable(FALSE);
	m_grid_control.SetListMode(FALSE);
	
	m_grid_control.AutoFill();

	m_grid_control.SetColumnCount(MAX_VABLES);
	
	m_grid_control.SetFixedRowCount(2);
	m_grid_control.SetFixedColumnCount(0);

	m_grid_control.SetFixedBkColor(RGB(240, 240, 240));
	m_grid_control.SetGridColor (RGB(230, 243, 235));
	m_grid_control.SetGridLineColor (RGB(235, 235, 235));
	
	m_grid_control.SetHeaderSort(TRUE); // ordenar por dato de columna


	// LIMPIA TODO EL GRID
	for (int i = 0 ; i < MAX_ROW_GRID; i++ ) 
		for (int j = 0 ; j < MAX_VABLES; j++ ) {
			m_grid_control.SetItemText(i,j,_T(""));
		}

	m_grid_control.Refresh();

}

void CDialogSearchVables::FillGrid()
{
	CString SqlString,sOrdenVable,sValorVable,sIDElementoAUX,sIDElemento,sTipoElemento; // SQL
	int iTipoElemento = 0 , i = 0 , iColumnaOrden = 0;

	m_cboTipoElemento.GetWindowText		(sTipoElemento);
	iTipoElemento = m_GlobalData.GetIDCodigo(sTipoElemento,ID_TIPO_ELEMENTOS) ;


	WaitingCursor (TRUE) ;


	CRecordset recset ( &m_GlobalData_2.m_dnInv );	

	SqlString.Format(_T("SELECT * FROM INV_DETAILS_ELEMENT WHERE TYPE = '%i' ORDER BY ID_ELEMENTO"), iTipoElemento);	
	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;
	
	i = 1; // Se empieza a gargar el GRID desde la columna 2
	sIDElementoAUX = _T("");
	m_grid_control.SetItemText(1,0,_T("IDENTIFICADOR")); // TITULO COLUMNA
	m_grid_control.SetItemBkColour (1, 0, RGB(200, 250, 200));

	m_grid_control.SetItemText(0,0,_T("ORDEN VABLE")); // TITULO COLUMNA

	int m_nRows = m_grid_control.GetRowCount();

	// Por cada ID_ELEMENTO recuperamos la VABLE y el VALOR de la VABLE
	while( !recset.IsEOF() )
	{
		
		recset.GetFieldValue(_T("ID_ELEMENTO"),		sIDElemento );

		// Sólo se pasa a la siguiente fila del grid si el ID_ELEMENTO CAMBIA
		if (sIDElemento != sIDElementoAUX) {
			sIDElementoAUX = sIDElemento ;
			i++;
			m_grid_control.SetRowCount(1+i); 
		}

		m_grid_control.SetItemText(i,0,sIDElemento);
		
		// Poner el ORDEN en su columna. Cada Orden está en una columna

		recset.GetFieldValue(_T("ORDEN_VABLE"),	sOrdenVable);
		iColumnaOrden = SetOrdenEnColumna ( sOrdenVable , iTipoElemento ) ;

		// POR CADA ELEMENTO HAY QUE colocar el valor en su columna correspondiente. El Orden determina la columna donde debe ir el valor
		recset.GetFieldValue(_T("VALOR_VABLE"),		sValorVable);
		m_grid_control.SetItemText(i,iColumnaOrden,sValorVable);


		// go to next record
		recset.MoveNext();



	}

	
	
    m_grid_control.AutoSize();

	m_grid_control.Refresh();

	WaitingCursor (FALSE) ;


}


int  CDialogSearchVables::SetOrdenEnColumna( CString sOrdenVable, int iTipoElemento)
{

	// iOrdenVable contiene el orden de la Variable. Este metodo devuelve la columna donde está ese Orden y si no existe crearlo
	
	int i = 1 ; // DEBE buscar desde la columna 2

	for ( i = 1 ; i < MAX_VABLES ; i++ ) {
		// Si el orden ya está en el grid devuelve la columna donde está almacenado.
		if ( sOrdenVable == m_grid_control.GetItemText (0 , i )) return i ;

	}

	// si no hemos encontrado el orden en la columna es necesario insertarlo en la primera columna vacia
	for ( i = 1 ; i < MAX_VABLES ; i++ ) {
		// Si el orden ya está en el grid devuelve la columna donde está almacenado.
		if ( _T("") == m_grid_control.GetItemText (1 , i )) {
			m_grid_control.SetItemText(0,i,sOrdenVable);
			// Al ser un nuevo Orden debe buscar el titulo en la base de datos en la tabla RD_VABLE_TIPO
			m_grid_control.SetItemText(1,i,GetTitulo(sOrdenVable,iTipoElemento ) );
			m_grid_control.SetItemBkColour (1, i, RGB(200, 250, 200));

			
			return i ;
		}

	}
	
}


CString  CDialogSearchVables::GetTitulo ( CString sOrdenVable , int iTipoElemento) {

	CString SqlString, sNombreVable ;
	
	CRecordset recset ( &m_GlobalData.m_dnInv );	
	
	SqlString.Format(_T("SELECT NOM_VABLE FROM RD_VABLES_TIPO WHERE ORDEN= '%s' and TYPE = '%i'"), sOrdenVable, iTipoElemento);

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;

	recset.GetFieldValue(_T("NOM_VABLE"),sNombreVable );

	return sNombreVable ;


}

void CDialogSearchVables::FillInitData()
{

	// Carga los elementos en el combo
	int i = 0 ;
	while ( ( m_GlobalData.m_RDTipoElementos[i] != _T("") ) &&  ( i <= MAX_TIPO_ELEMENTOS) ) {
	 if ( m_GlobalData.m_RDTipoElementos[i] != _T("EMPRESA") ) // El tipo empresa no puede aparece en el combo. Las empresas solo se crean por código.
		 m_cboTipoElemento.AddString( LPCTSTR (m_GlobalData.m_RDTipoElementos[i])) ;
	 i++;
	}

	
}


void CDialogSearchVables::WaitingCursor (BOOLEAN bCursor)
{
	if (bCursor) 	AfxGetApp()->DoWaitCursor(1); // 1->>display the hourglass cursor
	else 	AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor

}

void CDialogSearchVables::OnCbnSelendokTipoElemento1()
{
	// TODO: Agregue aquí su código de controlador de notificación de control

	InitGridComponent();

	FillGrid ( ) ;



}
