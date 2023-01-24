
// PhotoFilterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "PhotoFilter.h"
#include "PhotoFilterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPhotoFilterDlg 대화 상자



CPhotoFilterDlg::CPhotoFilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PHOTOFILTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhotoFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPhotoFilterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GRAY_BTN, &CPhotoFilterDlg::OnBnClickedGrayBtn)
	ON_BN_CLICKED(IDC_FILTER1_BTN, &CPhotoFilterDlg::OnBnClickedFilter1Btn)
	ON_BN_CLICKED(IDC_FILTER2_BTN, &CPhotoFilterDlg::OnBnClickedFilter2Btn)
	ON_BN_CLICKED(IDC_FILTER3_BTN, &CPhotoFilterDlg::OnBnClickedFilter3Btn)
	ON_BN_CLICKED(IDC_RESTORE_BTN, &CPhotoFilterDlg::OnBnClickedRestoreBtn)
	ON_BN_CLICKED(IDC_COMMIT_BTN, &CPhotoFilterDlg::OnBnClickedCommitBtn)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CPhotoFilterDlg 메시지 처리기

BOOL CPhotoFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_image.Load(L"me2.jpg");
	GetObject((HBITMAP)m_image, sizeof(BITMAP), &m_bmp_info);

	mp_image_pattern = (unsigned char*)m_image.GetBits();
	mp_image_pattern += m_bmp_info.bmWidthBytes - 1;

	m_commit_image.Create(m_bmp_info.bmWidth, m_bmp_info.bmHeight, m_bmp_info.bmBitsPixel);
	mp_commit_pattern = (unsigned char*)m_commit_image.GetBits();
	mp_commit_pattern += m_bmp_info.bmWidthBytes - 1;

	int image_size = m_bmp_info.bmWidth * m_bmp_info.bmHeight * 3; // 각 점은 3바이트씩 되있기 때문에 3을 곱함
	memcpy(mp_commit_pattern - image_size + 3, mp_image_pattern - image_size + 3, image_size);	// 메모리 끝위치에 있으니 제일 앞쪽으로 올수있도록 image_size를 뺌

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPhotoFilterDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		dc.SetStretchBltMode(COLORONCOLOR);
		m_image.Draw(dc, 0, 0, m_bmp_info.bmWidth / 4, m_bmp_info.bmHeight / 4);
		// CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPhotoFilterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPhotoFilterDlg::OnBnClickedGrayBtn()
{
	int dot_count = m_bmp_info.bmWidth * m_bmp_info.bmHeight;
	unsigned char* p = mp_image_pattern; // 초기 기준점 기억
	int temp;


	for (int i = 0; i < dot_count; i++)
	{
		temp = (*p + *(p - 1) + *(p - 2)) / 3;	// red green blue 다 더해서 평균낸 값
		*p-- = temp;	// 1바이트 이동, Red
		*p-- = temp;	// 1바이트 이동, Green
		*p-- = temp;	// 1바이트 이동, Blue
	}

	Invalidate(FALSE);
}


_inline void InclimentColor(unsigned char* ap_color, int a_value)
{
	int temp = *ap_color + a_value;
	if (temp > 255) *ap_color = 255;
	else *ap_color = temp;
}

void CPhotoFilterDlg::OnBnClickedFilter1Btn()
{
	int dot_count = m_bmp_info.bmWidth * m_bmp_info.bmHeight;
	unsigned char* p = mp_image_pattern; // 초기 기준점 기억
	/*int temp;*/

	for (int i = 0; i < dot_count; i++)
	{
		InclimentColor(p--, 30);
		InclimentColor(p--, 30);
		InclimentColor(p--, 30);

		//if (temp > 255) *p = 255;
		//else *p = temp;	// Red 값에 고정값 상승
		//p--;			// 1바이트 이동

		//if (temp > 255) *p = 255;
		//else *p = temp;	// Green 값에 고정값 상승
		//p--;			// 1바이트 이동

		//if (temp > 255) *p = 255;
		//else *p = temp;	// Blue 값에 고정값 상승
		//p--;			// 1바이트 이동
	}

	Invalidate(FALSE);
}

