// OpenGLRenderer.cpp : implementation file
//
#include "pch.h"
#include "stdafx.h"
//#include "OpenGL_MFCDialog.h"
#include "OpenGLRenderer.h"
//#include "afxdialogex.h"


// OpenGLRenderer

OpenGLRenderer::OpenGLRenderer()
{
	m_iWidth = 200;
	m_iHeight =200;
	m_iViewPortWidth =200;
	m_iViewPortHeight=200;

	m_hdc = NULL;
}

OpenGLRenderer::~OpenGLRenderer()
{
}


BEGIN_MESSAGE_MAP(OpenGLRenderer, CWnd)
	ON_WM_SIZE()
	//ON_WM_TIMER()
END_MESSAGE_MAP()


// OpenGLRenderer message handlers

bool OpenGLRenderer::CreateGLContext(CRect rect, CWnd *parent)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(WHITE_BRUSH), NULL); //default background colour
	CreateEx(0, className, _T("OpenGL with MFC/CDialog"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	SetViewPortSize(rect.Width(), rect.Height());

	m_WindowOld = rect;
	m_WindowOrig = rect;
	if(!InitContext() )
	{
		MessageBox(_T("ERROR Creating InitContext"));
		return false;
	};
	return true;
}


bool OpenGLRenderer::InitContext()
{
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 32;
        pfd.iLayerType = PFD_MAIN_PLANE;
		
		m_hdc = GetDC()->m_hDC;

        int nPixelFormat = ChoosePixelFormat(m_hdc, &pfd); 
        if (nPixelFormat == 0) return false;
        BOOL bResult = SetPixelFormat (m_hdc, nPixelFormat, &pfd);
        if (!bResult) return false; 
 
        HGLRC tempContext = wglCreateContext(m_hdc);
		wglMakeCurrent( m_hdc, tempContext);
 	
		GLenum GlewInitResult;
		glewExperimental = GL_TRUE;
		GlewInitResult = glewInit();
		
        if (GlewInitResult != GLEW_OK)
        {
                AfxMessageBox(_T("GLEW is not initialized!"));
        }
 
        int attribs[] =
        {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_FLAGS_ARB, 0,
                0
        };
 
        if(wglewIsSupported("WGL_ARB_create_context") == 1)
        {
                m_hrc = wglCreateContextAttribsARB(m_hdc,0, attribs);
                wglMakeCurrent(NULL,NULL);
                wglDeleteContext(tempContext);
                wglMakeCurrent(m_hdc, m_hrc);
        }
        else
        {       //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
                m_hrc = tempContext;
        }
 
        if (!m_hrc)
			return false;

		CString str;
		str.Format(_T("OpenGL version: %s\n"),(CString)glGetString(GL_VERSION));
		TRACE(str);
		return true;
}

void OpenGLRenderer::PrepareScene()
{
	wglMakeCurrent(m_hdc, m_hrc);
	glClearColor(0.0, 0.0, 1.0, 0.0); //background to clear with.

	//do other preparations here

	wglMakeCurrent(NULL, NULL);
}

void OpenGLRenderer::Reshape(UINT nType, int w, int h)
{
	OnSize(nType, w, h);
}


void OpenGLRenderer::OnSize(UINT nType, int cx, int cy)
{
	if(m_hdc != NULL)
	{
		wglMakeCurrent(m_hdc, m_hrc);
		//---------------------------------
		glViewport(0, 0, (GLsizei)cx, (GLsizei)cy); 
		//---------------------------------

		switch (nType)
		{
			case SIZE_RESTORED:
			case SIZE_MAXIMIZED:
				{
					MoveWindow(m_WindowOld.left,m_WindowOld.top,GetViewPortWidth(),GetViewPortHeight(),TRUE);
					break;
				}
			default:
				break;
		}
	}
	wglMakeCurrent(NULL, NULL);
}


void OpenGLRenderer::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
		{
			// Clear color and depth buffer bits
			glClear(GL_COLOR_BUFFER_BIT );

			// Draw OpenGL scene
			DrawScene();
			// Swap buffers
			SwapBuffers(m_hdc);
			break;
		}
		default:
			break;
	}
	CWnd::OnTimer(nIDEvent);
}


void OpenGLRenderer::DrawScene()
{
 	//////---------------------------------
	wglMakeCurrent(NULL, NULL);

	wglMakeCurrent(m_hdc, m_hrc);
	//--------------------------------
	glClear(GL_COLOR_BUFFER_BIT);
	
	for(int i=0;i<2;i++)
	{
		glBindVertexArray(m_vaoID[i]); 
		glDrawArrays(GL_LINE_LOOP, 0, m_GLSizeCount);
	}
	//--------------------------------
	glFlush();
	SwapBuffers(m_hdc);
	wglMakeCurrent(NULL, NULL);
}


void OpenGLRenderer::SetData(int iType)
{
	const int SQUARE = 1;
	//const int CUBE = 2;
	const int TRIANGLE = 3;
	m_iShapeType = iType;

	switch(iType)
	{
		case SQUARE:
			{
				SetSquare();
			}
			break;
		case TRIANGLE:
			{
				SetTriangle();
			}
			break;
		default:
			SetTriangle();
	}
}


void OpenGLRenderer::SetSquare()
{
	wglMakeCurrent(m_hdc, m_hrc);

		// First simple object
		m_GLSizeCount = 4;
		m_GLIntSize = 4;
		GLfloat Square[] = 
		{
			-0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f,
			0.5f,  0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f
		};

		//VAO allocation
		glGenVertexArrays(1, &m_vaoID[0]);
 
		// First VAO setup
		glBindVertexArray(m_vaoID[0]);
		glGenBuffers(1, &m_vboID[0]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Square), Square, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, m_GLIntSize, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(0);
	
	wglMakeCurrent(NULL, NULL);
}




void OpenGLRenderer::SetTriangle()
{
	wglMakeCurrent(m_hdc, m_hrc);

		m_GLIntSize = 4; //number of floats per item. i.e. number of floats per point in space.
		m_GLSizeCount = 3; //number of items. ie sizeof(TriangleA)/m_GLIntSize

		GLfloat TriangleA[] = 
		{
			-0.3f, 0.5f, -1.0f, 1.0f,
			-0.8f, -0.5f, -1.0f, 1.0f,
			0.2f, -0.5f, -1.0f, 1.0f
		};
	
		GLfloat TriangleB[] = 
		{
			-0.2f,  0.5f, -1.0f, 1.0f,
			0.3f,  -0.5f, -1.0f, 1.0f,
			0.8f, 0.5f, -1.0f, 1.0f,
		};

		//VAOs allocation
		glGenVertexArrays(2, &m_vaoID[0]);

		// First VAO setup
		glBindVertexArray(m_vaoID[0]);
		glGenBuffers(1, m_vboID); //VBO allocation
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleA), TriangleA, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, m_GLIntSize, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(0);

		// Second VAO setup     
		glBindVertexArray(m_vaoID[1]);
		glGenBuffers(1, &m_vboID[1]); //VBO allocation
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleB), TriangleB, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, m_GLIntSize, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(0);

	wglMakeCurrent(NULL, NULL);
}


void OpenGLRenderer::DestroyScene()
{
	DestroyWindow();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(3, m_vboID);

    glBindVertexArray(0);
    glDeleteVertexArrays(2, m_vaoID);

	wglMakeCurrent(NULL, NULL);
	if(m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
