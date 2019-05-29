
// playtrikeView.cpp : CplaytrikeView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "playtrike.h"
#endif

#include "playtrikeDoc.h"
#include "playtrikeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CplaytrikeView

IMPLEMENT_DYNCREATE(CplaytrikeView, CView)

BEGIN_MESSAGE_MAP(CplaytrikeView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CplaytrikeView 构造/析构
#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")

CplaytrikeView::CplaytrikeView()
{
	// TODO: 在此处添加构造代码
	PlaySound((LPCTSTR)IDR_WAVE1, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC | SND_LOOP);
	board.x = board.y = 0;
	board.length = 30;
	board.width = 5;
	board.r = board.g = board.b = 0;
	board.bullet = NULL;
	ball = new Ball;
	hit.now = 0;
	hit.n = 1;
	hit.x = hit.y = 0;
	hit.nbrick = 50;
	for (int i = 0; i < brick_NUM; i++)
	{
		brick[i].length = 30;
		brick[i].width = 15;
		brick[i].flag = 1;
	}
}

CplaytrikeView::~CplaytrikeView()
{
	Ball *p = ball;
	while (p)
	{
		Ball *tmp = p;
		p = p->next;
		delete(tmp);
	}
	Bullet *b = board.bullet;
	while (b)
	{
		Bullet *tmp = b;
		b = b->next;
		delete(tmp);
	}
}

BOOL CplaytrikeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	
	return CView::PreCreateWindow(cs);
}

