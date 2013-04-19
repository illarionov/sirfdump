// sirfdump.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "sirfdump0.h"

#define MAX_LOADSTRING 100


struct {
    enum t_output_type type;
    const TCHAR * name;
} static const OUTPUT_TYPES[] = {
    { OUTPUT_RINEX,  TEXT("RINEX observation") },
    { OUTPUT_RINEX_NAV,  TEXT("RINEX navigation") },
    { OUTPUT_NMEA,  TEXT("NMEA") },
    { OUTPUT_DUMP,  TEXT("Dump") },
    { OUTPUT_RTCM,  TEXT("RTCM 3.0") },
    { OUTPUT_RINEX, NULL}
};

// Global Variables:
static HINSTANCE			g_hInst;			// current instance
static HWND				g_hWndMenuBar;		// menu bar handle
static HANDLE              g_hConvertThread;   // 


// Forward declarations of functions included in this code module:
static BOOL			InitInstance(HINSTANCE, int);
static INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
static INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

static void setSelectedOutputType(HWND hDlg, t_output_type type);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIRFDUMP));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    TCHAR szTitle[MAX_LOADSTRING];		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

    g_hInst = hInstance; // Store instance handle in our global variable
    g_hConvertThread = NULL;

    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the device specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_SIRFDUMP, szWindowClass, MAX_LOADSTRING);

    //If it is already running, then focus on the window, and exit
    hWnd = FindWindow(szWindowClass, szTitle);	
    if (hWnd) 
    {
        // set focus to foremost child window
        // The "| 0x00000001" is used to bring any owned windows to the foreground and
        // activate them.
        SetForegroundWindow((HWND)((ULONG) hWnd | 0x00000001));
        return 0;
    } 

	hWnd = CreateDialog (hInstance,
		MAKEINTRESOURCE (IDD_MAIN),
		NULL,
		WndProc);

    SHDoneButton(hWnd, SHDB_SHOW);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

