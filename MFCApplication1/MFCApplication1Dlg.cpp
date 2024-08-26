﻿// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "afxinet.h"
#include "ShlObj.h"
#include "direct.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <stack>


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
	int g_totalFiles = 0;
	int g_uploadedFiles = 0;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	// Drag & Drop 관련 멤버 변수
	BOOL m_bDragging;
	CImageList* m_pDragImage;
	int m_nDragIndex;
	CWnd* m_pDropWnd;
	CPoint m_ptDropPoint;
	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
private:
	CInternetSession* m_pInternetSession;
	CFtpConnection* m_pFtpConnection;
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
	, m_editIP(_T("127.0.0.1"))
	, m_editID(_T("test"))
	, m_editPW(_T("1234"))
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
	DDX_Control(pDX, IDC_TREE2, m_TreeCtrl2);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl2);
	DDX_Control(pDX, IDC_EDIT_LOCAL_PATH, m_localPath);
	DDX_Control(pDX, IDC_EDIT_REMOTE_PATH, m_remotePath);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_Logtext);
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
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CMFCApplication1Dlg::OnTvnSelchangedTree2)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, &CMFCApplication1Dlg::OnLvnBegindragList1)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST2, &CMFCApplication1Dlg::OnLvnBegindragList2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// List Control 초기화
	CRect rt;
	m_ListCtrl.GetWindowRect(&rt);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, rt.Width() * 0.4);
	m_ListCtrl.InsertColumn(1, _T("Type"), LVCFMT_LEFT, rt.Width() * 0.2);
	m_ListCtrl.InsertColumn(2, _T("Last Modified"), LVCFMT_LEFT, rt.Width() * 0.2);
	m_ListCtrl.InsertColumn(3, _T("Size"), LVCFMT_RIGHT, rt.Width() * 0.2);

	m_ListCtrl2.GetWindowRect(&rt);
	m_ListCtrl2.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListCtrl2.InsertColumn(0, _T("Name"), LVCFMT_LEFT, rt.Width() * 0.4);
	m_ListCtrl2.InsertColumn(1, _T("Type"), LVCFMT_LEFT, rt.Width() * 0.2);
	m_ListCtrl2.InsertColumn(2, _T("Last Modified"), LVCFMT_LEFT, rt.Width() * 0.2);
	m_ListCtrl2.InsertColumn(3, _T("Size"), LVCFMT_RIGHT, rt.Width() * 0.2);

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




/// <summary>
/// 현재 시간 반환 메서드
/// </summary>
/// <returns></returns>
CString GetFormattedCurrentTime()
{
	// 현재 시간 가져오기
	CTime t = CTime::GetCurrentTime();

	// 원하는 형식으로 시간 포맷
	CString strTime = t.Format("%Y-%m-%d %H:%M:%S  ");

	return strTime;
}

/// <summary>
/// LogText에 글 추가 메서드
/// </summary>
/// <param name="newText"></param>
void CMFCApplication1Dlg::AppendTextToEditControl(CString newText)
{
    // Edit Control의 현재 텍스트를 가져옴
    CString currentText;
	m_Logtext.GetWindowText(currentText);

    // 기존 텍스트에 줄바꿈과 함께 새 텍스트 추가
    if (!currentText.IsEmpty()) // 기존 텍스트가 비어 있지 않으면 줄바꿈 추가
    {
        currentText += _T("\r\n");
    }
    currentText += newText;

    // Edit Control에 업데이트된 텍스트 설정
	m_Logtext.SetWindowText(currentText);

    // 텍스트의 끝으로 커서를 이동
    int nLength = m_Logtext.GetWindowTextLength();
	m_Logtext.SetSel(nLength, nLength);

    // 현재의 총 라인 수를 가져와서 마지막 라인으로 스크롤 이동
    int nLineCount = m_Logtext.GetLineCount();
	m_Logtext.LineScroll(nLineCount - 1);
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
		AppendTextToEditControl(GetFormattedCurrentTime() +_T("FTP 연결 성공\n"));
		AfxMessageBox(_T("FTP 연결 성공"));

		// FTP 디렉토리 트리 로드
		LoadFTPDirectoryStructure(m_pFtpConnection, _T("/"), TVI_ROOT);
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
		AppendTextToEditControl(GetFormattedCurrentTime() + _T("FTP 연결이 해제되었습니다."));
		AfxMessageBox(_T("FTP 연결이 해제되었습니다."));
	}
	else
	{
		m_Logtext.SetWindowText(GetFormattedCurrentTime() + _T("현재 활성화된 FTP 연결이 없습니다.\n"));
		AfxMessageBox(_T("현재 활성화된 FTP 연결이 없습니다."));
	}
}



////////////////////////////////////////////////// Local Tree/List Contorl 관련 메소드 ///////////////////////////////////////////////////////////////

// 내 디렉토리 목록을 가져와서 트리뷰에 뿌려주는 메서드
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



// 클릭한 TreeView의 디레토리 목록을 ListView에 보여주는 메서드
void CMFCApplication1Dlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

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


// 클릭한 노드의 경로를 보여줌.
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

	m_localPath.SetWindowTextW(strPath);

	// 최종적으로 전체 경로 반환
	return strPath;
}



////////////////////////////////////////////////// server Tree/List Contorl 관련 메소드 ///////////////////////////////////////////////////////////////


