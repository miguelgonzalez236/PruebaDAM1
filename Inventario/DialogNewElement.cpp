// DialogNewElement.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Inventario.h"
#include "DialogNewElement.h"
#include "afxdialogex.h"
#include "DialogInvMain.h"

#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"


// Cuadro de diálogo de CDialogNewElement

IMPLEMENT_DYNAMIC(CDialogNewElement, CDialogEx)

CDialogNewElement::CDialogNewElement(BOOL bIsNew , CWnd* pParent, CString sEmpresa, CString sLevel ,CString sIdentificador, CString sType )
	: CDialogEx(CDialogNewElement::IDD)
{
	m_sEmpresa			= sEmpresa;
	m_sLevel			= sLevel;
	m_sIdentificador	= sIdentificador;
	m_sType				= sType;

	m_IsNew				= bIsNew;

	
}

CDialogNewElement::~CDialogNewElement()
{


}

void CDialogNewElement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_EMPRESA		, m_cboEmpresa		);
	DDX_Control(pDX, IDC_TIPO			, m_cboTipoElemento	);
	DDX_Control(pDX, IDC_IDENTIFICADOR	, m_edIdentificador	);
	DDX_Control(pDX, IDC_INSERT			, m_butNewElement	);
	DDX_Control(pDX, IDOK				, m_butExit			);

	DDX_Control(pDX, IDC_GRID_VABLES_ELEMENTO, m_grid_control);

	DDX_Control(pDX, IDS_TIPO_ELEMENTO, m_staticTipoElemento);
}


BEGIN_MESSAGE_MAP(CDialogNewElement, CDialogEx)
	
	ON_WM_SYSCOMMAND()

	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)

	ON_BN_CLICKED	(IDC_INSERT	, &CDialogNewElement::OnBnClickedInsert)
	ON_BN_CLICKED	(IDOK		, &CDialogNewElement::OnBnClickedCancel)
	ON_CBN_SELENDOK	(IDC_TIPO	, &CDialogNewElement::OnCbnSelendokTipo)


	ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GRID_VABLES_ELEMENTO, OnGridStartEdit)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_VABLES_ELEMENTO, OnGridEndEdit)

END_MESSAGE_MAP()


void CDialogNewElement::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);

}
// GVN_BEGINLABELEDIT
void CDialogNewElement::OnGridStartEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
}

// GVN_ENDLABELEDIT
void CDialogNewElement::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
}
// Controladores de mensajes de CDialogNewElement

BOOL CDialogNewElement::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EnableToolTips (TRUE);			// Habilita la ayuda sensible al ratón	
	
	// Insertar valores en los combos
	InitGridComponent();
	SetUpButtons ();
	FillInitData ( ) ; // 

	// Inicialmente no hay Cambios
	m_bCambios = FALSE ;

	// Si el diálogo es para modificar un elemento existente
	if (!m_IsNew) {

		DBLoadValuesPorElemento (  );
		if (_wtoi(m_sType) > 0 ) 	m_cboTipoElemento.SetCurSel(_wtoi(m_sType)-1);
		m_cboTipoElemento.DestroyWindow();
		
		m_staticTipoElemento.SetWindowText(m_GlobalData.m_RDTipoElementos[_wtoi(m_sType)]);
		m_cboEmpresa.SetWindowText(m_sEmpresa);
		if ( _wtoi(m_sEmpresa) > 0 ) m_cboEmpresa.SetCurSel(_wtoi(m_sEmpresa)-1);
		m_edIdentificador.SetWindowText(m_sIdentificador);
	}


	return true;  // Devuelve TRUE  a menos que establezca el foco en un control
}



void CDialogNewElement::FillInitData()
{

	int i = 0 ;

	// Carga las empresas en el Combo
	i = 1; // En empresas empieza desde el 1. La 0 queda libre
	while ( ( m_GlobalData.m_RDEmpresas[i] != _T("") ) &&  ( i < MAX_EMPRESAS) )  {
	 m_cboEmpresa.AddString( LPCTSTR (m_GlobalData.m_RDEmpresas[i])) ;
	 i++;
	}


	// Carga los elementos en el combo
	i = 0 ;
	while ( ( m_GlobalData.m_RDTipoElementos[i] != _T("") ) &&  ( i <= MAX_TIPO_ELEMENTOS) ) {
	 if ( m_GlobalData.m_RDTipoElementos[i] != _T("EMPRESA") ) // El tipo empresa no puede aparece en el combo. Las empresas solo se crean por código.
		 m_cboTipoElemento.AddString( LPCTSTR (m_GlobalData.m_RDTipoElementos[i])) ;
	 i++;
	}

	
}



