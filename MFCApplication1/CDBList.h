#pragma once
#include "afxdialogex.h"
#include "pch.h"


// CDBList 대화 상자

class CDBList : public CDialogEx
{
	DECLARE_DYNAMIC(CDBList)

public:
	CDBList(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDBList();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DBHISTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	BOOL OnInitDialog();
	_ConnectionPtr pConn;
	_RecordsetPtr pRecordset;

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_DBList;
	afx_msg void OnBnClickedButtonDeleteAll();
};