// FTP 서버의 디렉토리 목록을 Tree Control로 보여주기
void CMFCApplication1Dlg::LoadFTPDirectoryStructure(CFtpConnection* pFtpConnection, const CString& strStartPath, HTREEITEM hParentItem)
{
	// 스택을 사용해 디렉토리 경로를 관리
	std::stack<std::pair<CString, HTREEITEM>> directories;
	directories.push(std::make_pair(strStartPath, hParentItem));

	while (!directories.empty())
	{
		// 스택에서 디렉토리 경로와 Tree Control 아이템을 꺼냄
		std::pair<CString, HTREEITEM> current = directories.top();
		directories.pop();

		CString currentPath = current.first;
		HTREEITEM parentItem = current.second;

		// 디렉토리 경로에 슬래시 추가
		if (currentPath.Right(1) != _T("/"))
			currentPath += _T("/");

		// 파일 찾기 객체 생성
		CFtpFileFind finder(pFtpConnection);
		CString strSearchPath = currentPath + _T("*");

		BOOL bWorking = finder.FindFile(strSearchPath);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			// "."과 ".."을 무시
			if (finder.IsDots())
				continue;

			// 파일 이름과 전체 경로 얻기
			CString strFileName = finder.GetFileName();
			CString strFilePath = currentPath + strFileName;

			// 트리 컨트롤에 아이템 추가
			HTREEITEM hItem = m_TreeCtrl2.InsertItem(strFileName, parentItem);

			// 디렉토리인 경우 스택에 추가
			if (finder.IsDirectory())
			{
				directories.push(std::make_pair(strFilePath, hItem));
			}
		}
		// 검색 종료
		finder.Close();
	}
}



/// <summary>
///  클릭한 노드의 하위 디렉토리를 List Control에 뿌려주기
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void CMFCApplication1Dlg::OnTvnSelchangedTree2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hSelectedItem = m_TreeCtrl2.GetSelectedItem();
	if (hSelectedItem == NULL) {
		return;
	}

	// 선택된 항목 경로
	CString strSelectedPath = GetFullPathFromTreeItem2(hSelectedItem);

	// List Control 초기화
	m_ListCtrl2.DeleteAllItems();

	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;

	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);

	// 파일 찾기 객체 생성
	CFtpFileFind fileFind(ftpConnection);

	CString strSearchPath = strSelectedPath;

	BOOL bWorking = fileFind.FindFile(strSearchPath + _T("\\*.*"));

	int nIndex = 0;

	while (bWorking)
	{
		bWorking = fileFind.FindNextFile();

		if (fileFind.IsDots())
			continue;

		CString strFileName = fileFind.GetFileName();
		BOOL bIsDirectory = fileFind.IsDirectory();
		ULONGLONG fileSize = fileFind.GetLength();
		CTime lastModified;
		fileFind.GetLastWriteTime(lastModified);
		CString strLastModified = lastModified.Format(_T("%Y-%m-%d %H:%M:%S"));

		// 리스트 컨트롤에 파일 또는 디렉토리 정보 추가
		int nItem = m_ListCtrl2.InsertItem(nIndex++, strFileName);
		m_ListCtrl2.SetItemText(nItem, 1, bIsDirectory ? _T("Folder") : _T("File"));
		m_ListCtrl2.SetItemText(nItem, 2, strLastModified);

		if (!bIsDirectory)
		{
			CString strFileSize;
			strFileSize.Format(_T("%llu bytes"), fileSize);
			m_ListCtrl2.SetItemText(nItem, 3, strFileSize);
		}
	}
	fileFind.Close();

	*pResult = 0;
}

/// <summary>
/// FTP 서버 디렉토리 경로 찾기
/// </summary>
/// <param name="hItem"></param>
/// <returns></returns>
CString CMFCApplication1Dlg::GetFullPathFromTreeItem2(HTREEITEM hItem)
{
	CString strPath;
	CString strItemText;

	// 아이템이 존재할 때까지 부모 아이템을 따라가며 경로를 구성
	while (hItem != NULL)
	{
		strItemText = m_TreeCtrl2.GetItemText(hItem);
		strPath = _T("/") + strItemText + strPath;
		hItem = m_TreeCtrl2.GetParentItem(hItem);
	}

	m_remotePath.SetWindowTextW(strPath);

	// 최종적으로 전체 경로 반환
	return strPath;
}



////////////////////////////////////////////////// Drag 관련 메소드 ///////////////////////////////////////////////////////////////////////////

/// <summary>
/// Local List Control Drag 설정
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void CMFCApplication1Dlg::OnLvnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// 드래그 시작
	m_bDragging = TRUE;
	m_nDragIndex = pNMLV->iItem;
	m_pDragImage = m_ListCtrl.CreateDragImage(m_nDragIndex, &m_ptDropPoint);
	m_pDragImage->BeginDrag(0, CPoint(0, 0));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMLV->ptAction);

	SetCapture();
	*pResult = 0;
}

/// <summary>
/// Remote List Control Drag 설정
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void CMFCApplication1Dlg::OnLvnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// 드래그 시작
	m_bDragging = TRUE;
	m_nDragIndex = pNMLV->iItem;
	m_pDragImage = m_ListCtrl2.CreateDragImage(m_nDragIndex, &m_ptDropPoint);
	m_pDragImage->BeginDrag(0, CPoint(0, 0));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMLV->ptAction);

	SetCapture();
	*pResult = 0;
}

