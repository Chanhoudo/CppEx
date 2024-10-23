#include "pch.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_PROGRESS_DIALOG, pParent)  // ���ҽ� ID�� ����
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_MESSAGE, m_staticMessage); // Static �޽��� ��Ʈ�� ����
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
END_MESSAGE_MAP()

// ���� �޼��带 �߰��Ͽ� protected ��� ���� ���� �ذ�
CRuntimeClass* CProgressDlg::GetMyRuntimeClass()
{
    return CProgressDlg::GetRuntimeClass(); // �⺻ Ŭ������ protected �޼��� ȣ��
}
