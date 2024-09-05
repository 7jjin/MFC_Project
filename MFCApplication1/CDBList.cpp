// CDBList.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CDBList.h"


// CDBList 대화 상자

IMPLEMENT_DYNAMIC(CDBList, CDialogEx)

CDBList::CDBList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DBHISTORY, pParent)
{

}

CDBList::~CDBList()
{
}

void CDBList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_DBList);
}


BEGIN_MESSAGE_MAP(CDBList, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ALL, &CDBList::OnBnClickedButtonDeleteAll)
END_MESSAGE_MAP()


// CDBList 메시지 처리기
BOOL CDBList::OnInitDialog()
{
    CDialogEx::OnInitDialog();

	SetWindowText(_T("Log History"));

    CRect rt;
    m_DBList.GetWindowRect(&rt);
    m_DBList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_DBList.InsertColumn(0, _T("Date"), LVCFMT_LEFT, rt.Width() * 0.3);
    m_DBList.InsertColumn(1, _T("Log"), LVCFMT_LEFT, rt.Width() * 0.7);

	

	// DB 연결
	HRESULT hr = pConn.CreateInstance(__uuidof(Connection));

	if (SUCCEEDED(hr)) {
		try {
			pConn->Open("Provider=SQLOLEDB;Data Source=192.168.0.6;Initial Catalog=bems_db;User Id=bems_user;Password=fhwl123!@#;", "", "", adConnectUnspecified);
		}
		catch (_com_error& e) {
			AfxMessageBox(e.Description());
		}
	}
	else {
		AfxMessageBox(_T("Filed to create connection instance."));
	}

	CString query;
	query.Format(_T("Select * From dbo.LogHistory"));
	pConn->Execute((LPCTSTR)query, NULL, adCmdText);
	pRecordset.CreateInstance(__uuidof(Recordset));

	try {
		pRecordset->Open((_bstr_t)query, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);

		// 데이터가 존재하는 동안 반복
		while (!pRecordset->EndOfFile) {
			// 각 필드를 변수에 담기
			CString Date = (LPCTSTR)(_bstr_t)pRecordset->Fields->GetItem("EventDateTime")->GetValue();
			CString Log = (LPCTSTR)(_bstr_t)pRecordset->Fields->GetItem("LogText")->GetValue();

			// List Control에 아이템 추가
			int nIndex = m_DBList.InsertItem(0, Date); // 첫 번째 열에 ID 추가
			m_DBList.SetItemText(nIndex, 1, Log); // 두 번째 열에 Description 추가

			// 다음 레코드로 이동
			pRecordset->MoveNext();
		}
		pRecordset->Close();
	}
	catch (_com_error& e) {
		AfxMessageBox(e.ErrorMessage());
	}
    return TRUE;
}

void CDBList::OnBnClickedButtonDeleteAll()
{
	try {
		CString query = _T("DELETE FROM dbo.LogHistory");
		pConn->Execute((LPCTSTR)query, NULL, adCmdText);
		m_DBList.DeleteAllItems();
		AfxMessageBox(_T("모든 데이터가 삭제되었습니다."));
	}
	catch (_com_error& e) {
		CString strError;
		strError.Format(_T("Error: %s"), (LPCTSTR)e.Description());
		AfxMessageBox(strError);
	}
}