/// <summary>
/// Drag 하는 도중 메서드
/// </summary>
/// <param name="nFlags"></param>
/// <param name="point"></param>
void CMFCApplication1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		CPoint pt(point);
		ClientToScreen(&pt);
		m_pDragImage->DragMove(pt);
		m_pDragImage->DragShowNolock(FALSE);

		// 드롭할 윈도우 탐색
		CWnd* pDropWnd = WindowFromPoint(pt);
		if (pDropWnd != m_pDropWnd)
		{
			if (m_pDropWnd != nullptr)
			{
				m_pDragImage->DragLeave(m_pDropWnd);
			}
			m_pDropWnd = pDropWnd;
			m_pDragImage->DragEnter(m_pDropWnd, pt);
		}
		m_pDragImage->DragShowNolock(TRUE);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

/// <summary>
/// 드레그 시작했을 때의 항목 추출
/// </summary>
/// <param name="nFlags"></param>
/// <param name="point"></param>
void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	CWnd* pWnd = GetFocus();

	CString msg;
	msg.Format(_T("Focused Control: %p"), pWnd);
	// List Control에서의 클릭인지 확인
	if (pWnd == &m_ListCtrl || pWnd == &m_ListCtrl2)
	{
		LVHITTESTINFO hitTestInfo;
		hitTestInfo.pt = point;
		pWnd->ScreenToClient(&hitTestInfo.pt);
		int nItem = m_ListCtrl.HitTest(&hitTestInfo);
		
		if (nItem != -1) // -1은 항목이 없음을 의미
		{
			m_nDragIndex = nItem; // 드래그 시작한 항목의 인덱스 저장
			m_bDragging = TRUE;

			// 드래그 이미지를 생성하고 드래그를 시작합니다.
			m_pDragImage = m_ListCtrl.CreateDragImage(nItem, &point);
			m_pDragImage->BeginDrag(0, CPoint(0, 0));
			m_pDragImage->DragEnter(GetDesktopWindow(), point);
			::SetCapture(m_hWnd);
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}



/// <summary>
/// Drop 했을 때 처리 메서드
/// </summary>
/// <param name="nFlags"></param>
/// <param name="point"></param>
void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		m_bDragging = FALSE;
		::ReleaseCapture();
		m_pDragImage->EndDrag();
		delete m_pDragImage;
		m_pDragImage = nullptr;

		// 드래그된 항목의 텍스트 얻기
		CString draggedItemText = m_ListCtrl.GetItemText(m_nDragIndex, 1);
		CString draggedItemText2 = m_ListCtrl2.GetItemText(m_nDragIndex, 1);

		// 드롭할 윈도우 탐색
		ClientToScreen(&point);
		CWnd* pDropWnd = WindowFromPoint(point);


		// pDropWnd가 m_ListCtrl2를 가리키는지 확인
		if (pDropWnd->GetSafeHwnd() == m_ListCtrl2.GetSafeHwnd()) {
			if (draggedItemText == "File") {
				// List1에서 List2로 드래그 -> 파일 업로드
				UploadFileToFtp(m_nDragIndex);
			}
			else if (draggedItemText == "Folder") {
				// 폴더 업로드
				UploadFolderFromFtp(m_nDragIndex);
			}
		}
		// pDropWnd가 m_ListCtrl을 가리키는지 확인
		else if (pDropWnd->GetSafeHwnd() == m_ListCtrl.GetSafeHwnd()) {
			if (draggedItemText2 == "File") {
				// List2에서 List1로 드래그 -> 파일 다운로드
				DownloadFileFromFtp(m_nDragIndex);
			}
			else if (draggedItemText2 == "Folder") {
				// 폴더 다운로드
				DownloadFolderFromFtp(m_nDragIndex);
			}
		}
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}



////////////////////////////////////////////////////// 업로드/다운로드 메서드 /////////////////////////////////////////////////////////////////////////////

/// <summary>
/// 파일 업로드 메서드
/// </summary>
/// <param name="nIndex"></param>
void CMFCApplication1Dlg::UploadFileToFtp(int nIndex)
{
	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;

	HTREEITEM hSelectedItem = m_TreeCtrl.GetSelectedItem();
	if (hSelectedItem == NULL) {
		return;
	}

	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);
	CString strLocalFilePath = GetFullPathFromTreeItem(hSelectedItem) + _T("\\") + m_ListCtrl.GetItemText(nIndex, 0);
	CString strRemoteFilePath = GetSelectedFtpPath() + _T("/") + m_ListCtrl.GetItemText(nIndex, 0);

	// 프로그래스 바 초기화
	CProgressCtrl* pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgressCtrl->SetRange(0, 100);
	pProgressCtrl->SetPos(0);

	// 로컬 파일 열기
	CFile localFile;
	if (!localFile.Open(strLocalFilePath, CFile::modeRead | CFile::typeBinary)) {
		AfxMessageBox(_T("로컬 파일을 열 수 없습니다."));
		return;
	}

	// 파일 크기 가져오기
	DWORD dwFileSize = (DWORD)localFile.GetLength();

	// 파일 크기가 0인 경우
	if (dwFileSize == 0) {
		// 빈 파일이라도 원격 파일을 열어야 함
		CInternetFile* remoteFile = ftpConnection->OpenFile(strRemoteFilePath, GENERIC_WRITE, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_TRANSFER_BINARY);

		if (!remoteFile) {
			AfxMessageBox(_T("원격 파일을 열 수 없습니다."));
			localFile.Close();
			return;
		}

		// 프로그래스 바를 100%로 설정
		pProgressCtrl->SetPos(100);
		AppendTextToEditControl(GetFormattedCurrentTime() + m_ListCtrl.GetItemText(nIndex, 0) + _T(" 파일 업로드 완료!\n"));
		// 파일 닫기
		remoteFile->Close();
		delete remoteFile;
	}
	else {
		// 파일이 0바이트가 아닌 경우
		DWORD dwBytesRead = 0;
		DWORD dwTotalBytesRead = 0;
		const int bufferSize = 4096;  // 4KB 버퍼
		byte buffer[bufferSize];

		// 원격 파일 열기 (쓰기 모드)
		CInternetFile* remoteFile = ftpConnection->OpenFile(strRemoteFilePath, GENERIC_WRITE, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_TRANSFER_BINARY);

		if (!remoteFile) {
			AfxMessageBox(_T("원격 파일을 열 수 없습니다."));
			localFile.Close();
			return;
		}

		// 파일을 청크 단위로 읽어 전송하면서 프로그래스 바 업데이트
		while ((dwBytesRead = localFile.Read(buffer, bufferSize)) > 0)
		{
			remoteFile->Write(buffer, dwBytesRead);
			dwTotalBytesRead += dwBytesRead;

			// 프로그래스 바 업데이트
			int nProgress = (int)(((double)dwTotalBytesRead / dwFileSize) * 100.0);
			pProgressCtrl->SetPos(nProgress);
		}

		// 파일 닫기
		localFile.Close();
		remoteFile->Close();
		delete remoteFile;
		m_Logtext.SetWindowText(GetFormattedCurrentTime() + m_ListCtrl.GetItemText(nIndex, 0) + _T(" 파일 업로드 완료!\n"));
	}
}


