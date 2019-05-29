
// playtrikeView.h : CplaytrikeView 类的接口
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
protected: // 仅从序列化创建
	CplaytrikeView();
	DECLARE_DYNCREATE(CplaytrikeView)

// 特性
public:
	CplaytrikeDoc* GetDocument() const;

// 操作
public:
	Board board;
	brick brick[brick_NUM];
	Ball *ball;
	Hit hit;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CplaytrikeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // playtrikeView.cpp 中的调试版本
inline CplaytrikeDoc* CplaytrikeView::GetDocument() const
   { return reinterpret_cast<CplaytrikeDoc*>(m_pDocument); }
#endif

