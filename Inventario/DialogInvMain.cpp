// DialogInvMain.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Inventario.h"
#include "DialogInvMain.h"
#include "afxdialogex.h"
#include "Log.h"

#include "NewCellTypes/GridURLCell.h"
#include "NewCellTypes/GridCellCombo.h"
#include "NewCellTypes/GridCellCheck.h"
#include "NewCellTypes/GridCellNumeric.h"
#include "NewCellTypes/GridCellDateTime.h"

#include "globaldata.h"
// Cuadro de diálogo de CDialogInvMain
 CGlobalData m_GlobalData;
 CGlobalData m_GlobalData_1;
 CGlobalData m_GlobalData_2;

IMPLEMENT_DYNAMIC(CDialogInvMain, CDialogEx)

CDialogInvMain::CDialogInvMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogInvMain::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDialogInvMain::~CDialogInvMain()
{
		

}

void CDialogInvMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_INV_TREE	, m_InvTree);
	DDX_Control(pDX, IDC_TOOLS		, m_boton_tools);
	DDX_Control(pDX, IDC_LOAD		, m_loadButton);
	DDX_Control(pDX, IDOK			, m_OKButton);
	DDX_Control(pDX, IDC_NEW_ELEMENT, m_newElement);
	DDX_Control(pDX, IDC_MODIFY		, m_butModifyElement);
	DDX_Control(pDX, IDC_SEARCH_VABLES, m_butSearchVables);
	DDX_Control(pDX, IDC_PROGRESS1, m_spProgress);
	DDX_Control(pDX, IDC_MAXIMO, m_Maximo);
	DDX_Control(pDX, IDC_ENCURSO, m_EnCurso);

}


BEGIN_MESSAGE_MAP(CDialogInvMain, CDialogEx)

	// Help Field
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)

	ON_BN_CLICKED	(IDC_LOAD						, &CDialogInvMain::OnBnClickedLoad			)
	ON_BN_CLICKED	(IDC_NEW_ELEMENT				, &CDialogInvMain::OnBnClickedNewElement	)
	ON_BN_CLICKED	(IDC_TOOLS						, &CDialogInvMain::OnBnClickedTools			)
	ON_BN_CLICKED	(IDC_SEARCH_VABLES				, &CDialogInvMain::OnBnClickedSearchVables	)
	ON_BN_CLICKED	(IDOK							, &CDialogInvMain::OnBnClickedOk			)
	ON_BN_CLICKED	(IDC_MODIFY						, &CDialogInvMain::OnBnClickedModify		)

	ON_NOTIFY		(TVN_ITEMCHANGED, IDC_INV_TREE	, &CDialogInvMain::OnTvnItemChangedInvTree	)
	ON_NOTIFY		(NM_DBLCLK		, IDC_INV_TREE	, &CDialogInvMain::OnNMDblclkInvTree		)

	ON_WM_TIMER()

	ON_WM_PAINT()

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CDialogInvMain::OnNMCustomdrawProgress1)

	ON_BN_CLICKED(IDC_LOG, &CDialogInvMain::OnBnClickedLog)
END_MESSAGE_MAP()


// Controladores de mensajes de CDialogInvMain

BOOL CDialogInvMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	EnableToolTips (TRUE);			// Habilita la ayuda sensible al ratón	

	 
	if ( CheckUser () ) {

		if ( GetConfigData () ) {

			DBGetConfiguration ();
			m_GlobalData.DBOpen_New ();

	        m_GlobalData_1.DBGetConfiguration();
	        m_GlobalData_1.DBOpen_New ();

	        m_GlobalData_2.DBGetConfiguration();
	        m_GlobalData_2.DBOpen_New ();

			DBGetEmpresas();
			DBGetTipoElementos();

			SetIcons (); // Incluye en m_imgList los iconos que se van a utilizar	 
			InitTree (); // Inicializa el Control Tree donde se muestran los elementos


			SetUpButtons ();

				LoadInvTree();

		} else return FALSE;
		return TRUE;  // Devuelve TRUE  a menos que establezca el foco en un control

	} else return FALSE ; 
	
	

	return TRUE;
}


