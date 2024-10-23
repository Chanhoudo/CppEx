#pragma once

#include "afxdialogex.h"
#include "resource.h"
class CProgressDlg : public CDialogEx
{
public:
    CProgressDlg(CWnd* pParent = nullptr);   // 표준 생성자

    enum { IDD = IDD_PROGRESS_DIALOG }; // 리소스 ID와 일치해야 함

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    DECLARE_MESSAGE_MAP()
public:
    CStatic m_staticMessage;  // 메시지를 표시하는 Static 컨트롤
};
