// VablesTipo.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Inventario.h"
#include "DialogVablesTipo.h"
#include "afxdialogex.h"

#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"


// Cuadro de diálogo de CVablesTipo

IMPLEMENT_DYNAMIC(CVablesTipo, CDialogEx)

CVablesTipo::CVablesTipo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVablesTipo::IDD, pParent)
{

}

CVablesTipo::~CVablesTipo()
{
}

void CVablesTipo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIPO_ELEMENTO, m_cboTipoElemento);
	DDX_Control(pDX, IDC_ORDEN, m_edOrden);
	DDX_Control(pDX, IDC_NOMBRE_VABLE, m_edNombreVable);
	DDX_Control(pDX, IDC_TIPO_VABLE, m_cboTipoVable);
	DDX_Control(pDX, OK, m_butNewVable);
	DDX_Control(pDX, IDC_GRID_VABLES_TIPO, m_grid_Component);
	DDX_Control(pDX, IDCANCEL, m_butExit);
	DDX_Control(pDX, IDC_MODIFY, m_butModify);
	DDX_Control(pDX, IDC_TIPO_ELEMENTO_ASOCIADO, m_cbo_tipo_asociado);
}


BEGIN_MESSAGE_MAP(CVablesTipo, CDialogEx)

	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
    ON_NOTIFY(NM_CLICK,	IDC_GRID_VABLES_TIPO,	&CVablesTipo::OnGridClick)

	ON_CBN_SELENDOK(IDC_TIPO_ELEMENTO, &CVablesTipo::OnCbnSelendokTipoElemento)

	ON_BN_CLICKED(IDC_MODIFY	, &CVablesTipo::OnBnClickedModify)
	ON_BN_CLICKED(OK			, &CVablesTipo::OnBnClickedOk)


END_MESSAGE_MAP()


void CVablesTipo::InitGridComponent()
{
	m_grid_Component.EnableDragAndDrop(TRUE);
	m_grid_Component.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));
	m_grid_Component.SetEditable(FALSE);
	m_grid_Component.SetListMode(TRUE);
	
	m_grid_Component.SetRowCount(MAX_ROW_GRID); 

	m_grid_Component.AutoFill();
	m_grid_Component.SetColumnCount(4);
	m_grid_Component.SetFixedRowCount(1);
	m_grid_Component.SetFixedColumnCount(1);

	m_grid_Component.SetFixedBkColor(RGB(240, 240, 240));
	m_grid_Component.SetGridColor (RGB(230, 243, 235));
	m_grid_Component.SetGridLineColor (RGB(235, 235, 235));
	
	m_grid_Component.SetHeaderSort(FALSE);

	m_grid_Component.SetItemText(0,0,_T("ORDEN"));
	m_grid_Component.SetItemText(0,1,_T("NOMBRE"));
	m_grid_Component.SetItemText(0,2,_T("TIPO"));
	m_grid_Component.SetItemText(0,3,_T("ASOCIADO A"));

	m_grid_Component.SetColumnWidth(0,50);
	m_grid_Component.SetColumnWidth(1,200);
	m_grid_Component.SetColumnWidth(2,50);
	m_grid_Component.SetColumnWidth(3,200);

	m_grid_Component.Refresh();

}
// Controladores de mensajes de CVablesTipo
void CVablesTipo::FillInitData()
{

	int i = 0 ;

	// Carga los elementos en el combo
	while ( ( m_GlobalData.m_RDTipoElementos[i] != _T("") ) &&  ( i <= MAX_TIPO_ELEMENTOS) ) {
	 if ( m_GlobalData.m_RDTipoElementos[i] != _T("EMPRESA") ) // El tipo empresa no puede aparece en el combo. Las empresas solo se crean por código.
		{ m_cboTipoElemento.AddString( LPCTSTR (m_GlobalData.m_RDTipoElementos[i])) ;
		m_cbo_tipo_asociado.AddString( LPCTSTR (m_GlobalData.m_RDTipoElementos[i])) ;
	 }
	 i++;
	}


	m_RowSelected = 0 ;
	
}

BOOL CVablesTipo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EnableToolTips (TRUE);			// Habilita la ayuda sensible al ratón	
	SetUpButtons();
	// Insertar valores en los combos

	InitGridComponent();
	FillInitData ( ) ;


	return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control
}




void CVablesTipo::SetUpButtons()
{

	// NEW
	m_butNewVable.SetWindowText (_T("NEW"));
	m_butNewVable.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_NEW_ELEMENT),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));
	
	// SALIR BUTTON
	m_butExit.SetWindowText (_T("EXIT"));
	m_butExit.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_EXIT),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));

	// MODIFICAR ELEMENTO
	m_butModify.SetWindowText (_T("MODIFY"));
	m_butModify.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_MODIFY),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));


}