void CDialogNewElement::SetUpButtons()
{

	// NEW
	m_butNewElement.SetWindowText (_T("SAVE"));
	m_butNewElement.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_NEW_ELEMENT),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));


	// OK BUTTON
	m_butExit.SetWindowText (_T("EXIT"));
	m_butExit.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_EXIT),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));

	// MODIFICAR ELEMENTO - Modifica el ICONO del botón nuevo sólo en el caso de estar en modo Modificación
	if (!m_IsNew) { 
		m_butNewElement.SetWindowText (_T("SAVE"));
		m_butNewElement.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_MODIFY),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));
	}
	
   m_butNewElement.SetFocus();	
  

}

void CDialogNewElement::WaitingCursor (BOOLEAN bCursor)
{
	if (bCursor) 	AfxGetApp()->DoWaitCursor(1); // 1->>display the hourglass cursor
	else 	AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor

}

void CDialogNewElement::OnBnClickedInsert()
{
	// TODO: Agregue aquí su código de controlador de notificación de control

	if (m_IsNew) DBNewElement(  );
	else  DBModifyElement(  );

}



/* ******************************************************************************* */
/* 07/06/2012 - Miguel Angel Gonzalez : first write
/* DBNewElement :
/* Da de alta un nuevo elemento de sistemas para el grupo
/* ******************************************************************************* */
void CDialogNewElement::DBNewElement(  )
{

   	// Capturar valores de los campos Edit
	CString SqlString, CItem,COrden, CValor,CVable,CIDElement;
	int iEmpresa=0, iTipoElemento = 0 ;
	int iCount = 0 ;
	
	WaitingCursor (TRUE) ;

	// Recoje los valores de los Campos de Edicion
	m_cboEmpresa.GetWindowText	(CItem);
	iEmpresa = m_GlobalData.GetIDCodigo(CItem,ID_EMPRESAS) ;
	
	m_cboTipoElemento.GetWindowText		(CItem);
	iTipoElemento = m_GlobalData.GetIDCodigo(CItem,ID_TIPO_ELEMENTOS) ;

	// Introducir los valores en la tabla de control de todos los elementos de la empresa INV_EMPRESA
	m_edIdentificador.GetWindowText		(CIDElement);
	
	// Comprueba si el ID Ya existe
	if (DBExisteID ( CIDElement) ) {
		MessageBox(TEXT("ID Repetido. elija otro"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );
		return ;
	}

	m_GlobalData.DBOpen ();

	m_GlobalData.InsertLog( _T("NEW ELEMENT"),  CIDElement , CIDElement , _T(".") ) ;

	SqlString.Format(_T("INSERT INTO INV_EMPRESA VALUES ('%i','%s', '%i', '%i' )"), iEmpresa,	CIDElement, 2 , iTipoElemento);
	m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;

	// Introducir los valores en la tabla de valores de variables por tipo
	// Recorrer el GRID Buscando nuevos datos
	for (int i = 1 ; i < MAX_VABLES ; i++ ) 
	{
		COrden = m_grid_control.GetItemText (i , 0 );
		CVable = m_grid_control.GetItemText (i , 1 ); 
		CValor = m_grid_control.GetItemText (i , 2 ); 
		
		if ( (CValor == _T(CAMPO_VACIO))  || (CValor == _T("")) ) continue; // Si el Valor está vacio o Pending no se graba pero se continua leyendo el Grid
		if (CVable == _T("")) break; // Hasta la última variable

		m_GlobalData.InsertLog( _T("NEW ELEMENT"), CIDElement, CIDElement , CValor ) ;

		SqlString.Format(_T("INSERT INTO INV_DETAILS_ELEMENT VALUES ('%s', '%i', '%s', '%i',0 )"),CIDElement, _wtoi(COrden ), CValor, iTipoElemento);
		m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;

		m_GlobalData.InsertLog( _T("INSERT "), CIDElement , CVable , CValor ) ;

	}
	
	WaitingCursor (FALSE) ;

	MessageBox(TEXT("Inserción realizada"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );

	m_bCambios = TRUE ;

	/*
	Esta parte sirve para windows 7 pero en XP no funcionan los no Modales.
	if (!m_IsNew) DestroyWindow();
		else 		CDialogEx::OnCancel();
		*/

	CDialogEx::OnCancel();
    m_GlobalData.DBClose();
}



/* ******************************************************************************* */
/* 07/06/2012 - Miguel Angel Gonzalez : first write
/* DBModifyElement :
/* Modifica un elemento de sistemas para el grupo
/* ******************************************************************************* */
void CDialogNewElement::DBModifyElement(  )
{
	 // Capturar valores de los campos Edit
	CString SqlString, CEmpresa,COrden, CValor,CVable,CIDElement,CTipoElemento;
	int iEmpresa=0, iTipoElemento = 0 , iContador = 0 ;
	BOOL bCambiaID = FALSE, bCambiaEmpresa = FALSE, bExisteValorPrevio = FALSE;

	// Recoje los valores de los Campos de Edicion
	m_cboEmpresa.GetWindowText	(CEmpresa);
	iEmpresa = m_GlobalData.GetIDCodigo(CEmpresa,ID_EMPRESAS) ;

	m_staticTipoElemento.GetWindowText		(CTipoElemento);
	iTipoElemento = m_GlobalData.GetIDCodigo(CTipoElemento,ID_TIPO_ELEMENTOS) ;

	m_GlobalData.DBOpen ();

	// Introducir los valores en la tabla de control de todos los elementos de la empresa INV_EMPRESA
	m_edIdentificador.GetWindowText		(CIDElement);

	// Comprobar si se ha cambiado el ID del elemento
	if (CIDElement != m_sIdentificador) 
		if (MessageBox(TEXT("Está Seguro que quiere cambiar el ID del elemento?"),TEXT("Salir"),MB_YESNO) == IDNO) {
			m_edIdentificador.SetWindowText	(m_sIdentificador);
			return ;
		} else bCambiaID= TRUE;

	// Comprobar si se ha cambiado la empresa
	if (iEmpresa != _wtoi(m_sEmpresa)) 
		if (MessageBox(TEXT("Está Seguro que quiere cambiar la empresa?"),TEXT("Salir"),MB_YESNO) == IDNO) {
			m_edIdentificador.SetWindowText	(m_sIdentificador);
			m_cboEmpresa.SetCurSel(_wtoi(m_sEmpresa)-1);
			return ;
		} else bCambiaID= TRUE;

	WaitingCursor (TRUE) ;

	// Si se cambia la empresa hay que hacer un update en la tabla de INV_EMPRESA
	if (bCambiaEmpresa) {
		SqlString.Format(_T("UPDATE INV_EMPRESA SET ID_EMPRESA = '%i', ID_ELEMENTO = '%s' WHERE ID_ELEMENTO = '%s'"),iEmpresa, CIDElement,m_sIdentificador);
		m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;
	    m_GlobalData.InsertLog( _T("Cambio empresa"), CIDElement , m_sEmpresa , CEmpresa ) ;

	}

	// Si se cambia el ID y no la empresa se hace un UPDATE en la tabla INV_EMPRESA. Si se cambian los dos el cambia se ha realizado en el paso anterior
	if ( (bCambiaID) && (!bCambiaEmpresa)) {
		SqlString.Format(_T("UPDATE INV_EMPRESA SET ID_EMPRESA = '%i', ID_ELEMENTO = '%s' WHERE ID_ELEMENTO = '%s'"),iEmpresa, CIDElement,m_sIdentificador);
		m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;

		// Si se cambia el ID también hay que Update la tabla de INV_DETAILS_ELEMENT
		SqlString.Format(_T("UPDATE INV_DETAILS_ELEMENT SET ID_ELEMENTO = '%s' WHERE ID_ELEMENTO = '%s'"),CIDElement, m_sIdentificador);
		m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;
	    m_GlobalData.InsertLog( _T("Cambio empresa"), CIDElement , m_sEmpresa , CEmpresa ) ;
	}

	m_GlobalData.InsertLog( _T("UPDATE ELEMENT"), CIDElement , CIDElement , m_sIdentificador ) ;

	int iEstado = 0 ;
	CString szValorOld;
	// Introducir los valores en la tabla de valores de variables por tipo
	// Recorrer el GRID Buscando nuevos datos
	for (int i = 1 ; i < MAX_VABLES ; i++ ) 
	{
		COrden = m_grid_control.GetItemText (i , 0 );
		CVable = m_grid_control.GetItemText (i , 1 ); 
		CValor = m_grid_control.GetItemText (i , 2 ); 
		
		if ( (CValor == _T(CAMPO_VACIO))  || (CValor == _T("")) ) continue; // Si el Valor está vacio o Pending no se graba pero se continua leyendo el Grid
		if (CVable == _T("")) break; // Ultima variable implica salir del bucle.

		// Comprobar si existe valor previo
		// Existe, con lo cual se hace un Update
		iEstado = DBValorNuevo (CIDElement, COrden, CValor , &szValorOld ) ;
		if (iEstado == 1 ) continue ; // No hay que hacer nada porque el valor de la variable es la misma
		if (iEstado == 2 ) { // El valor es diferente al que existía hay que hacer un update
				SqlString.Format(
				_T("UPDATE INV_DETAILS_ELEMENT SET VALOR_VABLE = '%s'WHERE ID_ELEMENTO = '%s' and ORDEN_VABLE = '%i' ") 
				, CValor, CIDElement , _wtoi(COrden ) );
				m_GlobalData.InsertLog( _T("UPDATE"), CIDElement, CVable + _T(": ") + szValorOld.Left(80)  , CValor.Left(80) ) ;
				m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;
		}
		if (iEstado == 0 )	{		// es valor nuevo y variable nueva 
			SqlString.Format(_T("INSERT INTO INV_DETAILS_ELEMENT VALUES ('%s', '%i', '%s' ,'%i', 0 )"),CIDElement, _wtoi(COrden ), CValor.Left(80), iTipoElemento); 
			m_GlobalData.InsertLog( _T("INSERT"), CIDElement, CVable , CValor ) ;
			m_GlobalData.m_dnInv.ExecuteSQL(LPCTSTR(SqlString)) ;

		}

	}


	WaitingCursor (FALSE) ;
	m_GlobalData.DBClose ();
	MessageBox(TEXT("Modificación realizada"),TEXT("Mensaje de la Aplicación"),MB_OK );

	m_bCambios = TRUE ;

	m_GlobalData.HasBeenModifyed = TRUE ;

	CDialogEx::OnCancel();

}

// Indica si el valor es nuevo o existía previamente
int CDialogNewElement::DBValorNuevo( CString szIDElement, CString szOrden , CString szValor , CString * szValorOld ) {

	CString SqlString;
	m_GlobalData.DBOpen ();
	CRecordset recset ( &m_GlobalData.m_dnInv );	

	SqlString.Format(_T("SELECT * FROM INV_DETAILS_ELEMENT WHERE ID_ELEMENTO = '%s' AND ORDEN_VABLE = %i"), szIDElement ,_wtoi(szOrden));

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;

	while( !recset.IsEOF() )
	{
		recset.GetFieldValue(_T("VALOR_VABLE"),		*szValorOld);
		if (*szValorOld == szValor ) {
			recset.Close();
			m_GlobalData.DBClose (); 
			return 1; // El valor es el mismo que el que existía
		}
		else {
			recset.Close();
			m_GlobalData.DBClose (); 
			return 2 ; // El valor es diferente al que existía
		}
		m_GlobalData.DBClose (); 
		return 0; // No existía
	}


	recset.Close();
	m_GlobalData.DBClose ();
	return 0; // No existía el valor
}

/* ******************************************************************************* */
/* 04/06/2012 - Miguel Angel Gonzalez : first write
/* OnToolTipNotify :
/* Contiene la ayuda por cada control sensible al ratón 
/* ******************************************************************************* */
BOOL CDialogNewElement::OnToolTipNotify( UINT id,
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
        case IDC_INSERT:
                // Set the tooltip text.
				if (m_IsNew) pTTT->lpszText = _T("Inserta un nuevo elemento");
				else pTTT->lpszText = _T("Graba las modificaciones");
            break;
        case IDC_TIPO:
                // Set the tooltip text.
                if (m_IsNew) pTTT->lpszText = _T("Tipo de elemento. En función del tipo de elemento aparecerán unas variables u otras. Esto se define ea través del botón TOOLS");
				else pTTT->lpszText = _T("Tipo de elemento, no es posible cambiarlo");
            break;
        case IDC_IDENTIFICADOR:
                // Set the tooltip text.
                if (m_IsNew) pTTT->lpszText = _T("ID del nuevo elemento (Etiqueta)");
				else pTTT->lpszText = _T("ID del elemento (Etiqueta)");
            break;	
        case IDC_EMPRESA:
                // Set the tooltip text.
				if (m_IsNew) pTTT->lpszText = _T("Empresa donde se ubicará el nuevo elemento");
				else pTTT->lpszText = _T("Empresa donde ubica el elemento");
            break;			
        case IDOK:
                // Set the tooltip text.
                pTTT->lpszText = _T("Salir !!!");
            break;
        case IDC_GRID_VABLES_ELEMENTO:
                // Set the tooltip text.
                pTTT->lpszText = _T("Contiene las variables asociadas al tipo de elemento.");
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


void CDialogNewElement::InitGridComponent()
{
	m_grid_control.EnableDragAndDrop(TRUE);
	m_grid_control.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));
	m_grid_control.SetEditable(TRUE);
	m_grid_control.SetListMode(TRUE);
	
	m_grid_control.SetRowCount(MAX_ROW_GRID); 
	m_grid_control.AutoFill();
	m_grid_control.SetColumnCount(3);
	m_grid_control.SetFixedRowCount(1);
	m_grid_control.SetFixedColumnCount(2);

	m_grid_control.SetFixedBkColor(RGB(240, 240, 240));
	m_grid_control.SetGridColor (RGB(230, 243, 235));
	m_grid_control.SetGridLineColor (RGB(235, 235, 235));
	
	m_grid_control.SetHeaderSort(FALSE); // Impedir ordenar por dato de columna

	m_grid_control.SetItemText(0,0,_T("ORDEN"));
	m_grid_control.SetItemText(0,1,_T("NOMBRE"));
	m_grid_control.SetItemText(0,2,_T("VALOR"));

	m_grid_control.SetColumnWidth(0,50);
	m_grid_control.SetColumnWidth(1,180);
	m_grid_control.SetColumnWidth(2,300);

	m_grid_control.Refresh();

}


void CDialogNewElement::OnCbnSelendokTipo()
{
		// TODO: Agregue aquí su código de controlador de notificación de control

	CString sTipoElemento ; 

	int idx = m_cboTipoElemento.GetCurSel();
	m_cboTipoElemento.GetLBText( idx,sTipoElemento );


	m_grid_control.DeleteAllItems ();

	InitGridComponent();

	DBLoadVablesPorTipo(sTipoElemento); 

}




/* ******************************************************************************* */
/* 06/06/2012 - Miguel Angel Gonzalez : first write
/* DBExisteID :
/* Comprueba si el ID existe.
/* ******************************************************************************* */
BOOL CDialogNewElement::DBExisteID(CString sID) 
{
	int iCount=0;
	CString SqlString;

	m_GlobalData.DBOpen ();
	CRecordset recset ( &m_GlobalData.m_dnInv );	

	SqlString.Format(_T("SELECT * FROM INV_EMPRESA WHERE ID_ELEMENTO = '%s'"), sID);

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;

	while( !recset.IsEOF() )
	{
		recset.MoveNext();
		iCount++;
	}

	if (iCount==0) return FALSE ;
	else return TRUE;
	m_GlobalData.DBClose ();

}


/* ******************************************************************************* */
/* 06/06/2012 - Miguel Angel Gonzalez : first write
/* LoadVablesPorTipo :
/* Busca en la tabla RD_VABLES_TIPO las variables para el tipo introducido
/* ******************************************************************************* */
void CDialogNewElement::DBLoadVablesPorTipo(CString sTipoElemento) 
{
	CString SqlString,CItem,COrden,CValor,sIdentificador; // SQL
	int iTipoElemento = 0 , i = 0 ;

	iTipoElemento = m_GlobalData.GetIDCodigo(sTipoElemento, ID_TIPO_ELEMENTOS ) ;
	m_edIdentificador.GetWindowText	(sIdentificador);

	WaitingCursor (TRUE) ;

	m_GlobalData.DBOpen ();
	CRecordset recset ( &m_GlobalData.m_dnInv );	

	SqlString.Format(_T("SELECT ORDEN,NOM_VABLE,TYPE_VABLE FROM RD_VABLES_TIPO WHERE TYPE = %i ORDER BY ORDEN ASC"), iTipoElemento);

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;

	while( !recset.IsEOF() )
	{
		// Copy each column into a variable
		recset.GetFieldValue(_T("ORDEN"),		COrden);
		m_grid_control.SetItemText(i+1,0,COrden);
	
		recset.GetFieldValue(_T("NOM_VABLE"),	CItem);
		m_grid_control.SetItemText(i+1,1,CItem);

		m_grid_control.SetItemText(i+1,2,_T(CAMPO_VACIO));

		// go to next record
		recset.MoveNext();
		i++;
	}
	
	m_grid_control.Refresh();

    m_GlobalData.DBClose();
    WaitingCursor (FALSE) ;
}



void CDialogNewElement::OnBnClickedCancel()
{
	// TODO: Agregue aquí su código de controlador de notificación de control


	CDialogEx::OnCancel();

}



/* ******************************************************************************* */
/* 06/06/2012 - Miguel Angel Gonzalez : first write
/* DBLoadValuesPorElemento :
/* Busca en la tabla INV_DETAILS_ELEMENT las variables para el tipo introducido y el identificador
/* ******************************************************************************* */
void CDialogNewElement::DBLoadValuesPorElemento (  ) 
{
	CString SqlString,CVable, CTypeVable,COrden,CValor,COrdenTemp; // SQL
	int i = 0 ;
	bool bEncontrado = false;


	if 	(m_sIdentificador	== _T("") ) return ;
	if 	(m_sType			== _T("") ) return ;
	if 	(m_sEmpresa			== _T("") ) return ;
	if 	(m_sLevel			== _T("") ) return ;

	WaitingCursor (TRUE) ;

	m_GlobalData.DBOpen ();
	CRecordset recset ( &m_GlobalData.m_dnInv );	
	
	SqlString.Format(_T("SELECT ORDEN,NOM_VABLE,TYPE_VABLE FROM RD_VABLES_TIPO WHERE TYPE = %s ORDER BY ORDEN ASC"), m_sType);

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly) ;

	// Buscamos en la tabla de INV_DETAILS_ELEMENT el valor de la variable en curso
	CRecordset recset1 ( &m_GlobalData_1.m_dnInv );	
	// SqlString.Format(_T("SELECT VALOR_VABLE FROM INV_DETAILS_ELEMENT WHERE ID_ELEMENTO = '%s' and ORDEN_VABLE = %i "),m_sIdentificador,_wtoi(COrden ));
	SqlString.Format(_T("SELECT ORDEN_VABLE,VALOR_VABLE FROM INV_DETAILS_ELEMENT WHERE ID_ELEMENTO = '%s'"),m_sIdentificador);
	recset1.Open(CRecordset::snapshot,SqlString,CRecordset::readOnly) ;

	bEncontrado = false;

	while( !recset.IsEOF() )
	{

		recset.GetFieldValue(_T("ORDEN"),	COrden);
		m_grid_control.SetItemText(i+1,0,COrden);

		recset.GetFieldValue(_T("NOM_VABLE"),	CVable);
		m_grid_control.SetItemText(i+1,1,CVable);

		recset.GetFieldValue(_T("TYPE_VABLE"),	CTypeVable);
		
		bEncontrado = false;
		recset1.MoveFirst ( ) ;
		while (!recset1.IsEOF()) {
			recset1.GetFieldValue(_T("ORDEN_VABLE"),COrdenTemp);
			recset1.GetFieldValue(_T("VALOR_VABLE"),CValor);
			if (COrdenTemp==COrden) {
				m_grid_control.SetItemText(i+1,2,CValor);
				bEncontrado = true;
				break;
			}
			recset1.MoveNext();
		}

		if (!bEncontrado) {
				CValor = _T(CAMPO_VACIO); 
				m_grid_control.SetItemBkColour (i+1, 2, RGB(240, 200, 200));
				m_grid_control.SetItemText(i+1,2,CValor);
		}
		// go to next record
		recset.MoveNext();
		i++;
	}
	
	m_grid_control.Refresh();

	WaitingCursor (FALSE) ;
    m_GlobalData.DBClose();
}