_inline void InclimentColor2(unsigned char* ap_color, float a_rate)
{
	// RGB중 제일 큰 값 찾기
	int index;
	if (*ap_color > *(ap_color - 1))	// red > green
	{
		if (*ap_color > *(ap_color - 2)) index = 0;	// R이 최대값
		else index = 2;	// B가 최대값
	}
	else
	{
		if (*(ap_color - 1) > *(ap_color - 2)) index = 1;	// G가 최대값
		else index = 2;	// B가 최대값
	}

	if (*(ap_color - index) * a_rate > 255)
	{
		a_rate = 255.0f / *(ap_color - index);
	}
	*ap_color-- = (int)(*ap_color * a_rate);	// Red
	*ap_color-- = (int)(*ap_color * a_rate);	// Green
	*ap_color-- = (int)(*ap_color * a_rate);	// Blue
}

void CPhotoFilterDlg::OnBnClickedFilter2Btn()
{
	int dot_count = m_bmp_info.bmWidth * m_bmp_info.bmHeight;
	unsigned char* p = mp_image_pattern; // 초기 기준점 기억

	for (int i = 0; i < dot_count; i++)
	{
		InclimentColor2(p, 1.2f); // 20% 정도 밝게 go
		p -= 3;
	}

	Invalidate(FALSE);
}


void CPhotoFilterDlg::OnBnClickedFilter3Btn()
{
	int dot_count = m_bmp_info.bmWidth * m_bmp_info.bmHeight;
	unsigned char* p = mp_image_pattern; // 초기 기준점 기억
	int temp;

	for (int i = 0; i < dot_count; i++)
	{
		temp = (*p + *(p - 1) + *(p - 2)); // RGB 합산
		InclimentColor2(p, 1.0f + ((768 - temp) * 2) / 6000.0f); // 어두운 색이 더 가중치를 받음
		p -= 3;
	}

	Invalidate(FALSE);
}


void CPhotoFilterDlg::OnBnClickedRestoreBtn()
{
	int image_size = m_bmp_info.bmWidth * m_bmp_info.bmHeight * 3;
	memcpy(mp_image_pattern - image_size + 3, mp_commit_pattern - image_size + 3, image_size);

	Invalidate(FALSE);
}


void CPhotoFilterDlg::OnBnClickedCommitBtn()
{
	int image_size = m_bmp_info.bmWidth * m_bmp_info.bmHeight * 3;
	memcpy(mp_commit_pattern - image_size + 3, mp_image_pattern - image_size + 3, image_size);
}


void CPhotoFilterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 밖을 누르면 안쪽으로 당겨지도록 하는 보완코드
	if (point.x < 50) point.x = 50;
	else if (point.x + 50 >= m_bmp_info.bmWidth / 4) point.x = m_bmp_info.bmWidth / 4 - 51; // 마우스 영역을 찾는데에는 이미지가 현재 1/4로 축소되있어서 4를 나눠줘야됨.
	
	if (point.y < 50) point.y = 50;
	else if (point.y + 50 >= m_bmp_info.bmHeight / 4) point.y = m_bmp_info.bmHeight / 4 - 51;

	int pos = -(point.y * 4) * m_bmp_info.bmWidth * 3 - (m_bmp_info.bmWidth - (point.x * 4)) * 3; // 실제 사진 영역의 포지션을 찾기위함이므로 * 4
	unsigned char* p = mp_image_pattern + pos; // 원의 중심점
	unsigned char* p_commit = mp_commit_pattern + pos; // 커밋된 사진의 원의 중심점

	// 커밋을 했을때만 재클릭했을 경우 밝아짐
	if(*p == *p_commit) InclimentColor2(p, 1.2f); // 원의 중심점 밝기를 1.2배 올림

	double radian, x, y;

	for (int degree = 0; degree < 360; degree++)
	{
		radian = degree * 3.141592 / 180.0;
		
		x = sin(radian);
		y = cos(radian);

		for (int radius = 1; radius < 50; radius++)
		{
			int pos = -(point.y * 4 - (int)(y * radius)) * m_bmp_info.bmWidth * 3 - (m_bmp_info.bmWidth - (point.x * 4) - (int)(x * radius)) * 3;
			p = mp_image_pattern + pos;
			p_commit = mp_commit_pattern + pos;
			if (*p == *p_commit) InclimentColor2(p, 1.2f);
		}
	}

	Invalidate(FALSE);

	/*CDialogEx::OnLButtonDown(nFlags, point);*/
}