/// <summary>
/// 파일 다운로드 메서드
/// </summary>
/// <param name="nIndex"></param>
void CMFCApplication1Dlg::DownloadFileFromFtp(int nIndex)
{
	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;

	CString strLocalPath;
	m_localPath.GetWindowText(strLocalPath);

	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);
	CString strRemoteFilePath = GetSelectedFtpPath() + _T("/") + m_ListCtrl2.GetItemText(nIndex, 0);
	CString strLocalFilePath = strLocalPath + _T("\\") + m_ListCtrl2.GetItemText(nIndex, 0);

	// 프로그래스 바 초기화
	CProgressCtrl* pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgressCtrl->SetRange(0, 100);
	pProgressCtrl->SetPos(0);

	// 원격 파일 열기
	CInternetFile* remoteFile = nullptr;
	try {
		remoteFile = ftpConnection->OpenFile(strRemoteFilePath, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_TRANSFER_BINARY);

		if (!remoteFile) {
			AfxMessageBox(_T("원격 파일을 열 수 없습니다."));
			return;
		}

		// 로컬 파일 열기
		CFile localFile;
		if (!localFile.Open(strLocalFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
			AfxMessageBox(_T("로컬 파일을 열 수 없습니다."));
			remoteFile->Close();
			delete remoteFile;
			return;
		}

		// 원격 파일 크기 가져오기
		DWORD dwFileSize = (DWORD)remoteFile->GetLength();

		if (dwFileSize == 0) {
			// 파일 크기가 0일 때의 경우
			
			localFile.Close();
			remoteFile->Close();
			delete remoteFile;
			pProgressCtrl->SetPos(100); // 프로그래스 바를 100%로 설정
			AppendTextToEditControl(GetFormattedCurrentTime() + m_ListCtrl2.GetItemText(nIndex, 0) + _T(" 파일 다운로드 완료!\n"));
			return;
		}

		DWORD dwBytesRead = 0;
		DWORD dwTotalBytesRead = 0;
		const int bufferSize = 4096;  // 4KB 버퍼
		byte buffer[bufferSize];

		// 파일을 청크 단위로 읽어 저장하면서 프로그래스 바 업데이트
		while ((dwBytesRead = remoteFile->Read(buffer, bufferSize)) > 0)
		{
			localFile.Write(buffer, dwBytesRead);
			dwTotalBytesRead += dwBytesRead;

			// 프로그래스 바 업데이트
			int nProgress = (int)(((double)dwTotalBytesRead / dwFileSize) * 100.0);
			pProgressCtrl->SetPos(nProgress);
		}

		// 파일 닫기
		localFile.Close();
		remoteFile->Close();
		delete remoteFile;

		
		pProgressCtrl->SetPos(100); // 프로그래스 바를 100%로 설정
		AppendTextToEditControl(GetFormattedCurrentTime() + m_ListCtrl2.GetItemText(nIndex, 0) + _T(" 파일 다운로드 완료!\n"));
		// 트리 컨트롤에 파일 추가
		HTREEITEM hSelectedItem = m_TreeCtrl.GetSelectedItem();
		if (hSelectedItem != NULL) {
			m_TreeCtrl.InsertItem(m_ListCtrl2.GetItemText(nIndex, 0), hSelectedItem);
			m_TreeCtrl.Expand(hSelectedItem, TVE_EXPAND); // 부모 노드를 확장하여 새 파일이 보이도록 함
		}
	}

	catch (CInternetException* pEx) {
		TCHAR szError[1024];
		pEx->GetErrorMessage(szError, 1024);
		CString strError = _T("인터넷 오류: ") + CString(szError);
		AfxMessageBox(strError);
		pEx->Delete();
	}
}


/// <summary>
/// 선택된 FTP 아이템 경로 메서드
/// </summary>
/// <returns></returns>
CString CMFCApplication1Dlg::GetSelectedFtpPath()
{
	HTREEITEM hSelectedItem = m_TreeCtrl2.GetSelectedItem();
	return GetFullPathFromTreeItem2(hSelectedItem);
}

