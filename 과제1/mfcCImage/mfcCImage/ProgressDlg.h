#pragma once

#include "afxdialogex.h"
#include "resource.h"
class CProgressDlg : public CDialogEx
{
public:
    CProgressDlg(CWnd* pParent = nullptr);   // ǥ�� ������

    enum { IDD = IDD_PROGRESS_DIALOG }; // ���ҽ� ID�� ��ġ�ؾ� ��

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

    DECLARE_MESSAGE_MAP()
public:
    CStatic m_staticMessage;  // �޽����� ǥ���ϴ� Static ��Ʈ��
};