// CplaytrikeView 绘制
int init = 0;
int init_brick = 0;
int mytime = 0;
CPoint mouse;
CString about[] = {"Description: as we all know, this is a game which is called \"play brick\","
,"but it's different from what you player ever."
,"There are some rules:"
,"1. Start game by clicking left button"
,"2. Launch bullet by clicking right button, and you can do it continually"
,"when you hit the first ball, it can split a lot!"
,"3. If you eliminate all the brick, congratulation... they can create again!"
,"4. Key up/down control the speed of the first ball, left/right control the size"
,"5. Have fun"};
void CplaytrikeView::OnDraw(CDC* pDC)
{
	CplaytrikeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	RECT rect;
	GetClientRect(&rect);
	Ball *p = ball;
	Bullet *b = board.bullet;
	//srand(time(NULL));
	//初始化挡板位置
	if (init == 0)
	{
		board.x = rect.right / 2;
		board.y = rect.bottom - 10;
		init++;
	}
	else
	{
		board.x = mouse.x;
		board.y = rect.bottom - 10;
	}
	//初始化砖块位置
	if (init_brick == 0)
	{
		for (int i = 0; i < brick_NUM; i++)
		{
			brick[i].flag = 1;
			brick[i].x = rand() % (rect.right - brick[i].length) + brick[i].length / 2;
			brick[i].y = rand() % (rect.bottom / 3) + brick[i].width / 2;
			brick[i].r = rand() % 256;
			brick[i].g = rand() % 256;
			brick[i].b = rand() % 256;
		}
		hit.nbrick = brick_NUM;
		init_brick++;
	}
	//增加小球
	if (hit.now < hit.n)
	{	
		if (hit.now == 0)//第一个球
		{
			ball->x = ball->y = 0;
			ball->rad = 5;
			ball->r = 255;
			ball->g = ball->b = 0;
			ball->speedx = ball->speedy = 3;
			ball->dir = 45;
			hit.now = 1;
			ball->next  = ball->prev = NULL;
		}
		else	//分离的球
		{
			Ball *q = new Ball;
			q->x = hit.x;
			q->y = hit.y;
			q->speedy =  3;
			q->dir = rand() % 361;
			q->speedx = (int)tan(q->dir) * q->speedy;
			q->r = rand() % 256;
			q->g = rand() % 256;
			q->b = rand() % 256;
			q->rad = 5;
			q->next = NULL;
			while (p->next)
			{
				p = p->next;
			}
			p->next = q;
			q->prev = p;
			hit.now++;
		}
		p = ball;
	}
	//画小球
	while (p)
	{
		CBrush brush(RGB(p->r, p->g, p->b));
		CBrush *oldbrush;
		oldbrush = pDC->SelectObject(&brush);
		pDC->Ellipse(p->x - p->rad, p->y - p->rad, p->x + p->rad, p->y + p->rad);
		pDC->SelectObject(oldbrush);
		//pDC->Ellipse(p->x - p->rad, p->y - p->rad, p->x + p->rad, p->y + p->rad);
		p = p->next;
	}
	//画挡板
	CRect rc(board.x - board.length / 2, board.y - board.width / 2,
					board.x + board.length / 2, board.y + board.width / 2);
	pDC->FillSolidRect(&rc, RGB(board.r, board.g, board.b));
	/*pDC->Rectangle(board.x - board.length / 2, board.y - board.width / 2,
					board.x + board.length / 2, board.y + board.width / 2);*/
	//画子弹
	int i = 0;
	if (board.bullet != NULL)
	{
		while (b)
		{
			pDC->Rectangle(b->x - 1, b->y - 3, b->x + 1, b->y + 3);
			int distance = (int)sqrt(pow((double)(ball->x - b->x), 2.0) + pow((double)(ball->y - b->y), 2));
			
			if (b->y <= rect.top)
			{
				Bullet *tmp = b;
				b = b->next;
				board.bullet = b;
				delete(tmp);
				//KillTimer(ID_BULLETTIME);
			}
			else if (distance <= ball->rad)
			{
				//ball->speedx>0?ball->speedx+=1:ball->speedx-=1;
				//ball->speedy>0?ball->speedy+=1:ball->speedy-=1;
				hit.x = ball->x;
				hit.y = ball->y;
				hit.n++;
				b = b->next;
			}
			else
				b = b->next;
		}
	}
	//文字
	CClientDC dc(this);
	CFont font;
	VERIFY(font.CreatePointFont(120, "Arial", &dc));
	CFont* def_font = dc.SelectObject(&font);
	CString s = "Please Full Screen Game";
	dc.TextOut(5, 5, s, s.GetLength());
	s.Format("Game time: %d", mytime);
	dc.TextOut(5, 20, s, s.GetLength());
	if (mytime <= 60)
	{
		for (int i = 0; i < 9; i++)
			dc.TextOut(rect.right - 500, rect.bottom - 20 * (16 - i), about[i], about[i].GetLength());
	}
	dc.SelectObject(def_font);
	//画砖块
	for (int i = 0; i < brick_NUM; i++)
	{	
		if (brick[i].flag == 1)
		{
			CBrush brush(RGB(brick[i].r, brick[i].g, brick[i].b));
			CBrush *oldbrush;
			oldbrush = pDC->SelectObject(&brush);
			pDC->Rectangle(brick[i].x - brick[i].length / 2, brick[i].y - brick[i].width / 2,
				brick[i].x + brick[i].length / 2, brick[i].y + brick[i].width / 2);
			pDC->SelectObject(oldbrush);
		}
	}
	for (int i = 0; i < brick_NUM; i++)
	{
		if (brick[i].flag == 0)
			hit.nbrick--;
	}
	if (hit.nbrick == 0)
		init_brick = 0;
	else
		hit.nbrick = brick_NUM;
}

void CplaytrikeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CplaytrikeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CplaytrikeView 诊断

#ifdef _DEBUG
void CplaytrikeView::AssertValid() const
{
	CView::AssertValid();
}

void CplaytrikeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CplaytrikeDoc* CplaytrikeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CplaytrikeDoc)));
	return (CplaytrikeDoc*)m_pDocument;
}
#endif //_DEBUG


// CplaytrikeView 消息处理程序