void CDialogInvMain::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Agregue aquí su código de controlador de mensajes
	// No llama a CDialogEx::OnPaint() para mensajes de pintura




}




/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* OnToolTipNotify :
/* Contiene la ayuda por cada control sensible al ratón 
/* ******************************************************************************* */
BOOL CDialogInvMain::OnToolTipNotify( UINT id,
                                  NMHDR * pNMHDR,
                                  LRESULT * pResult )
{
    // Get the tooltip structure.
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

    // Actually the idFrom holds Control's handle.
    UINT CtrlHandle = pNMHDR->idFrom;

    // Check once again that the idFholds handle itself.
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // Get the control's ID.
        UINT nID = ::GetDlgCtrlID( HWND( CtrlHandle ));

        // Now you have the ID. depends on control,
        // set your tooltip message.
        switch( nID )
        {
        case IDC_INV_TREE:
                // Set the tooltip text.
                pTTT->lpszText = _T("Contiene todos los elementos de Sistemas Informáticos del Grupo Sarriena Group");
            break;
        case IDC_TOOLS:
                // Set the tooltip text.
                pTTT->lpszText = _T("Acceso a la herramienta de definición de Variables");
            break;
        case IDC_LOAD:
                // Set the tooltip text.
                pTTT->lpszText = _T("Recupera todos los datos de la base de datos. Inicializa el Arbol de elementos");
            break;			
		case IDOK:
                // Set the tooltip text.
                pTTT->lpszText = _T("Salir del Programa!!");
            break;
		case IDC_NEW_ELEMENT:
                // Set the tooltip text.
                pTTT->lpszText = _T("Acceso al diálogo para dar de alta un nuevo elemento");
            break;
		case IDC_MODIFY:
                // Set the tooltip text.
                pTTT->lpszText = _T("Acceso al diálogo para dar modificar un elemento");
            break;        

		case IDC_SEARCH_VABLES:
                // Set the tooltip text.
                pTTT->lpszText = _T("Acceso al diálogo para dar Visualizar los Datos por Tipo de Elemento");
            break;        
		case IDC_PROGRESS1:
                // Set the tooltip text.
                pTTT->lpszText = _T("Marca el progreso de la llamada a Base de Datos..... Aproximado");
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

void CDialogInvMain::WaitingCursor (BOOLEAN bCursor)
{
	if (bCursor) 	AfxGetApp()->DoWaitCursor(1); // 1->>display the hourglass cursor
		else 	AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor

}



/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* InitTree :
/* Inicializa el control tree e incluye las columnas.
/* ******************************************************************************* */
void CDialogInvMain::InitTree() 
{
	UINT uTreeStyle = TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_FULLROWSELECT;
	
	m_InvTree.GetTreeCtrl().ModifyStyle(0,uTreeStyle);

	m_InvTree.GetTreeCtrl().SetImageList(&m_imgList,TVSIL_NORMAL);

	m_InvTree.InsertColumn(0, _T("")			, LVCFMT_LEFT, 180);
	m_InvTree.InsertColumn(1, _T("Tipo")		, LVCFMT_LEFT, 0);
	m_InvTree.InsertColumn(2, _T("Level")		, LVCFMT_CENTER, 0);
	m_InvTree.InsertColumn(3, _T("Empresa")		, LVCFMT_CENTER, 0);
	m_InvTree.InsertColumn(4, _T("codigo 101")	, LVCFMT_LEFT, 250);	
	m_InvTree.InsertColumn(5, _T("codigo 100")	, LVCFMT_LEFT, 300);	
	m_InvTree.InsertColumn(6, _T("codigo 102")	, LVCFMT_LEFT, 100);	

}	

/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* SetUpButtons :
/* Inicializa los botones del diálogo
/* ******************************************************************************* */

void CDialogInvMain::SetUpButtons()
{

	// BOTON DE HERRAMIENTAS
	m_boton_tools.SetWindowText (_T("CONFIG"));
	m_boton_tools.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_TOOLS),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));
	// LOAD BUTTON
	m_loadButton.SetWindowText (_T("LOAD"));
	m_loadButton.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_LOAD),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));
	// OK BUTTON
	m_OKButton.SetWindowText (_T("EXIT"));
	m_OKButton.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_EXIT),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));
	// OK NEW ELEMENT
	m_newElement.SetWindowText (_T("NUEVO"));
	m_newElement.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_NEW_ELEMENT),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));
	// MODIFICAR ELEMENTO
	m_butModifyElement.SetWindowText (_T("MODIFY"));
	m_butModifyElement.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_MODIFY),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));

	// MODIFICAR ELEMENTO
	m_butSearchVables.SetWindowText (_T("SEARCH"));
	m_butSearchVables.SetIcon(static_cast<HICON>(LoadImage(AfxGetInstanceHandle(),
	                                             MAKEINTRESOURCE(IDI_SEARCH_VABLES),
					             IMAGE_ICON,
					      	     32,
					      	     32,
					      	     LR_DEFAULTSIZE)));


}



