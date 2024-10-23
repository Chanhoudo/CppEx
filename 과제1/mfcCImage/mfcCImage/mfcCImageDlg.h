// mfcCImageDlg.h: 헤더 파일

#pragma once
#include <vector>
class CmfcCImageDlg : public CDialogEx
{
    // 생성입니다.
public:
    CmfcCImageDlg(CWnd* pParent = nullptr);    // 표준 생성자입니다.

    // 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFCCIMAGE_DIALOG };
#endif
public:
    afx_msg void OnEnChangeEditX1();  // X1 Edit Control의 변경 이벤트
    afx_msg void OnEnChangeEditY1();  // Y1 Edit Control의 변경 이벤트
    afx_msg void OnEnChangeEditX2();  // X2 Edit Control의 변경 이벤트
    afx_msg void OnEnChangeEditY2();  // Y2 Edit Control의 변경 이벤트
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    // 구현입니다.
protected:
    HICON m_hIcon;

    // 이미지 처리 관련 멤버 변수
    CImage m_image; // 다이얼로그에 표시할 이미지 객체

    // 좌표 저장
    int m_x1, m_y1, m_x2, m_y2;
    // 좌표 입력을 받는 텍스트
    CEdit m_editX1, m_editY1, m_editX2, m_editY2;
    virtual BOOL OnInitDialog();                // OnInitDialog 함수 선언
    CStatic m_drawArea;                         // 그림을 그릴 네모 영역
    CBrush m_brushWhite;                        // 배경을 흰색으로 만들기 위한 CBrush 객체 
    void DrawRandomCircle(int x, int y);        // 특정 좌표에서 랜덤한 크기로 원 그리기 함수
    CString m_FolderPath;                   // 이미지 저장 경로

    void SaveImage(const CString& folderPath);  // 이미지를 저장하는 함수

    void DrawCrossAndCoordinates(CDC* pDC, int x, int y);   // X 표시와 좌표를 그리는 함수
    void CreateDirectories();       // X 표시와 좌표를 그리는 함수
    void ShowAllImages(const std::vector<CString>& imagePaths); // 저장된 모든 이미지를 보여주는 함수
    DECLARE_MESSAGE_MAP()
public:
    // 버튼 클릭 핸들러
    afx_msg void OnBnClickedBtnDraw();   // Draw 버튼 클릭 시
    afx_msg void OnBnClickedBtnAction(); // Action 버튼 클릭 시
    afx_msg void OnBnClickedBtnOpen();   // Open 버튼 클릭 시
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);  // 배경 색상 변경
    afx_msg void OnStnClickedStaticDrawArea();
    afx_msg void OnBnClickedBtnReset();
    afx_msg void DrawFixedCircle(int x, int y, int radius, CBrush& brush);
};