void CplaytrikeView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RECT rect;
	GetClientRect(&rect);

	if (nIDEvent == ID_BALLTIME)
	{
		Ball *p = ball;
		while (p)
		{
			for (int i = 0; i < brick_NUM; i++)
			{
				if (brick[i].flag == 0)
					continue;
				if (abs(p->x + p->rad + p->speedx - brick[i].x) <= brick[i].length / 2 && \
					(p->y + p->rad < brick[i].y + brick[i].width / 2 && p->y + p->rad > brick[i].y - brick[i].width / 2))
				{
					brick[i].flag = 0;
					p->speedx *= -1;
				}
				if (abs(p->y + p->rad + p->speedy - brick[i].y) <= brick[i].width / 2 &&\
					(p->x + p->rad < brick[i].x + brick[i].length / 2 && p->x + p->rad > brick[i].x - brick[i].length / 2))
				{
					brick[i].flag = 0;
					p->speedy *= -1;
				}
			}
			if (p->x + p->rad + p->speedx > rect.right || p->x + p->rad + p->speedy < rect.left)
				p->speedx *= -1;
			if (p->y + p->rad + p->speedy > rect.bottom || p->y + p->rad + p->speedy < rect.top || \
				p->y + p->rad + p->speedy > board.y && (p->x + p->rad > rect.left + board.x - board.length / 2 && 
				p->x + p->rad < rect.left + board.x + board.length / 2))
			{
				if (p->y + p->rad + p->speedy > board.y && (p->x + p->rad > rect.left + board.x - board.length / 2 && 
					p->x + p->rad < rect.left + board.x + board.length / 2))
				{
					board.length += 4;
					board.r = rand() % 256;
					board.g = rand() % 256;
					board.b = rand() % 256;
				}
				else if (p->y + p->rad + p->speedy > rect.bottom && p != ball)
				{
					Ball *tmp= p;
					p->prev->next = p->next;
					if (p->next)
						p->next->prev = p->prev;
					p = p->next;
					delete(tmp);
				}
				if (p)
					p->speedy *= -1;
			}
			if (p)
			{
				p->x += p->speedx;
				p->y += p->speedy;
				p = p->next;
			}
		}
	}
	else if (nIDEvent == ID_BULLETTIME)
	{
		Bullet *p = board.bullet;
		while (p)
		{
			p->y -= board.bullet->speed;
			p = p->next;
		}
	}
	else if (nIDEvent == ID_TIME)
	{
		mytime++;
	}
	Invalidate();

	CView::OnTimer(nIDEvent);
}


void CplaytrikeView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	mouse = point;

	CView::OnMouseMove(nFlags, point);
}


void CplaytrikeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Bullet *p = board.bullet;
	Bullet *q = NULL;
	if (board.bullet == NULL)
	{
		board.bullet = new Bullet;
		board.bullet->n = 1;
		board.bullet->x = board.x;
		board.bullet->y = board.y;
		board.bullet->speed = 10;
		board.bullet->next = NULL;
	}
	else
	{
		q = new Bullet;
		q->n++;
		q->x = board.x;
		q->y = board.y;
		q->speed = 10;
		q->next =  NULL;
		while (p->next)
			p = p->next;
		p->next = q;
	}
	SetTimer(ID_BULLETTIME, 1, NULL);

	CView::OnRButtonDown(nFlags, point);
}


void CplaytrikeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int i = 0;
	if (i == 0)
	{
		SetTimer(ID_BALLTIME, 100, NULL);
		board.x = point.x;
		board.y = point.y;
		i++;
	}
	SetTimer(ID_TIME, 1000, NULL);

	CView::OnLButtonDown(nFlags, point);
}


void CplaytrikeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nChar)
	{
	case VK_UP:
		if (ball->speedx >= 5)
			break;
		ball->speedx>0?ball->speedx++:ball->speedx--;
		ball->speedy>0?ball->speedy++:ball->speedy--;
		break;
	case VK_DOWN:
		if (ball->speedx <= 0)
			break;
		ball->speedx<0?ball->speedx++:ball->speedx--;
		ball->speedy<0?ball->speedy++:ball->speedy--;
		break;
	case VK_LEFT:
		if (ball->rad < 3)
			break;
		ball->rad--;
		break;
	case VK_RIGHT:
		if (ball->rad > 20)
			break;
		ball->rad++;
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


//BOOL CplaytrikeView::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	return NULL;
//	//return CView::OnEraseBkgnd(pDC);
//}
