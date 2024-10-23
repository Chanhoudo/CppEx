// mfcCImageDlg.cpp: 구현 파일

#include "pch.h"
#include "framework.h"
#include "mfcCImage.h"
#include "mfcCImageDlg.h"
#include "afxdialogex.h"
#include <vector>   // vector 사용을 위해 추가
#include <thread>   // thread 처리를 위해 추가
#include <chrono>   // 시간 지연을 하기 위해 사용 (이미지 저장 시간이 조금 필요할거라 느껴서 추가)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CmfcCImageDlg, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT_X1, &CmfcCImageDlg::OnEnChangeEditX1)
    ON_EN_CHANGE(IDC_EDIT_Y1, &CmfcCImageDlg::OnEnChangeEditY1)
    ON_EN_CHANGE(IDC_EDIT_X2, &CmfcCImageDlg::OnEnChangeEditX2)
    ON_EN_CHANGE(IDC_EDIT_Y2, &CmfcCImageDlg::OnEnChangeEditY2)          
    ON_BN_CLICKED(IDC_BTN_DRAW, &CmfcCImageDlg::OnBnClickedBtnDraw)         // Draw 버튼 (X1, Y1 좌표에 랜덤한 색, 랜덤한 반지름으로 원을 그림)
    ON_BN_CLICKED(IDC_BTN_ACTION, &CmfcCImageDlg::OnBnClickedBtnAction)     // Action 버튼 (X1, Y1 좌표에서 X2, Y2 좌표로 정해진 랜덤한 거리로 일정하게 일직선으로 이동)
    ON_BN_CLICKED(IDC_BTN_OPEN, &CmfcCImageDlg::OnBnClickedBtnOpen)         // Open 버튼 (이미지 가져와서 원 원점 알려주는 함수)
    ON_BN_CLICKED(IDC_BTN_RESET, &CmfcCImageDlg::OnBnClickedBtnReset)       // 초기화 버튼 추가 (원을 없애고 흰 배경만 남기고 싶을때가 있어서 추가한 버튼)
    ON_WM_CTLCOLOR()                                                        // 배경색 변경 처리 (처음에 흰색으로 바꿀때만 사용하기는 합니다.)
END_MESSAGE_MAP()

// 그림 그림 공간에 흰색 배경 넣기, 좌표 초기화
CmfcCImageDlg::CmfcCImageDlg(CWnd* pParent)
    : CDialogEx(IDD_MFCCIMAGE_DIALOG, pParent)
    , m_x1(0), m_y1(0), m_x2(0), m_y2(0)                // x1, y1, x2, y2 초기화
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_brushWhite.CreateSolidBrush(RGB(255, 255, 255));  // 흰색 배경 생성
}


// DDX/DDV 지원
void CmfcCImageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // 좌표 입력 CEdit 컨트롤과 연결
    DDX_Control(pDX, IDC_EDIT_X1, m_editX1);
    DDX_Control(pDX, IDC_EDIT_Y1, m_editY1);
    DDX_Control(pDX, IDC_EDIT_X2, m_editX2);
    DDX_Control(pDX, IDC_EDIT_Y2, m_editY2);
    // 네모 영역과 CStatic 컨트롤 연결
    DDX_Control(pDX, IDC_STATIC_DRAW_AREA, m_drawArea);

}

// Draw 버튼 클릭 시
void CmfcCImageDlg::OnBnClickedBtnDraw()
{
    // 입력받은 좌표 읽기
    CString strX1, strY1;
    m_editX1.GetWindowText(strX1);
    m_editY1.GetWindowText(strY1);
    m_x1 = _ttoi(strX1);
    m_y1 = _ttoi(strY1);

    // X1 좌표 유효성 검증
    if (m_x1 < 75 || m_x1 > 600)
    {
        AfxMessageBox(_T("X1 좌표는 75에서 600 사이의 값이어야 합니다."));
        return;
    }
    // Y1 좌표 유효성 검증
    if (m_y1 < 75 || m_y1 > 350)
    {
        AfxMessageBox(_T("Y1 좌표는 75에서 350 사이의 값이어야 합니다."));
        return;
    }
    DrawRandomCircle(m_x1, m_y1); // (x1, y1) 좌표에 원 그리기

    // Draw 버튼 이미지를 DrawImages 폴더에 저장
    SaveImage(m_FolderPath);
}