/// <summary>
/// 폴더 업로드 메서드
/// </summary>
/// <param name="nIndex"></param>
void CMFCApplication1Dlg::UploadFolderFromFtp(int nIndex)
{
	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;

	CString strLocalPath;
	m_localPath.GetWindowText(strLocalPath);

	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);
	CString strRemoteFilePath = GetSelectedFtpPath() + _T("/") + m_ListCtrl.GetItemText(nIndex, 0);
	CString strLocalFilePath = strLocalPath + _T("\\") + m_ListCtrl.GetItemText(nIndex, 0);

	// 폴더 생성
	BOOL result = ftpConnection->CreateDirectory(strRemoteFilePath);
	if (!result)
	{
		AfxMessageBox(_T("같은 폴더가 존재합니다."));
		return;
	}

	// 총 파일 개수를 계산
	g_totalFiles = CountFilesInFolder(strLocalFilePath);
	g_uploadedFiles = 0; // 초기화

	// 프로그래스 바 초기화
	CProgressCtrl* pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgressCtrl->SetRange(0, 100);
	pProgressCtrl->SetPos(0);

	// 로컬 폴더 내의 파일 및 폴더를 재귀적으로 FTP 서버로 업로드
	UploadFolderContents(strLocalFilePath, strRemoteFilePath, ftpConnection, pProgressCtrl);
	AppendTextToEditControl(GetFormattedCurrentTime() + m_ListCtrl.GetItemText(nIndex, 0) + _T(" 폴더 업로드 완료!\n"));

}

/// <summary>
/// 업로드 폴더 갯수 반환 메서드
/// </summary>
/// <param name="strLocalFolderPath"></param>
/// <returns></returns>
int CMFCApplication1Dlg::CountFilesInFolder(const CString& strLocalFolderPath)
{
	CFileFind finder;
	CString strSearchPath = strLocalFolderPath + _T("\\*.*");
	BOOL bWorking = finder.FindFile(strSearchPath);
	int fileCount = 0;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) // . and .. directories
			continue;

		if (finder.IsDirectory())
		{
			// 서브 폴더의 파일 개수도 포함
			fileCount += CountFilesInFolder(strLocalFolderPath + _T("\\") + finder.GetFileName());
		}
		else
		{
			fileCount++;
		}
	}

	return fileCount;
}

/// <summary>
/// 하위 디렉토리 업로드 메서드
/// </summary>
/// <param name="strLocalFolderPath"></param>
/// <param name="strRemoteFolderPath"></param>
/// <param name="pFtpConnection"></param>
/// <param name="pProgressCtrl"></param>
void CMFCApplication1Dlg::UploadFolderContents(const CString& strLocalFolderPath, const CString& strRemoteFolderPath, CFtpConnection* pFtpConnection, CProgressCtrl* pProgressCtrl)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strLocalFolderPath + _T("\\*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) // . and .. directories
			continue;

		CString strFileName = finder.GetFileName();
		CString strLocalFilePath = strLocalFolderPath + _T("\\") + strFileName;
		CString strRemoteFilePath = strRemoteFolderPath + _T("/") + strFileName;

		if (finder.IsDirectory())
		{
			// 서브 폴더가 발견되면, 서버에 폴더를 생성하고 재귀 호출
			if (!pFtpConnection->CreateDirectory(strRemoteFilePath))
			{
				AfxMessageBox(_T("Failed to create directory on FTP server: ") + strRemoteFilePath);
				continue;
			}

			// 재귀 호출
			UploadFolderContents(strLocalFilePath, strRemoteFilePath, pFtpConnection, pProgressCtrl);
		}
		else
		{
			// 파일을 발견하면 FTP 서버로 업로드
			if (!pFtpConnection->PutFile(strLocalFilePath, strRemoteFilePath))
			{
				AfxMessageBox(_T("Failed to upload file: ") + strLocalFilePath);
			}

			// 업로드된 파일 수를 증가시키고 프로그래스 바 업데이트
			g_uploadedFiles++;
			int nProgress = (int)(((double)g_uploadedFiles / g_totalFiles) * 100.0);
			pProgressCtrl->SetPos(nProgress);
		}
	}
}

/// <summary>
/// 폴더 다운로드 메서드
/// </summary>
/// <param name="nIndex"></param>
void CMFCApplication1Dlg::DownloadFolderFromFtp(int nIndex)
{
	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;

	CString strLocalPath;
	m_localPath.GetWindowText(strLocalPath);

	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);
	CString strRemoteFilePath = GetSelectedFtpPath() + _T("/") + m_ListCtrl2.GetItemText(nIndex, 0);
	CString strLocalFilePath = strLocalPath + _T("\\") + m_ListCtrl2.GetItemText(nIndex, 0);

	// 폴더 생성
	BOOL result = CreateDirectory(strLocalFilePath, NULL);
	if (!result)
	{
		AfxMessageBox(_T("같은 폴더가 존재합니다."));
		return;
	}

	// 총 파일 개수를 계산
	g_totalFiles = CountFilesInFolderOnFtp(ftpConnection, strRemoteFilePath);
	g_downloadedFiles = 0; // 초기화

	// 프로그래스 바 초기화
	CProgressCtrl* pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgressCtrl->SetRange(0, 100);
	pProgressCtrl->SetPos(0);

	// 로컬 폴더 내의 파일 및 폴더를 재귀적으로 FTP 서버로 다운로드
	DownloadFolderContents(strLocalFilePath, strRemoteFilePath, ftpConnection, pProgressCtrl);

	AppendTextToEditControl(GetFormattedCurrentTime() + m_ListCtrl2.GetItemText(nIndex, 0) + _T(" 폴더 다운로드 완료!\n"));
}