void CDialogInvMain::SetIcons()
{

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_imgList.Create (16, 15, ILC_COLOR32|ILC_MASK,5,1);

	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_FLOPPY));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_PC));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_SRV));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_USER));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_PRN));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_LAP));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_RED));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_WEB));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_ADSL));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_BACKUP));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_MONITOR));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_SERVCORREO));

	nIconFloppy			= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_FLOPPY));
	nIconPC				= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_PC));
	nIconSRV			= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_SRV));
	nIconUser			= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_USER));
	nIconPRN			= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_PRN));
	nIconLP				= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_LAP));
	nIconEmpresa		= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_RED));
	nIconWeb			= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_WEB));
	nIconADSL			= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_ADSL));
	nIconBackUp			= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_BACKUP));
	nIconMonitor		= m_imgList.Add(AfxGetApp()->LoadIcon(IDI_MONITOR));
	nIconServidorCorreo = m_imgList.Add(AfxGetApp()->LoadIcon(IDI_SERVCORREO));
	m_imgList.SetOverlayImage(nIconFloppy,1);

}


/* ******************************************************************************* */
/* 21/05/2012 - Miguel Angel Gonzalez : first write
/* GetConfigData :
/* Recupera la información del archivo ini : inventario.ini que debe estar en el 
/* mismo directorio que el ejecutable
/* ******************************************************************************* */
BOOL CDialogInvMain::GetConfigData()
{
	char buffer[255];
	
	GetPrivateProfileStringA("DataBase","user","ERROR",buffer,255,".\\inventario.ini");
	m_GlobalData.m_dbuser = CString(buffer);

	GetPrivateProfileStringA("DataBase","dbname","ERROR",buffer,255,".\\inventario.ini");
	m_GlobalData.m_dbname = CString(buffer);

	GetPrivateProfileStringA("DataBase","pwd","ERROR",buffer,255,".\\inventario.ini");
	m_GlobalData.m_dbpwd = CString(buffer);

	GetPrivateProfileStringA("DataBase","server","ERROR",buffer,255,".\\inventario.ini");
	m_GlobalData.m_dbserver = CString(buffer);

	if (m_GlobalData.m_dbname == "ERROR" || m_GlobalData.m_dbuser == "ERROR" ) {

		MessageBox(TEXT("Error buscando fichero inventario.ini"),TEXT("Mensaje de la Aplicación"),MB_ICONEXCLAMATION | MB_OK );

		return FALSE;
	} 

	CString message = _T("usuario =") + m_GlobalData.m_dbuser; 
	message = message + _T("   Base de datos =") + m_GlobalData.m_dbname; 
	
	// MessageBox(message,TEXT("Mensaje de la Aplicación"),MB_OK );

	return TRUE ;

}


/* ******************************************************************************* */
/* 24/05/2012 - Miguel Angel Gonzalez : first write
/* GetIconType:
/* Indica el Icono para el tipo
/* ******************************************************************************* */
int CDialogInvMain::GetIconType(int type )
{

	switch ( type ) {
		case 0 : return nIconEmpresa;
		break;
		
		case 1 : return nIconPC;
		break;
		
		case 2 : return nIconSRV;
		break;
		
		case 3 :return nIconUser;
		break;
		
		case 4 :return nIconPRN;
		break;
		
		case 5 :return nIconLP;
		break;
		
		case 6 :return nIconWeb;
		break;
		
		case 7 :return nIconADSL;
		break;

		case 8 :return nIconBackUp;
		break;

		case 9 :return nIconMonitor;
		break;

		case 10 :return nIconServidorCorreo;
		break;

		
	}

}