// Action 버튼 클릭 시
void CmfcCImageDlg::OnBnClickedBtnAction()
{
    // 비모달 진행 상태를 보여주는 창 생성 (비모달이란, 메세지 창이 떠 있어도 다른 프로그램이 병렬 작업이 가능한 것)
    CWnd* pProgressWnd = new CWnd();
    pProgressWnd->CreateEx(0, AfxRegisterWndClass(0), _T("작업 진행 중..."), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 100, NULL, NULL);
    //이 함수는 작동하는데 다른 작업들은 같이 못 하도록 창을 띄우도록 했음.
    
    CString strX1, strY1, strX2, strY2;
    m_editX1.GetWindowText(strX1);
    m_editY1.GetWindowText(strY1);
    m_editX2.GetWindowText(strX2);
    m_editY2.GetWindowText(strY2);

    m_x1 = _ttoi(strX1);
    m_y1 = _ttoi(strY1);
    m_x2 = _ttoi(strX2);
    m_y2 = _ttoi(strY2);

    // X1, X2 좌표 유효성 검증
    if (m_x1 < 75 || m_x1 > 600)
    {
        AfxMessageBox(_T("X1 좌표는 75에서 600 사이의 값이어야 합니다."));
        return;
    }

    if (m_x2 < 75 || m_x2 > 600)
    {
        AfxMessageBox(_T("X2 좌표는 75에서 600 사이의 값이어야 합니다."));
        return;
    }

    // Y1, Y2 좌표 유효성 검증
    if (m_y1 < 75 || m_y1 > 350)
    {
        AfxMessageBox(_T("Y1 좌표는 75에서 350 사이의 값이어야 합니다."));
        return;
    }

    if (m_y2 < 75 || m_y2 > 350)
    {
        AfxMessageBox(_T("Y2 좌표는 75에서 350 사이의 값이어야 합니다."));
        return;
    }

    
    int radius = rand() % 50 + 10;  // 반지름 랜덤하게 설정
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    CBrush brush(RGB(r, g, b)); // 랜덤 색상
    //미리 좌표 저장
    int x = m_x1;
    int y = m_y1;
    int randDistance = (rand() % 41) + 10; // 랜덤한 이동거리 결정 (10~50) 으로 설정하기 위한 수식
    
    // 랜덤 거리 이동일 때 일직선으로 이동 시키고 싶어서 진행한 처리
    // 두 점 사이의 총 거리 계산
    double totalDistance = sqrt(pow(m_x2 - m_x1, 2) + pow(m_y2 - m_y1, 2));
    // 이동 비율 계산 (랜덤한 거리를 전체 거리로 나눈 값)
    double moveRatio = static_cast<double>(randDistance) / totalDistance;
    // x와 y의 증분 계산 (이 거리로 이동하면 일직선으로 이동)
    int deltaX = static_cast<int>((m_x2 - m_x1) * moveRatio);
    int deltaY = static_cast<int>((m_y2 - m_y1) * moveRatio);
    
    while (x < m_x2 && y < m_y2) {
        
        // 새로운 x, y 좌표를 계산하고 목표 지점을 넘지 않도록 제한
        x = (x < m_x2) ? min(x + deltaX, m_x2) : m_x2;
        y = (y < m_y2) ? min(y + deltaY, m_y2) : m_y2;

        DrawFixedCircle(x, y, radius, brush);
        SaveImage(m_FolderPath);  // 매번 이미지를 저장
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 이미지를 저장할 시간을 주기 위해 추가
    }
    // 비모달 창 삭제
    pProgressWnd->DestroyWindow();
    delete pProgressWnd;

}
void CmfcCImageDlg::DrawFixedCircle(int x, int y, int radius, CBrush& brush)
{
    // Static Text 컨트롤에서 DC를 얻어서 그림을 그립니다. (그림 그리기 위해서 가져옴)
    CDC* pDC = m_drawArea.GetDC();
    if (pDC != nullptr) //그림 그리기 위해 가져온 DC가 잘 가져왔는지 확인
    {
        // 메모리 DC 생성 (미리 인터페이스를 가져와 작업)
        CDC memDC;
        memDC.CreateCompatibleDC(pDC);

        // CStatic 영역의 크기를 얻음
        CRect rect;
        m_drawArea.GetClientRect(&rect);

        // 메모리 비트맵 생성 (CStatic 영역과 동일 크기)
        CBitmap bitmap;
        bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

        // 메모리 DC에 비트맵 선택
        CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

        // 배경 흰색으로 채우기
        memDC.FillSolidRect(&rect, RGB(255, 255, 255));

        // 고정된 브러시로 원 그리기
        CBrush* pOldBrush = memDC.SelectObject(&brush);
        memDC.Ellipse(x - radius, y - radius, x + radius, y + radius);

        // 화면에도 원을 그리기
        pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

        // CImage로 저장하기 위해 비트맵을 CImage에 적용
        m_image.Destroy(); // 이전 이미지가 있다면 해제
        m_image.Attach((HBITMAP)bitmap.Detach());

        // 메모리 해제 및 원래 브러시 복원
        memDC.SelectObject(pOldBrush);
        memDC.SelectObject(pOldBitmap);
        m_drawArea.ReleaseDC(pDC);
    }
}