/// <summary>
/// 다운로드 폴더 갯수 반환 메서드
/// </summary>
/// <param name="pFtpConnection"></param>
/// <param name="strRemoteFolderPath"></param>
/// <returns></returns>
int CMFCApplication1Dlg::CountFilesInFolderOnFtp(CFtpConnection* pFtpConnection, const CString& strRemoteFolderPath)
{
	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;

	CString strLocalPath;
	m_localPath.GetWindowText(strLocalPath);

	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);
	CFtpFileFind fileFind(ftpConnection);
	CString strSearchPath = strRemoteFolderPath + _T("/*.*");
	BOOL bWorking = fileFind.FindFile(strSearchPath);
	int fileCount = 0;

	while (bWorking)
	{
		bWorking = fileFind.FindNextFile();
		if (fileFind.IsDots()) // . and .. directories
			continue;

		if (fileFind.IsDirectory())
		{
			// 서브 폴더의 파일 개수도 포함
			fileCount += CountFilesInFolderOnFtp(ftpConnection, strRemoteFolderPath + _T("/") + fileFind.GetFileName());
		}
		else
		{
			fileCount++;
		}
	}

	return fileCount;
}

/// <summary>
/// 다운로드 하위 디렉토리 반환 메서드
/// </summary>
/// <param name="strLocalFolderPath"></param>
/// <param name="strRemoteFolderPath"></param>
/// <param name="pFtpConnection"></param>
/// <param name="pProgressCtrl"></param>
void CMFCApplication1Dlg::DownloadFolderContents(const CString& strLocalFolderPath, const CString& strRemoteFolderPath, CFtpConnection* pFtpConnection, CProgressCtrl* pProgressCtrl)
{
	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;

	CString strLocalPath;
	m_localPath.GetWindowText(strLocalPath);

	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);
	CFtpFileFind fileFind(ftpConnection);
	BOOL bWorking = fileFind.FindFile(strRemoteFolderPath + _T("\\*.*"));
	while (bWorking)
	{
		bWorking = fileFind.FindNextFile();
		if (fileFind.IsDots()) // . and .. directories
			continue;

		CString strFileName = fileFind.GetFileName();
		CString strLocalFilePath = strLocalFolderPath + _T("\\") + strFileName;
		CString strRemoteFilePath = strRemoteFolderPath + _T("/") + strFileName;

		if (fileFind.IsDirectory())
		{
			// 서브 폴더가 발견되면, 로컬 폴더를 생성하고 재귀 호출
			CreateDirectory(strLocalFilePath, NULL);
			DownloadFolderContents(strLocalFilePath, strRemoteFilePath, ftpConnection, pProgressCtrl);
		}
		else
		{
			// 파일을 발견하면 FTP 서버에서 로컬로 다운로드
			if (!ftpConnection->GetFile(strRemoteFilePath, strLocalFilePath))
			{
				AfxMessageBox(_T("Failed to download file: ") + strRemoteFilePath);
			}

			// 다운로드된 파일 수를 증가시키고 프로그래스 바 업데이트
			g_downloadedFiles++;
			int nProgress = (int)(((double)g_downloadedFiles / g_totalFiles) * 100.0);
			pProgressCtrl->SetPos(nProgress);
		}
	}
}

/// <summary>
/// 하위 디렉토리 목록 모두 삭제 메서드
/// </summary>
/// <param name="pFtpConnection"></param>
/// <param name="strPath"></param>
void CMFCApplication1Dlg::ClearFtpDirectory(CFtpConnection* pFtpConnection, const CString& strPath, bool bKeepRoot = false)
{
	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;


	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);
	CString strSearchPath = strPath;
	if (strSearchPath.Right(1) != _T("/"))
		strSearchPath += _T("/");

	// FTP 파일 찾기 객체 생성
	CFtpFileFind finder(ftpConnection);
	BOOL bWorking = finder.FindFile(strSearchPath + _T("*"));

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// "."과 ".."을 무시
		if (finder.IsDots())
			continue;

		CString strFileName = finder.GetFileName();
		CString strFilePath = strSearchPath + strFileName;

		if (finder.IsDirectory())
		{
			// 하위 디렉토리 비우기
			ClearFtpDirectory(ftpConnection, strFilePath, false);

			// 빈 디렉토리 삭제 시도
			if (!bKeepRoot)
			{
				// 빈 디렉토리 삭제
				if (ftpConnection->RemoveDirectory(strFilePath))
				{
					TRACE(_T("Successfully removed directory: %s\n"), strFilePath);
				}
				else
				{
					TRACE(_T("Failed to remove directory: %s\n"), strFilePath);
				}
			}
		}
		else
		{
			// 파일인 경우, 삭제
			if (ftpConnection->Remove(strFilePath))
			{
				TRACE(_T("Successfully deleted file: %s\n"), strFilePath);
			}
			else
			{
				TRACE(_T("Failed to delete file: %s\n"), strFilePath);
			}
		}
	}
	finder.Close();

	// 빈 디렉토리 삭제 시도 (이때까지 하위 항목이 모두 삭제된 상태)
	if (!bKeepRoot)
	{
		// 빈 디렉토리 삭제
		if (ftpConnection->RemoveDirectory(strPath))
		{
			TRACE(_T("Successfully removed directory: %s\n"), strPath);
		}
		else
		{
			TRACE(_T("Failed to remove directory: %s\n"), strPath);
		}
	}
}

