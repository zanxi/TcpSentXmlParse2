#pragma once

#include "stdafx.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib") //make sure project settings have: Linker -> Additional Library Directories -> include path for library.
#include "glew.h"
#include "wglew.h"


// OpenGLRenderer

class OpenGLRenderer : public CWnd
{
public:
	OpenGLRenderer();
	virtual ~OpenGLRenderer();
	bool CreateGLContext(CRect rect, CWnd *parent);
	void PrepareScene();				// Scene preparation
	void Reshape(UINT nType, int w, int h); //maps to OnSize()
	void DrawScene();				// Draw scene
	void DestroyScene();            // Cleanup
	
	void SetViewPortSize(int ViewPortWidth, int ViewPortHeight) { m_iViewPortWidth = ViewPortWidth; m_iViewPortHeight = ViewPortHeight; };

	CRect &GetWinRect() { return m_WindowOrig; };
	int &GetViewPortWidth() { return m_iViewPortWidth; };
	int &GetViewPortHeight() { return m_iViewPortHeight; };

	void SetData(int iType);  // Creates VAO and VBOs and fill them with data

	UINT_PTR m_unpTimer;

public:
	void SetTriangle();
	void SetSquare();
	bool InitContext();					 // Creates OpenGL Rendering Context
	afx_msg void OnSize(UINT nType, int cx, int cy);  // Changing viewport
	afx_msg void OnTimer(UINT nIDEvent);

	HGLRC m_hrc;                        // OpenGL Rendering Context 
	HDC m_hdc;

	CRect m_rect;
	CRect m_WindowOld;
	CRect m_WindowOrig;

	int m_iWidth, m_iHeight;
	int	m_iViewPortWidth, m_iViewPortHeight;

	GLuint m_vaoID[2];                      // 2 vertex array objects
	GLuint m_vboID[2];                      // 2 VBOs
	int m_GLIntSize;
	int m_GLSizeCount;
	int m_iShapeType; //1,3 for triangle & square.

	DECLARE_MESSAGE_MAP()
};