// OnCtlColor 함수에서 컨트롤의 배경색을 설정
HBRUSH CmfcCImageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // Static 컨트롤에 대해서만 흰색 배경 적용
    if (pWnd->GetDlgCtrlID() == IDC_STATIC_DRAW_AREA)
    {
        pDC->SetBkMode(TRANSPARENT);   // 배경을 투명화하게 설정
        pDC->SetBkColor(RGB(255, 255, 255));  // 배경색을 흰색으로 설정
        return m_brushWhite;  // 흰색 반환
    }

    return hbr;
}

// 랜덤한 크기의 원 그리기 함수
void CmfcCImageDlg::DrawRandomCircle(int x, int y)
{
    // Static Text 컨트롤에서 DC를 얻어서 그린다.
    CDC* pDC = m_drawArea.GetDC();
    if (pDC != nullptr)
    {
        // 메모리 DC 생성
        CDC memDC;
        memDC.CreateCompatibleDC(pDC);

        // CStatic 영역의 크기를 얻음
        CRect rect;
        m_drawArea.GetClientRect(&rect);

        // 메모리 비트맵 생성 (CStatic 영역과 동일 크기)
        CBitmap bitmap;
        bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

        // 메모리 DC에 비트맵 선택
        CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

        // 배경 흰색으로 채우기
        memDC.FillSolidRect(&rect, RGB(255, 255, 255));

        // 랜덤한 색상 생성
        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;

        // 런더 색상으로 브러쉬 생성
        CBrush brush(RGB(r, g, b));
        CBrush* pOldBrush = memDC.SelectObject(&brush);

        // 랜덤한 반지름을 가진 원 그리기
        int radius = rand() % 50 + 10;
        memDC.Ellipse(x - radius, y - radius, x + radius, y + radius);

        // 화면에 원 그리기
        pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

        // CImage로 저장하기 위해 비트맵을 CImage에 적용
        m_image.Destroy(); // 이전 이미지가 있다면 해제
        m_image.Attach((HBITMAP)bitmap.Detach());

        // 메모리 해제 및 원래 브러쉬 복원
        memDC.SelectObject(pOldBrush);
        memDC.SelectObject(pOldBitmap);
        m_drawArea.ReleaseDC(pDC);
    }
}


// X자와 좌표 그리기
void CmfcCImageDlg::DrawCrossAndCoordinates(CDC* pDC, int x, int y)
{
    pDC->MoveTo(x - 5, y);
    pDC->LineTo(x + 5, y);
    pDC->MoveTo(x, y - 5);
    pDC->LineTo(x, y + 5);

    CString strCoords;
    strCoords.Format(_T("(%d, %d)"), x, y);
    pDC->TextOut(x + 10, y + 10, strCoords);
}

// 이미지를 저장하는 함수
void CmfcCImageDlg::SaveImage(const CString& folderPath)
{
    static int imageIndex = 1; // 이미지 인덱스 추가
    // 현재 시간을 가져오기 위해 SYSTEMTIME 구조체 사용
    SYSTEMTIME st;
    GetLocalTime(&st);  // 로칬 시스템 시간 가져오기

    // 시간을 기반으로 파일 이름 생성
    CString filename;
    filename.Format(_T("%s\\image_%04d%02d%02d_%02d%02d%02d_%d.bmp"),
        (LPCTSTR)folderPath, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, imageIndex++);

    // 이미지가 유효한지 확인 후 저장
    if (m_image.IsNull() == FALSE)
    {
        HRESULT hr = m_image.Save(filename);
        if (FAILED(hr))
        {
            AfxMessageBox(_T("이미지 저장에 실패했습니다."));
        }
    }
    else
    {
        AfxMessageBox(_T("저장할 이미지가 없습니다."));
    }
}




void CmfcCImageDlg::OnEnChangeEditX1()
{
    // X1 좌표가 변경될 때의 처리 로직
}

void CmfcCImageDlg::OnEnChangeEditY1()
{
    // Y1 좌표가 변경될 때의 처리 로직
}

void CmfcCImageDlg::OnEnChangeEditX2()
{
    // X2 좌표가 변경될 때의 처리 로직
}

void CmfcCImageDlg::OnEnChangeEditY2()
{
    // Y2 좌표가 변경될 때의 처리 로직
}

