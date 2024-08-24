﻿
// MFCApplication1Dlg.cpp: 구현 파일
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
	, m_editIP(_T("192.168.0.5"))
	, m_editID(_T("FTP_user"))
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
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST2, &CMFCApplication1Dlg::OnLvnBegindragList2)
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
	CString strInitialPath = _T("C:\\Test");
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

		AfxMessageBox(_T("FTP 연결이 해제되었습니다."));
	}
	else
	{
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
	strPath = _T("C:\\Test") + strPath;

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

	// 스택을 사용하여 디렉토리 탐색
	std::stack<CString> directories;
	directories.push(strSelectedPath);

	int nIndex = 0;

	while (!directories.empty())
	{
		CString currentPath = directories.top();
		directories.pop();

		// 인터넷 세션 객체 생성
		CInternetSession internetSession;

		// FTP 서버에 연결
		CFtpConnection* ftpConnection = nullptr;

		ftpConnection = internetSession.GetFtpConnection(m_editIP, m_editID, m_editPW);

		// 파일 찾기 객체 생성
		CFtpFileFind fileFind(ftpConnection);


		CString strSearchPath = currentPath;

		BOOL bWorking = fileFind.FindFile(strSearchPath + _T("\\*.*"));

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

			// 디렉토리인 경우 스택에 추가
			if (bIsDirectory)
			{
				CString strSubDir = currentPath;
				if (strSubDir.Right(1) != _T("/"))
					strSubDir += _T("/");

				strSubDir += strFileName;
				directories.push(strSubDir);
			}
		}
		fileFind.Close();
	}

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

		// 파일인지 폴더인지 확인하는 변수
		CString draggedItemText = m_ListCtrl.GetItemText(m_nDragIndex, 1);
		

		// 드롭할 윈도우 탐색
		ClientToScreen(&point);
		CWnd* pDropWnd = WindowFromPoint(point);
		if (draggedItemText == "File") {
			if (pDropWnd == &m_ListCtrl2)
			{
				// List1에서 List2로 드래그 -> 파일 업로드
				UploadFileToFtp(m_nDragIndex);
			}
			else if (pDropWnd == &m_ListCtrl)
			{
				// List2에서 List1로 드래그 -> 파일 다운로드
				DownloadFileFromFtp(m_nDragIndex);
			}
		}
		else if (draggedItemText == "Folder") {
			if (pDropWnd == &m_ListCtrl2)
			{
				// List1에서 List2로 드래그 -> 폴더 업로드
				UploadFolderFromFtp(m_nDragIndex);
			}
			else if (pDropWnd == &m_ListCtrl)
			{
				// List2에서 List1로 드래그 -> 폴더 다운로드
				DownloadFileFromFtp(m_nDragIndex);
			}
		}
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}



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
	AfxMessageBox(strLocalFilePath);
	AfxMessageBox(strRemoteFilePath);


	if (ftpConnection->PutFile(strLocalFilePath, strRemoteFilePath))
	{
		AfxMessageBox(_T("파일 업로드 성공!"));
	}
	else
	{
		AfxMessageBox(_T("파일 업로드 실패!"));
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

	if (ftpConnection->GetFile(strRemoteFilePath, strLocalFilePath))
	{
		AfxMessageBox(_T("파일 다운로드 성공!"));
	}
	else
	{
		AfxMessageBox(_T("파일 다운로드 실패!"));
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
		AfxMessageBox(_T("폴더가 생성되지 않았습니다."));
		return;
	}
	// 로컬 폴더 내의 파일 및 폴더를 재귀적으로 FTP 서버로 업로드
	UploadFolderContents(strLocalFilePath, strRemoteFilePath, ftpConnection);
	AfxMessageBox(_T("성공적으로 폴더를 업로드 했습니다."));
	
	
}

/// <summary>
/// 재귀 함수를 이요해서 폴더 업로드 메서드
/// </summary>
/// <param name="strLocalFolderPath"></param>
/// <param name="strRemoteFolderPath"></param>
/// <param name="pFtpConnection"></param>
void CMFCApplication1Dlg::UploadFolderContents(const CString& strLocalFolderPath, const CString& strRemoteFolderPath, CFtpConnection* pFtpConnection)
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
			UploadFolderContents(strLocalFilePath, strRemoteFilePath, pFtpConnection);
		}
		else
		{
			// 파일을 발견하면 FTP 서버로 업로드
			if (!pFtpConnection->PutFile(strLocalFilePath, strRemoteFilePath))
			{
				AfxMessageBox(_T("Failed to upload file: ") + strLocalFilePath);
			}
		}
	}
}