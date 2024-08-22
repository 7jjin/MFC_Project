
// MFCApplication1Dlg.h: 헤더 파일
//

#pragma once
#include <afxinet.h>
#include <map>
#include <string>
#include <afxdialogex.h>
#include <wininet.h>
#include <iostream>

// FileInfo 구조체 정의
struct FileInfo
{
	BOOL isDir;                  // 디렉토리 여부
	__int64 nFileSize;           // 파일 크기
	SYSTEMTIME stCreattionTime;  // 생성일
	SYSTEMTIME stLastAccessTime; // 최종 접근일
	SYSTEMTIME stLastWriteTime;  // 최종 수정일
	TCHAR szFilePath[MAX_PATH];  // 파일 경로
};

typedef std::map<CString, FileInfo> FILE_LIST;

// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	CString m_editIP;	// ip
	CString m_editID;	// id
	CString m_editPW;	// pw
	CTreeCtrl m_TreeCtrl;
	CListCtrl m_ListCtrl;
	CTreeCtrl m_TreeCtrl2;
	CListCtrl m_ListCtrl2;

	
	CFtpConnection* m_pFtpConnection;	// ftp 연결 유무
public:
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonConnect();
	void LoadDirectoryStructure(const CString& strPath, HTREEITEM hParentItem);
	void LoadFTPDirectoryStructure(CFtpConnection* pFtpConnection, const CString& strPath, HTREEITEM hParentItem);
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg
		CString AddRemoteFile(const CString strCurrentRemoteDir, const WIN32_FIND_DATAA& find_data, FILE_LIST& file_list);
	CString GetFullPathFromTreeItem2(HTREEITEM hItem);
	afx_msg void OnTvnSelchangedTree2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg CString GetFullPathFromTreeItem(HTREEITEM hItem);
};
