﻿
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
	// Drag & Drop 관련 멤버 변수
	BOOL m_bDragging;
	CImageList* m_pDragImage;
	int m_nDragIndex;
	CWnd* m_pDropWnd;
	CPoint m_ptDropPoint;
	_ConnectionPtr pConn;
	_RecordsetPtr pRecordset;
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

	void AddTextToEditControl(CString newText);
	
	CString m_editIP;	// ip
	CString m_editID;	// id
	CString m_editPW;	// pw
	CTreeCtrl m_TreeCtrl;
	CListCtrl m_ListCtrl;
	CTreeCtrl m_TreeCtrl2;
	CListCtrl m_ListCtrl2;
	CEdit m_localPath;
	CEdit m_remotePath;
	CEdit m_Logtext;
	int g_totalFiles = 0;
	int g_uploadedFiles = 0;
	int g_downloadedFiles = 0;

	
	CFtpConnection* m_pFtpConnection;	// ftp 연결 유무
public:
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg
		//void AppendTextToEditControl(CString newText);
	void AppendTextToEditControl(CString date, CString newText);
	void OnBnClickedButtonConnect();
	void LoadDirectoryStructure(const CString& strPath, HTREEITEM hParentItem);
	void LoadFTPDirectoryStructure(CFtpConnection* pFtpConnection, const CString& strPath, HTREEITEM hParentItem);
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg CString GetFullPathFromTreeItem2(HTREEITEM hItem);
	void OnLvnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult);
	void OnLvnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void UploadFileToFtp(int nIndex);
	void OnLButtonDown(UINT nFlags, CPoint point);

	void DownloadFileFromFtp(int nIndex);
	CString GetSelectedFtpPath();
	void UploadFolderFromFtp(int nIndex);
	int CountFilesInFolder(const CString& strLocalFolderPath);
	void UploadFolderContents(const CString& strLocalFolderPath, const CString& strRemoteFolderPath, CFtpConnection* pFtpConnection, CProgressCtrl* pProgressCtrl);
	void UploadFolderContents(const CString& strLocalFolderPath, const CString& strRemoteFolderPath, CFtpConnection* pFtpConnection);
	void DownloadFolderFromFtp(int nIndex);
	int CountFilesInFolderOnFtp(CFtpConnection* pFtpConnection, const CString& strRemoteFolderPath);
	void DownloadFolderContents(const CString& strLocalFolderPath, const CString& strRemoteFolderPath, CFtpConnection* pFtpConnection, CProgressCtrl* pProgressCtrl);
	void ClearFtpDirectory(CFtpConnection* pFtpConnection, const CString& strPath);
	void ClearFtpDirectory(CFtpConnection* pFtpConnection, const CString& strPath, bool bKeepRoot);
	void UploadLocalDirectory(CFtpConnection* pFtpConnection, const CString& strLocalPath, const CString& strFtpPath);
	void DownloadFolderContents(const CString& strRemoteFolderPath, const CString& strLocalFolderPath, CFtpConnection* pFtpConnection);
	afx_msg void OnTvnSelchangedTree2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg CString GetFullPathFromTreeItem(HTREEITEM hItem);
	
	CProgressCtrl m_ProgressCtrl;
	
	afx_msg void OnBnClickedButton1();
	void UploadLocalDirectoryWithProgress(CFtpConnection* pFtpConnection, const CString& strLocalPath, const CString& strFtpPath, CProgressCtrl* pProgressCtrl);
	int CountFilesInDirectory(const CString& strPath);
	void ClearLocalDirectory(const CString& strPath);
	void DownloadFtpDirectory(CFtpConnection* pFtpConnection, const CString& strFtpPath, const CString& strLocalPath);
	void DownloadFtpDirectoryWithProgress(CFtpConnection* pFtpConnection, const CString& strFtpPath, const CString& strLocalPath, CProgressCtrl* pProgressCtrl);
	afx_msg void OnBnClickedButton2();
	int CountFilesInFtpDirectory(CFtpConnection* pFtpConnection, const CString& strFtpPath);
	void DeleteAllChildItems(CTreeCtrl& treeCtrl, HTREEITEM hParentItem);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButtonHistory();
};