void CDialogInvMain::OnBnClickedOk()
{


	if (MessageBox(TEXT("Esta Seguro?"),TEXT("Salir"),MB_YESNO)!= IDNO	) {
		CDialogEx::OnCancel();
        m_GlobalData.DBClose();
		m_GlobalData.DBClose_New ();
        m_GlobalData_1.DBClose_New ();
        m_GlobalData_2.DBClose_New ();

	}

}


void CDialogInvMain::OnBnClickedNewElement()
{
	// TODO: Agregue aquí su código de controlador de notificación de control

	// Abrir el diálogo de creación de nuevo elemento

	CDialogNewElement dlg ( TRUE, this, _T("") ,_T("") , _T("") ,_T(""));

	INT_PTR nResponse = dlg.DoModal();

	// Al salir del diálogo de introducción de un nuevo elemento es necesario refrescar el CtrlTree
	OnBnClickedLoad ();

	if (nResponse == IDOK)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Aceptar
	  

	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Cancelar
	}
	

	// 


}


void CDialogInvMain::OnBnClickedTools()
{

	// Abrir el diálogo de mantenimiento de Variables por Tipo de elemento
	CVablesTipo dlg ( this );

	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Aceptar
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Cancelar
	}



}




void CDialogInvMain::OnBnClickedModify()
{



	if ( m_sLevel != _T("2")) {
		
		return ; // solo se permite modificar elementos del nivel 2.
	}
	// Abrir el diálogo de creación de nuevo elemento

	CDialogNewElement dlg ( FALSE, this, m_sEmpresa, m_sLevel, m_sIdentificador, m_sType );

	INT_PTR nResponse = dlg.DoModal();


}



void CDialogInvMain::OnTvnItemChangedInvTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: Agregue aquí su código de controlador de notificación de control
	*pResult = 0;

	CPoint pt;
	::GetCursorPos(&pt);
	m_InvTree.ScreenToClient(&pt);
	
	CTVHITTESTINFO htinfo;
	htinfo.pt = pt;
	HTREEITEM hItem = m_InvTree.HitTest(&htinfo);

	m_sIdentificador	= m_InvTree.GetItemText(hItem, 0);
	m_sType				= m_InvTree.GetItemText(hItem, 1);
	m_sLevel			= m_InvTree.GetItemText(hItem, 2);
	m_sEmpresa			= m_InvTree.GetItemText(hItem, 3);

   
}


void CDialogInvMain::OnNMDblclkInvTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Agregue aquí su código de controlador de notificación de control
	*pResult = 0;
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: Agregue aquí su código de controlador de notificación de control
	*pResult = 0;

	CPoint pt;
	::GetCursorPos(&pt);
	m_InvTree.ScreenToClient(&pt);
	
	CTVHITTESTINFO htinfo;
	htinfo.pt = pt;
	HTREEITEM hItem = m_InvTree.HitTest(&htinfo);

	m_sIdentificador	= m_InvTree.GetItemText(hItem, 0);
	m_sType				= m_InvTree.GetItemText(hItem, 1);
	m_sLevel			= m_InvTree.GetItemText(hItem, 2);
	m_sEmpresa			= m_InvTree.GetItemText(hItem, 3);
	OnBnClickedModify();


}



void CDialogInvMain::OnBnClickedSearchVables()
{
	// TODO: Agregue aquí su código de controlador de notificación de control

	// Acceso al diáologo de visualización por variable

	CDialogSearchVables dlg ( this );

	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Aceptar
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: insertar aquí el código para controlar
		//  cuándo se descarta el cuadro de diálogo con Cancelar
	}
	


}



void CDialogInvMain::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Agregue aquí su código de controlador de notificación de control
	*pResult = 0;


}





void CDialogInvMain::OnBnClickedLog()
{
	// TODO: Agregue aquí su código de controlador de notificación de control


	CLog dlg ( this );

	dlg.DoModal();



}