void CVablesTipo::WaitingCursor (BOOLEAN bCursor)
{
	if (bCursor) 	AfxGetApp()->DoWaitCursor(1); // 1->>display the hourglass cursor
		else 	AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor

}


/* ******************************************************************************* */
/* 04/06/2012 - Miguel Angel Gonzalez : first write
/* OnToolTipNotify :
/* Contiene la ayuda por cada control sensible al ratón 
/* ******************************************************************************* */
BOOL CVablesTipo::OnToolTipNotify( UINT id,
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
        case IDC_TIPO_ELEMENTO:
                // Set the tooltip text.
                pTTT->lpszText = _T("Tipo de elemento");
            break;
        case IDC_ORDEN:
                // Set the tooltip text.
                pTTT->lpszText = _T("Orden en el que aparece la variable");
            break;
        case IDC_NOMBRE_VABLE:
                // Set the tooltip text.
                pTTT->lpszText = _T("Nombre de la variable");
            break;			
        case IDC_TIPO_VABLE:
                // Set the tooltip text.
                pTTT->lpszText = _T("Tipo de Variable");
            break;			
        case OK:
                // Set the tooltip text.
                pTTT->lpszText = _T("Crea la Variable");
            break;			
        case IDCANCEL:
                // Set the tooltip text.
                pTTT->lpszText = _T("Salir sin grabar");
            break;			
		case IDC_MODIFY:
                // Set the tooltip text.
                pTTT->lpszText = _T("Modifica Las Variables");
            break;
		case IDC_TIPO_ELEMENTO_ASOCIADO:
                // Set the tooltip text.
                pTTT->lpszText = _T("Tipo de elemento asociado a la Variable");
            break;			
        default:
            // Set the tooltip text.
            pTTT->lpszText = _T("Help Pending!!!");
            break;
        }

        return TRUE;
    }

    // Not handled.
    return FALSE;
}


/* ******************************************************************************* */
/* 06/06/2012 - Miguel Angel Gonzalez : first write
/* LoadVablesPorTipo :
/* Busca en la tabla RD_VABLES_TIPO las variables para el tipo introducido
/* ******************************************************************************* */
void CVablesTipo::DBLoadVablesPorTipo(CString sTipoElemento) 
{
	CString SqlString,CItem; // SQL
	int iTipoElemento = 0 , i = 0 ;

	iTipoElemento = m_GlobalData.GetIDCodigo(sTipoElemento, ID_TIPO_ELEMENTOS ) ;

	WaitingCursor (TRUE) ;


	CRecordset recset ( &m_GlobalData.m_dnInv );	

	SqlString.Format(_T("SELECT ORDEN,NOM_VABLE,TYPE_VABLE,ASOCIADO FROM RD_VABLES_TIPO WHERE TYPE = %i ORDER BY ORDEN ASC"), iTipoElemento);

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;

	while( !recset.IsEOF() )
	{
		// Copy each column into a variable
		recset.GetFieldValue(_T("ORDEN"),		CItem);
		m_grid_Component.SetItemText(i+1,0,CItem);
	
		recset.GetFieldValue(_T("NOM_VABLE"),	CItem);
		m_grid_Component.SetItemText(i+1,1,CItem);

		recset.GetFieldValue(_T("TYPE_VABLE"),	CItem);
		m_grid_Component.SetItemText(i+1,2,CItem);
	
		if (CItem == "10") {
			recset.GetFieldValue(_T("ASOCIADO"),	CItem);
			m_grid_Component.SetItemText(i+1,3,CItem);
		} else 
			m_grid_Component.SetItemText(i+1,3,_T("N/A"));

		// go to next record
		recset.MoveNext();
		i++;
	}
	
	m_grid_Component.Refresh();

	WaitingCursor (FALSE) ;
}


/* ******************************************************************************* */
/* 06/06/2012 - Miguel Angel Gonzalez : first write
/* LoadVablesPorTipo :
/* Busca en la tabla RD_VABLES_TIPO las variables para el tipo introducido
/* ******************************************************************************* */

