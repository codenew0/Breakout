
// playtrikeView.h : CplaytrikeView ��Ľӿ�
//

#pragma once

#define ID_BALLTIME 0
#define ID_BULLETTIME 1
#define ID_TIME 2
#define brick_NUM 50

typedef struct Hit
{
	int x, y;
	int now, n;
	int nbrick;
}Hit;

typedef struct Ball
{
	int x, y;
	int rad;
	int r, g, b;
	int speedx, speedy;
	int dir;
	struct Ball *next, *prev;
}Ball;

typedef struct Bullet
{
	int speed;
	int x, y;
	int n;
	struct Bullet *next;
}Bullet;

typedef struct Board
{
	int x, y;
	int length, width;
	int r, g, b;
	Bullet *bullet;
}Board;

typedef struct brick
{
	int x, y;
	int length, width;
	int r, g, b;
	int flag;
}brick;

class CplaytrikeView : public CView
{
protected: // �������л�����
	CplaytrikeView();
	DECLARE_DYNCREATE(CplaytrikeView)

// ����
public:
	CplaytrikeDoc* GetDocument() const;

// ����
public:
	Board board;
	brick brick[brick_NUM];
	Ball *ball;
	Hit hit;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CplaytrikeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // playtrikeView.cpp �еĵ��԰汾
inline CplaytrikeDoc* CplaytrikeView::GetDocument() const
   { return reinterpret_cast<CplaytrikeDoc*>(m_pDocument); }
#endif