void CmfcCImageDlg::OnStnClickedStaticDrawArea()
{
    // Static 컨트롤 클릭 시의 처리 로직 (필요 시 구현)
}
void CmfcCImageDlg::OnBnClickedBtnReset()
{
    // DC를 얻음
    CDC* pDC = m_drawArea.GetDC();
    if (pDC != nullptr)
    {
        CRect rect;
        m_drawArea.GetClientRect(&rect);  // Static 영역의 크기 가져오기
        pDC->FillSolidRect(&rect, RGB(255, 255, 255));  // 흰색으로 채우기

        m_drawArea.ReleaseDC(pDC);  // DC 해제
    }
}

BOOL CmfcCImageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);   // 크론 아이콘 설정
    SetIcon(m_hIcon, FALSE);  // 작은 아이콘 설정

    // 프로그램 시작 시 폴더 생성
    CreateDirectories();

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환
}

// 폴더를 생성하는 함수
void CmfcCImageDlg::CreateDirectories()
{
    // 실행파일 경로에 image 폴더에 상위 폴더 생성
    m_FolderPath = _T("image");

    // image 폴더가 없으면 생성
    if (!CreateDirectory(m_FolderPath, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
    {
        AfxMessageBox(_T("image 폴더를 생성할 수 없습니다."));
        return; // images 폴더 생성 실패 시 중단
    }

}


void CmfcCImageDlg::OnBnClickedBtnOpen()
{
    // 기존 이미지를 먼저 해제 안하면 Draw나 Action 후 실행하면 오류걸림
    if (!m_image.IsNull()) {
        m_image.Destroy();
    }
    // 파일 탐색기 창을 통해 이미지 파일을 선택합니다.
    CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
        _T("Image Files (*.bmp; *.jpg; *.jpeg)|*.bmp;*.jpg;*.jpeg|All Files (*.*)|*.*||"), this);

    if (dlg.DoModal() == IDOK)
    {
        CString filePath = dlg.GetPathName();

        // 이미지를 로드합니다.
        HRESULT hr = m_image.Load(filePath);
        if (FAILED(hr))
        {
            AfxMessageBox(_T("이미지를 로드하는 데 실패했습니다."));
            return;
        }

        // 이미지의 크기 가져오기
        int width = m_image.GetWidth();
        int height = m_image.GetHeight();

        // 흰색을 제외한 원의 픽셀 위치 계산
        int sumX = 0, sumY = 0, count = 0;
        std::vector<CPoint> points;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                COLORREF color = m_image.GetPixel(x, y);

                // 흰색이 아닌지 확인 (흰색의 RGB 값은 255, 255, 255)
                // 흰색이 아니라면 원이라고 생각하여 중심 구하기 위한 값으로 넣기
                BYTE r = GetRValue(color);
                BYTE g = GetGValue(color);
                BYTE b = GetBValue(color);

                if (!(r == 255 && g == 255 && b == 255))
                {
                    sumX += x;
                    sumY += y;
                    count++;
                    points.push_back(CPoint(x, y));
                }
            }
        }

        // 원의 중심 좌표 계산
        if (count > 0)
        {
            int centerX = sumX / count;
            int centerY = sumY / count;

            // 반지름 계산 (중심으로부터 가장 먼 점을 찾습니다)
            double maxDistance = 0;
            for (const auto& point : points)
            {
                double distance = sqrt(pow(point.x - centerX, 2) + pow(point.y - centerY, 2));
                if (distance > maxDistance)
                {
                    maxDistance = distance;
                }
            }

            float radius = static_cast<float>(maxDistance);

            // 다이얼로그에서 Static 영역의 DC를 가져옵니다.
            CDC* pDC = m_drawArea.GetDC();
            if (pDC != nullptr)
            {
                // Static 영역의 크기 가져오기
                CRect rect;
                m_drawArea.GetClientRect(&rect);

                // 이미지를 Static 영역에 그립니다.
                m_image.Draw(pDC->m_hDC, 0, 0, rect.Width(), rect.Height());

                // 원의 중심에 X 표시하기
                pDC->MoveTo(centerX - 5, centerY - 5); //좌측 하단에서
                pDC->LineTo(centerX + 5, centerY + 5); //우측 상단으로 선

                pDC->MoveTo(centerX - 5, centerY + 5); //좌측 상단에서
                pDC->LineTo(centerX + 5, centerY - 5); //우측 하단으로 선

                // 중심 좌표값 및 반지름을 문자열로 표시합니다.
                CString strCoords;
                strCoords.Format(_T("(%d, %d), Radius: %.2f"), centerX, centerY, radius);
                pDC->TextOut(centerX + 10, centerY + 10, strCoords);

                // DC 해제
                m_drawArea.ReleaseDC(pDC);
            }
        }
        else //그림이 없는 화면을 가져온 경우
        {
            AfxMessageBox(_T("원을 찾을 수 없습니다."));
        }
    }
}
