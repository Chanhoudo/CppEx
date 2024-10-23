#include "pch.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_PROGRESS_DIALOG, pParent)  // 리소스 ID와 연결
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_MESSAGE, m_staticMessage); // Static 메시지 컨트롤 연결
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
END_MESSAGE_MAP()

// 래퍼 메서드를 추가하여 protected 멤버 접근 제어 해결
CRuntimeClass* CProgressDlg::GetMyRuntimeClass()
{
    return CProgressDlg::GetRuntimeClass(); // 기본 클래스의 protected 메서드 호출
}