void CMFCApplication1Dlg::UploadLocalDirectory(CFtpConnection* pFtpConnection, const CString& strLocalPath, const CString& strFtpPath)
{
	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;


	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);
	CFileFind finder;
	CString strWildcard(strLocalPath);
	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// "."과 ".."을 무시
		if (finder.IsDots())
			continue;

		CString strFilePath = finder.GetFilePath();
		CString strFileName = finder.GetFileName();
		CString strFtpFilePath = strFtpPath + _T("/") + strFileName;

		if (finder.IsDirectory())
		{
			// 디렉토리인 경우, FTP 서버에 디렉토리 생성하고 재귀적으로 하위 디렉토리 업로드
			if (!ftpConnection->CreateDirectory(strFtpFilePath))
			{
				TRACE(_T("Failed to create directory: %s\n"), strFtpFilePath);
			}
			UploadLocalDirectory(ftpConnection, strFilePath, strFtpFilePath);
		}
		else
		{
			// 파일인 경우, FTP 서버에 파일 업로드
			if (!ftpConnection->PutFile(strFilePath, strFtpFilePath))
			{
				TRACE(_T("Failed to upload file: %s\n"), strFilePath);
			}
		}
	}
	finder.Close();
}

/// <summary>
/// 로컬 -> 서버 동기화 메서드
/// </summary>
void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// FTP 서버에 연결
	CInternetSession internetSession;
	CFtpConnection* ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);

	// 현재 FTP 디렉토리 경로
	CString strFtpCurrentPath = GetFullPathFromTreeItem2(m_TreeCtrl2.GetSelectedItem());
	// 로컬 디렉토리 경로
	CString strLocalPath = GetFullPathFromTreeItem(m_TreeCtrl.GetSelectedItem());

	HTREEITEM hSelectedItem = m_TreeCtrl2.GetSelectedItem();
	if (hSelectedItem == NULL)
	{
		AfxMessageBox(_T("트리에서 업데이트할 디렉토리를 선택하세요."));
		return;
	}

	// 파일 수 계산 및 Progress Bar 초기화
	int nTotalFiles = CountFilesInDirectory(strLocalPath);
	CProgressCtrl* pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgressCtrl->SetRange(0, nTotalFiles);
	pProgressCtrl->SetPos(0);

	// 현재 FTP 디렉토리 비우기
	ClearFtpDirectory(ftpConnection, strFtpCurrentPath, true);

	// 로컬 디렉토리 업로드
	UploadLocalDirectoryWithProgress(ftpConnection, strLocalPath, strFtpCurrentPath, pProgressCtrl);

	// FTP Tree Control 갱신
	DeleteAllChildItems(m_TreeCtrl2, hSelectedItem);
	LoadFTPDirectoryStructure(ftpConnection, strFtpCurrentPath, hSelectedItem);
	AppendTextToEditControl(GetFormattedCurrentTime() + _T(" 폴더 동기화 완료! (client -> server)\n"));
	AfxMessageBox(_T("노드를 한번 클릭해 List Control을 업데이트 시켜주세요!"));
}

void CMFCApplication1Dlg::UploadLocalDirectoryWithProgress(CFtpConnection* pFtpConnection, const CString& strLocalPath, const CString& strFtpPath, CProgressCtrl* pProgressCtrl)
{
	CFileFind finder;
	CString strWildcard(strLocalPath);
	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		CString strFilePath = finder.GetFilePath();
		CString strFileName = finder.GetFileName();
		CString strFtpFilePath = strFtpPath + _T("/") + strFileName;

		if (finder.IsDirectory())
		{
			if (!pFtpConnection->CreateDirectory(strFtpFilePath))
			{
				TRACE(_T("Failed to create directory: %s\n"), strFtpFilePath);
			}
			UploadLocalDirectoryWithProgress(pFtpConnection, strFilePath, strFtpFilePath, pProgressCtrl);
		}
		else
		{
			if (!pFtpConnection->PutFile(strFilePath, strFtpFilePath))
			{
				TRACE(_T("Failed to upload file: %s\n"), strFilePath);
			}

			// Progress Bar 업데이트
			pProgressCtrl->SetPos(pProgressCtrl->GetPos() + 1);
		}
	}
	finder.Close();
}

int CMFCApplication1Dlg::CountFilesInDirectory(const CString& strPath)
{
	int nCount = 0;
	CFileFind finder;
	CString strWildcard(strPath);
	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			nCount += CountFilesInDirectory(finder.GetFilePath());
		}
		else
		{
			nCount++;
		}
	}
	finder.Close();

	return nCount;
}

void CMFCApplication1Dlg::ClearLocalDirectory(const CString& strPath)
{

	CFileFind finder;
	CString strWildcard = strPath + _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// "." 및 ".."을 무시
		if (finder.IsDots())
			continue;

		CString strFilePath = finder.GetFilePath();
		if (finder.IsDirectory())
		{
			// 하위 디렉토리 비우기
			ClearLocalDirectory(strFilePath);

			// 빈 디렉토리 삭제 시도
			if (RemoveDirectory(strFilePath))
			{
				TRACE(_T("Successfully removed directory: %s\n"), strFilePath);
			}
			else
			{
				TRACE(_T("Failed to remove directory: %s\n"), strFilePath);
			}
		}
		else
		{
			// 파일인 경우, 삭제
			if (DeleteFile(strFilePath))
			{
				TRACE(_T("Successfully deleted file: %s\n"), strFilePath);
			}
			else
			{
				TRACE(_T("Failed to delete file: %s\n"), strFilePath);
			}
		}
	}
	finder.Close();
}

