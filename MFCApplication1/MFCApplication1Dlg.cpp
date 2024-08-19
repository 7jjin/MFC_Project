
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "afxinet.h"
#include "ShlObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, m_editIP(_T(""))
	, m_editID(_T(""))
	, m_editPW(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_editIP);
	DDX_Text(pDX, IDC_EDIT_ID, m_editID);
	DDX_Text(pDX, IDC_EDIT_PW, m_editPW);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMFCApplication1Dlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CMFCApplication1Dlg::OnBnClickedButtonDisconnect)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMFCApplication1Dlg::OnTvnSelchangedTree1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	
	// List Control 초기화
	CRect rt;
	m_ListCtrl.GetWindowRect(&rt);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, rt.Width()*0.4);
	m_ListCtrl.InsertColumn(1, _T("Type"), LVCFMT_LEFT, rt.Width() * 0.2);
	m_ListCtrl.InsertColumn(2, _T("Last Modified"), LVCFMT_LEFT, rt.Width() * 0.2);
	m_ListCtrl.InsertColumn(3, _T("Size"), LVCFMT_RIGHT, rt.Width() * 0.2);

	// 디렉토리 로드를 시작할 경로를 설정합니다. 예를 들어, C:\부터 시작할 수 있습니다.
	CString strInitialPath = _T("C:\\");
	LoadDirectoryStructure(strInitialPath, TVI_ROOT);

	return TRUE;
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CMFCApplication1Dlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


// FTP 연결 메서드
void CMFCApplication1Dlg::OnBnClickedButtonConnect()
{
	UpdateData(TRUE);

	CInternetSession session(_T("FTPExampleSession"));

	try
	{
		if (m_pFtpConnection != nullptr)
		{
			m_pFtpConnection->Close();
			delete m_pFtpConnection;
			m_pFtpConnection = nullptr;
		}

		m_pFtpConnection = session.GetFtpConnection(m_editIP, m_editID, m_editPW);
		AfxMessageBox(_T("FTP 연결 성공"));
	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		AfxMessageBox(szErr);
		pEx->Delete();
	}
}

// FTP 해제 메서드
void CMFCApplication1Dlg::OnBnClickedButtonDisconnect()
{
	if (m_pFtpConnection != nullptr)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
		m_pFtpConnection = nullptr;

		AfxMessageBox(_T("FTP 연결이 해제되었습니다."));
	}
	else
	{
		AfxMessageBox(_T("현재 활성화된 FTP 연결이 없습니다."));
	}
}

// 내 디렉토리 목록 가져오는 메서드
void CMFCApplication1Dlg::LoadDirectoryStructure(const CString& strPath, HTREEITEM hParentItem)
{
	CFileFind finder;
	CString strWildcard(strPath);
	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);
	int nIndex = 0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// '.' 및 '..'을 건너뜁니다.
		if (finder.IsDots())
			continue;

		CString strFilePath = finder.GetFilePath();
		CString strFileName = finder.GetFileName();

		// 트리 컨트롤에 아이템 추가
		HTREEITEM hItem = m_TreeCtrl.InsertItem(strFileName, hParentItem);

		// 디렉토리인 경우, 재귀적으로 하위 디렉토리를 추가합니다.
		if (finder.IsDirectory())
		{
			LoadDirectoryStructure(strFilePath, hItem);
		}
	}
}



void CMFCApplication1Dlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hSelectedItem = m_TreeCtrl.GetSelectedItem();
	if (hSelectedItem == NULL) {
		return;
	}
	// 선택된 항목 경로
	CString strSelectedPath = GetFullPathFromTreeItem(hSelectedItem);

	// List Control 초기화
	m_ListCtrl.DeleteAllItems();

	CFileFind finder;
	CString strWildcard(strSelectedPath);
	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);
	int nIndex = 0;
	while (bWorking) {
		bWorking = finder.FindNextFileW();

		if (finder.IsDots())
			continue;

		CString strFilePath = finder.GetFilePath();
		CString strFileName = finder.GetFileName();
		// 파일인지 디렉토리인지 확인
		BOOL bIsDirectory = finder.IsDirectory();

		// 파일 크기
		ULONGLONG fileSize = finder.GetLength();

		// 마지막 수정 날짜와 시간
		CTime lastModified;
		finder.GetLastWriteTime(lastModified);
		CString strLastModified = lastModified.Format(_T("%Y-%m-%d %H:%M:%S"));

		// List Control에 정보 추가
		int nItem = m_ListCtrl.InsertItem(nIndex++, strFileName);
		m_ListCtrl.SetItemText(nItem, 1, bIsDirectory ? _T("Folder") : _T("File"));
		m_ListCtrl.SetItemText(nItem, 2, strLastModified);
		if (!bIsDirectory)
		{
			CString strFileSize;
			strFileSize.Format(_T("%llu bytes"), fileSize);
			m_ListCtrl.SetItemText(nItem, 3, strFileSize);
		}
	}
	*pResult = 0;
}


CString CMFCApplication1Dlg::GetFullPathFromTreeItem(HTREEITEM hItem)
{
	CString strPath;
	CString strItemText;

	// 아이템이 존재할 때까지 부모 아이템을 따라가며 경로를 구성
	while (hItem != NULL)
	{
		// 현재 아이템의 텍스트를 가져옴
		strItemText = m_TreeCtrl.GetItemText(hItem);

		// 현재 경로 앞에 추가 (역순으로 구성됨)
		strPath = _T("\\") + strItemText + strPath;

		// 부모 아이템으로 이동
		hItem = m_TreeCtrl.GetParentItem(hItem);
	}

	// 루트 경로를 추가 (C 드라이브 가정)
	strPath = _T("C:\\") + strPath;

	// 최종적으로 전체 경로 반환
	return strPath;
}