BOOL CVablesTipo::DBInsertVablesPorTipoElemento()
{
	// TODO: Agregue aquí su código de controlador de notificación de control


	CString SqlString; // SQL
	int iTipoElemento = 0 , i = 0 ;
	CString sTipoElemento,sOrdenTemp, sOrden,sNombreVable,sTipoVable ; 

	m_cboTipoElemento.GetWindowText	(sTipoElemento);
	iTipoElemento = m_GlobalData.GetIDCodigo(sTipoElemento, ID_TIPO_ELEMENTOS ) ;

	m_edOrden.GetWindowText			(sOrden);
	m_edNombreVable.GetWindowText	(sNombreVable);
	m_cboTipoVable.GetWindowText	(sTipoVable);
	
	
	if ( _wtoi(sOrden) <= 0 ) {
		MessageBox(TEXT("Error en Orden"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
		return FALSE;
	}
	if ( (sOrden == _T("") ) || (sNombreVable == _T("") ) || (sTipoVable == _T("")  ) ) {
		MessageBox(TEXT("DATOS INCORRECTOS"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
		return FALSE; 
	}
	if ( !IsNewVariable( sOrden,sNombreVable) ) {
		MessageBox(TEXT("EL ORDEN O LA VARIABLE YA EXISTEN PARA ESTE TIPO DE ELEMENTO"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
		return FALSE; 
	}

	WaitingCursor (TRUE) ;

	m_GlobalData.InsertLog( _T("NEW VABLE"), sNombreVable ,sOrden , _T(".") ) ;

	SqlString.Format(_T("INSERT INTO RD_VABLES_TIPO ( TYPE, ORDEN, TYPE_VABLE, NOM_VABLE,ASOCIADO)  VALUES (%i,%i,  %i,'%s',0 )"), iTipoElemento,_wtoi(sOrden), _wtoi(sTipoVable), sNombreVable  );

	m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;

	WaitingCursor (FALSE) ;
		

	MessageBox(TEXT("Inserción realizada"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
	
	return TRUE;
}



void CVablesTipo::OnBnClickedOk()
{
	// TODO: Agregue aquí su código de controlador de notificación de control

	if (DBInsertVablesPorTipoElemento()) {
		OnCbnSelendokTipoElemento ();
	
		m_edOrden.SetWindowText			(_T(""));
		m_edNombreVable.SetWindowText	(_T(""));
		m_cboTipoVable.SetWindowText	(_T(""));
	}
}


void CVablesTipo::OnCbnSelendokTipoElemento()
{
	// TODO: Agregue aquí su código de controlador de notificación de control

	CString sTipoElemento ; 

	int idx = m_cboTipoElemento.GetCurSel();
	m_cboTipoElemento.GetLBText( idx,sTipoElemento );

	m_grid_Component.DeleteAllItems ();

	InitGridComponent();

	DBLoadVablesPorTipo(sTipoElemento); 
	

}



void CVablesTipo::OnBnClickedModify()
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	CString sTipoElemento,sOrden,sNombreVable,sTipoVable,SqlString, sOrdenGrid,sNombreVableGrid,sTipoVableGrid; 
	int iTipoElemento = 0 ;

	if (m_RowSelected <=0) 	{
		MessageBox(TEXT("No se ha seleccionado Variable en el Grid"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
		return ; 
	}

	m_cboTipoElemento.GetWindowText	(sTipoElemento);
	iTipoElemento = m_GlobalData.GetIDCodigo(sTipoElemento, ID_TIPO_ELEMENTOS ) ;

	m_edOrden.GetWindowText			(sOrden);
	m_edNombreVable.GetWindowText	(sNombreVable);
	m_cboTipoVable.GetWindowText	(sTipoVable);

	sOrdenGrid			= m_grid_Component.GetItemText (m_RowSelected, 0 );
	sNombreVableGrid	= m_grid_Component.GetItemText (m_RowSelected, 1 );
	sTipoVableGrid		= m_grid_Component.GetItemText (m_RowSelected, 2 ); 


	if ( _wtoi(sOrden) <= 0 ) {
		MessageBox(TEXT("Error en Orden"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
		return;
	}
		
	if ( ( _wtoi(sOrden) == COLUMNA_VISIBLE ) || ( _wtoi(sOrden) == COLUMNA_VISIBLE_1 ) ){
		MessageBox(TEXT("El Valor de esta Variable se visualizará en el Arbol principal de elementos"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
	}

	// Algun valor es nuvo
	if ( (sOrden == _T("") ) || (sNombreVable == _T("") ) || (sTipoVable == _T("")  ) ) {
		MessageBox(TEXT("DATOS INCORRECTOS"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
		return ; 
	}
		
	// Valores de los campos de edicion iguales a los del GRID
	if (	(sOrden			==	sOrdenGrid) && 
			(sNombreVable	==	sNombreVableGrid) &&
			(sTipoVable		==	sTipoVableGrid) ) 
	{

			MessageBox(TEXT("No se han realizado mofificaciones"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
		return ; 
	}

	// Comprobamos si el Orden o la 
	if ( ExisteEnelGRID ( m_RowSelected,sOrden,sNombreVable ) ) 
	{

		MessageBox(TEXT("El Orden o el Nombre de la Variable Ya existe. Seleccione otros"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
		return ; 
	}

	// FIN DE TODAS LAS COMPROBACIONES--- CONFIRMAR CAMBIOS

	CString sMensaje; 
	sMensaje.Format( _T("¿ Esta Seguro de cambiar : ") + sOrdenGrid + _T(" por ") + sOrden + _T(" y ") + sNombreVableGrid + _T(" por ") + sNombreVable + _T(" y ") + sTipoVableGrid + _T(" por ") + sTipoVable + _T(" ?"));
	if (MessageBox( sMensaje ,TEXT("Salir"),MB_YESNO)!= IDYES	) {

		return;	
	}

		

	CString olddata = sTipoElemento + _T(" ") + sOrdenGrid + _T(" ") + sNombreVableGrid + _T(" ") + sTipoVableGrid ;
	CString newdata = sTipoElemento + _T(" ") + sOrden + _T(" ") + sNombreVable + _T(" ") + sTipoVable;

	m_GlobalData.InsertLog( _T("UPDATE VABLE"), sNombreVable, olddata, newdata ) ;

	// Modificar el Orden en INV_DETAISL_ELEMENT para todos los elementos existentes
	SqlString.Format(_T("UPDATE INV_DETAILS_ELEMENT SET ORDEN_VABLE = '%i' WHERE ORDEN_VABLE = '%i' and TYPE = '%i' ") 
			, _wtoi(sOrden ) , _wtoi(sOrdenGrid) , iTipoElemento ); 		


	m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;

	// Modificar el Orden en RD_VABLES_TIPO para todos los elementos existentes
	SqlString.Format(_T("UPDATE RD_VABLES_TIPO SET ORDEN = '%i', NOM_VABLE = '%s', TYPE_VABLE = '%i' WHERE TYPE = '%i'and ORDEN = '%i' and NOM_VABLE = '%s'and TYPE_VABLE = '%s'"),
		_wtoi(sOrden ) , sNombreVable , _wtoi(sTipoVable) , 
		iTipoElemento,
		_wtoi(sOrdenGrid),  
		sNombreVableGrid,
		sTipoVableGrid ); 
	
	m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;
	
		
	OnCbnSelendokTipoElemento ();
	
}



/* ******************************************************************************* */
/* 07/06/2012 - Miguel Angel Gonzalez : first write
/* IsNewVariable :
/* Dado un Orden introducido en el campo edición determina si ya está en uso
/* ******************************************************************************* */

BOOL CVablesTipo::IsNewVariable( CString sOrden , CString sNombreVariable )
{
	CString sOrdenGrid, sNombreVariableGrid;

	for (int i = 1 ; i < MAX_VABLES ; i++ ) 
	{
		sOrdenGrid				= m_grid_Component.GetItemText (i , 0 );
		sNombreVariableGrid		= m_grid_Component.GetItemText (i , 1 );
		if  ( (sOrdenGrid == sOrden ) || ( sNombreVariableGrid == sNombreVariable )) return FALSE ; 
	}

	return TRUE;

}



/* ******************************************************************************* */
/* 07/06/2012 - Miguel Angel Gonzalez : first write
/* ExisteEnelGRID :
/* Dado un Orden introducido en el campo edición determina si ya está en uso
/* ******************************************************************************* */

BOOL CVablesTipo::ExisteEnelGRID( int iRowSelected , CString sOrden , CString sNombreVariable )
{
	CString sOrdenGrid, sNombreVariableGrid;

	for (int i = 1 ; i < MAX_VABLES ; i++ ) 
	{
		sOrdenGrid				= m_grid_Component.GetItemText (i , 0 );
		sNombreVariableGrid		= m_grid_Component.GetItemText (i , 1 );

		if  ( (sOrdenGrid == sOrden ) || ( sNombreVariableGrid == sNombreVariable )) {
			if (i == iRowSelected) continue;
			else return TRUE ; 
		}
	}

	return FALSE;

}


void CVablesTipo::OnGridClick(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	m_RowSelected = pItem->iRow;
	if (m_RowSelected <=0) {
		m_edOrden.SetWindowText			(_T(""));
		m_edNombreVable.SetWindowText	(_T(""));
		m_cboTipoVable.SetWindowText	(_T(""));
	} else {
		m_edOrden.SetWindowText			(m_grid_Component.GetItemText (m_RowSelected, 0 ));
		m_edNombreVable.SetWindowText	(m_grid_Component.GetItemText (m_RowSelected, 1 ));
		m_cboTipoVable.SetWindowText	(m_grid_Component.GetItemText (m_RowSelected, 2 ));
	}

}