void CMFCApplication1Dlg::DownloadFtpDirectory(CFtpConnection* pFtpConnection, const CString& strFtpPath, const CString& strLocalPath)
{
	// 인터넷 세션 객체 생성
	CInternetSession internetSession;

	// FTP 서버에 연결
	CFtpConnection* ftpConnection = nullptr;


	ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);
	CString strSearchPath = strFtpPath;
	if (strSearchPath.Right(1) != _T("/"))
		strSearchPath += _T("/");

	// FTP 파일 찾기 객체 생성
	CFtpFileFind finder(ftpConnection);
	BOOL bWorking = finder.FindFile(strSearchPath + _T("*"));

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// "."과 ".."을 무시
		if (finder.IsDots())
			continue;

		CString strFileName = finder.GetFileName();
		CString strFtpFilePath = strSearchPath + strFileName;
		CString strLocalFilePath = strLocalPath + _T("\\") + strFileName;

		if (finder.IsDirectory())
		{
			// 로컬 디렉토리 생성
			CreateDirectory(strLocalFilePath, NULL);

			// 하위 디렉토리 다운로드
			DownloadFtpDirectory(ftpConnection, strFtpFilePath, strLocalFilePath);
		}
		else
		{
			// 파일 다운로드
			CFile localFile;
			if (localFile.Open(strLocalFilePath, CFile::modeCreate | CFile::modeWrite))
			{
				// FTP 파일 다운로드
				if (ftpConnection->GetFile(strFtpFilePath, strLocalFilePath))
				{
					TRACE(_T("Successfully downloaded file: %s\n"), strFtpFilePath);
				}
				else
				{
					TRACE(_T("Failed to download file: %s\n"), strFtpFilePath);
				}
				localFile.Close();
			}
		}
	}
	finder.Close();
}

void CMFCApplication1Dlg::DownloadFtpDirectoryWithProgress(CFtpConnection* pFtpConnection, const CString& strFtpPath, const CString& strLocalPath, CProgressCtrl* pProgressCtrl)
{
	CFtpFileFind finder(pFtpConnection);
	CString strSearchPath = strFtpPath;
	if (strSearchPath.Right(1) != _T("/"))
		strSearchPath += _T("/");

	BOOL bWorking = finder.FindFile(strSearchPath + _T("*"));

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		CString strFileName = finder.GetFileName();
		CString strFtpFilePath = strSearchPath + strFileName;
		CString strLocalFilePath = strLocalPath + _T("\\") + strFileName;

		if (finder.IsDirectory())
		{
			CreateDirectory(strLocalFilePath, NULL);
			DownloadFtpDirectoryWithProgress(pFtpConnection, strFtpFilePath, strLocalFilePath, pProgressCtrl);
		}
		else
		{
			if (!pFtpConnection->GetFile(strFtpFilePath, strLocalFilePath))
			{
				TRACE(_T("Failed to download file: %s\n"), strFtpFilePath);
			}

			// Progress Bar 업데이트
			pProgressCtrl->SetPos(pProgressCtrl->GetPos() + 1);
		}
	}
	finder.Close();
}


/// <summary>
/// 서버 -> 로컬 동기화 메서드
/// </summary>
void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// FTP 서버에 연결
	CInternetSession internetSession;
	CFtpConnection* ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);

	CString strFtpCurrentPath = GetFullPathFromTreeItem2(m_TreeCtrl2.GetSelectedItem());
	CString strLocalPath = GetFullPathFromTreeItem(m_TreeCtrl.GetSelectedItem());

	HTREEITEM hSelectedItem = m_TreeCtrl.GetSelectedItem();
	if (hSelectedItem == NULL)
	{
		AfxMessageBox(_T("트리에서 업데이트할 디렉토리를 선택하세요."));
		return;
	}

	// 파일 수 계산 및 Progress Bar 초기화
	int nTotalFiles = CountFilesInFtpDirectory(ftpConnection, strFtpCurrentPath);
	CProgressCtrl* pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	pProgressCtrl->SetRange(0, nTotalFiles);
	pProgressCtrl->SetPos(0);

	ClearLocalDirectory(strLocalPath);
	DownloadFtpDirectoryWithProgress(ftpConnection, strFtpCurrentPath, strLocalPath, pProgressCtrl);

	DeleteAllChildItems(m_TreeCtrl, hSelectedItem);
	LoadDirectoryStructure(strLocalPath, hSelectedItem);

	AppendTextToEditControl(GetFormattedCurrentTime() + _T(" 폴더 동기화 완료! (server -> client)\n"));
	AfxMessageBox(_T("노드를 한번 클릭해 List Control을 업데이트 시켜주세요!"));
}

int CMFCApplication1Dlg::CountFilesInFtpDirectory(CFtpConnection* pFtpConnection, const CString& strFtpPath)
{
	int nCount = 0;
	CFtpFileFind finder(pFtpConnection);
	CString strSearchPath = strFtpPath;
	if (strSearchPath.Right(1) != _T("/"))
		strSearchPath += _T("/");

	BOOL bWorking = finder.FindFile(strSearchPath + _T("*"));

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			nCount += CountFilesInFtpDirectory(pFtpConnection, finder.GetFilePath());
		}
		else
		{
			nCount++;
		}
	}
	finder.Close();

	return nCount;
}

// 자식 노드를 모두 삭제하는 함수
void CMFCApplication1Dlg::DeleteAllChildItems(CTreeCtrl& treeCtrl, HTREEITEM hParentItem)
{
	HTREEITEM hChildItem = treeCtrl.GetChildItem(hParentItem);

	while (hChildItem != NULL)
	{
		HTREEITEM hNextItem = treeCtrl.GetNextSiblingItem(hChildItem);
		treeCtrl.DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}
}