static void ShowInputFilePickerDialog(HWND hDlg)
{
    OPENFILENAME ofn;
    TCHAR fname[MAX_PATH*sizeof(TCHAR)] = TEXT("");

    memset (&ofn, 0, sizeof (ofn));
    ofn.lStructSize  = sizeof (OPENFILENAME);
    ofn.hwndOwner = hDlg;
    ofn.lpstrFile = fname;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = TEXT("Select input file");
    ofn.lpstrFilter = TEXT("SiRF Binary\0*.bin;*.sirf;*.srf;*.raw\0All\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; 
    GetDlgItemText(hDlg, IDC_INPUT_FILE_EDITBOX, fname,
        sizeof(fname)/sizeof(fname[0]));
    if (GetOpenFileName (&ofn))
        SetDlgItemText(hDlg, IDC_INPUT_FILE_EDITBOX, fname);
}

static void ShowOutputFilePickerDialog(HWND hDlg)
{
    t_output_type OutType;
    int FilterIndex;
    OPENFILENAME ofn;
    TCHAR fname[MAX_PATH*sizeof(TCHAR)] = TEXT("");

    OutType = GetSelectedOutputType(hDlg);
    switch (OutType) {
        case OUTPUT_RINEX:     FilterIndex = 1; break;
        case OUTPUT_RINEX_NAV: FilterIndex = 2; break;
        case OUTPUT_NMEA:      FilterIndex = 3; break;
        case OUTPUT_DUMP:      FilterIndex = 4; break;
        case OUTPUT_RTCM:      FilterIndex = 5; break;
        default: FilterIndex = 0; break;
    }

    memset (&ofn, 0, sizeof (ofn));
    ofn.lStructSize  = sizeof (OPENFILENAME);
    ofn.hwndOwner = hDlg;
    ofn.lpstrFile = fname;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = TEXT("RINEX observation\0*.obs\0RINEX navigation\0*.nav\0NMEA\0*.nmea\0Dump\0*.dump\0RTCM 3.0\0*.rtcm\0");
    ofn.nFilterIndex = FilterIndex;
    ofn.lpstrTitle = TEXT("Select output file");
    ofn.Flags = OFN_HIDEREADONLY; 
    GetDlgItemText(hDlg, IDC_OUTPUT_FILE_EDITBOX, fname,
        sizeof(fname)/sizeof(fname[0]));
    if (GetSaveFileName (&ofn)) {
        SetDlgItemText(hDlg, IDC_OUTPUT_FILE_EDITBOX, fname);
        switch (ofn.nFilterIndex) {
            case 1: OutType = OUTPUT_RINEX; break;
            case 2: OutType = OUTPUT_RINEX_NAV; break;
            case 3: OutType = OUTPUT_NMEA; break;
            case 4: OutType = OUTPUT_DUMP; break;
            case 5: OutType = OUTPUT_RTCM; break;
            default: break;
        }
        setSelectedOutputType(hDlg, OutType);
    }
}

static void PopulateOutputTypeDropdownList(HWND hDlg)
{
    int i;
    HWND combobox;

    combobox = GetDlgItem(hDlg, IDC_OUTPUT_TYPE_COMBOBOX);

    for(i=0; OUTPUT_TYPES[i].name != NULL; ++i) {
        SendMessage(combobox, CB_ADDSTRING, 0, (LPARAM)OUTPUT_TYPES[i].name);
    }

    setSelectedOutputType(hDlg, OUTPUT_TYPES[0].type);
}

enum t_output_type GetSelectedOutputType(HWND hDlg)
{
    int idx = SendDlgItemMessage(hDlg, IDC_OUTPUT_TYPE_COMBOBOX, CB_GETCURSEL, 0, 0);
    return OUTPUT_TYPES[idx].type;
}

static void setSelectedOutputType(HWND hDlg, t_output_type type)
{
    int i;
    for(i=0; OUTPUT_TYPES[i].name != NULL; ++i) {
        if (OUTPUT_TYPES[i].type == type) {
            SendDlgItemMessage(hDlg, IDC_OUTPUT_TYPE_COMBOBOX, CB_SETCURSEL, (WPARAM)i, 0);
            break;
        }
    }
}

static void RefreshOutputFileExtension(HWND hDlg)
{
    enum t_output_type type;
    const TCHAR *extension;
    int length;
    TCHAR filename[MAX_PATH];
    TCHAR *dot;

    type = GetSelectedOutputType(hDlg);
    
    switch (type) {
        case OUTPUT_RINEX:     extension = TEXT(".obs") ; break;
        case OUTPUT_RINEX_NAV: extension = TEXT(".nav"); break;
        case OUTPUT_NMEA:      extension = TEXT(".nmea"); break;
        case OUTPUT_DUMP:      extension = TEXT(".dump"); break;
        case OUTPUT_RTCM:      extension = TEXT(".rtcm"); break;
        default:
            extension = NULL;
            break;
    }

    if (extension == NULL)
        return;

    length = GetDlgItemText((HWND)hDlg, IDC_OUTPUT_FILE_EDITBOX, filename, MAX_PATH);
    if (length <= 0) {
        return;
    }

    dot = _tcsrchr(filename, '.');

    if (dot == NULL)
        return;

    *dot = 0;

    _tcsncat(filename, extension, MAX_PATH-1-_tcslen(filename));

    SetDlgItemText(hDlg, IDC_OUTPUT_FILE_EDITBOX, filename);

}

static void Convert(HWND hDlg)
{
    g_hConvertThread = CreateThread(NULL, 0, ConvertThread, (LPVOID)hDlg, 0, NULL);
    if (g_hConvertThread == NULL) {
        MessageBox(hDlg, TEXT("Unable to create the convert thread"), 
			TEXT("Error"), MB_OK);
    }else {
        CloseHandle(g_hConvertThread);
    }

}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
static INT_PTR CALLBACK WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;

    static SHACTIVATEINFO s_sai;
	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hDlg, About);
                    break;
                case IDM_OK:
                case IDOK:
                case IDCLOSE:
                case IDCANCEL:
				case IDABORT:
                case IDYES:
				case IDNO:
                    EndDialog(hDlg, wmId);
                    PostQuitMessage(0);
                    break;
                case IDC_SELECT_INPUT_FILE_BUTTON:
                    ShowInputFilePickerDialog(hDlg);
                    break;
                case IDC_SELECT_OUTPUT_FILE_BUTTON:
                    ShowOutputFilePickerDialog(hDlg);
                    break;
                case IDC_CONVERT_BUTTON:
                    Convert(hDlg);
                    break;
                case IDC_CANCEL_BUTTON:
                    cancelConvert();
                    break;
                case IDC_OUTPUT_TYPE_COMBOBOX:
                    if (wmEvent == CBN_SELENDOK) {
                        RefreshOutputFileExtension(hDlg);
                    }
                    return FALSE;
                    break;
                default:
                    return FALSE;
            }
            break;
        case WM_INITDIALOG:
            SHMENUBARINFO mbi;
            SHINITDLGINFO shidi;

            memset(&shidi, 0, sizeof(SHINITDLGINFO));
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
            shidi.hDlg = hDlg;
			SHInitDialog(&shidi);

            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hDlg;
            mbi.nToolBarId = IDR_MENU;
            mbi.hInstRes   = g_hInst;

            if (!SHCreateMenuBar(&mbi)) 
            {
                g_hWndMenuBar = NULL;
            }
            else
            {
                g_hWndMenuBar = mbi.hwndMB;
            }

            // Initialize the shell activate info structure
            memset(&s_sai, 0, sizeof (s_sai));
            s_sai.cbSize = sizeof (s_sai);

            PopulateOutputTypeDropdownList(hDlg);

            break;
        case WM_DESTROY:
            CommandBar_Destroy(g_hWndMenuBar);
            PostQuitMessage(0);
            break;
        case WM_ACTIVATE:
            // Notify shell of our activate message
            SHHandleWMActivate(hDlg, wParam, lParam, &s_sai, FALSE);
            break;
        case WM_SETTINGCHANGE:
            SHHandleWMSettingChange(hDlg, wParam, lParam, &s_sai);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

// Message handler for about box.
static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
            {
                // Create a Done button and size it.  
                SHINITDLGINFO shidi;
                shidi.dwMask = SHIDIM_FLAGS;
                shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_EMPTYMENU;
                shidi.hDlg = hDlg;
                SHInitDialog(&shidi);
            }
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hDlg, message);
            return TRUE;

    }
    return (INT_PTR)FALSE;
}
