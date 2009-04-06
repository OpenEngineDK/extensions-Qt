// QT implementation of IFrame
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Core/Exceptions.h>
#include <Display/QtFrame.h>

#include <QtGui>
#include <QtOpenGL>

#include <string>

namespace OpenEngine {
namespace Display {

using OpenEngine::Core::Exception;
using std::string;
    
QtFrame::QtFrame(int width, int height,
                 int depth, FrameOption options)
    : IFrame(width, height, depth, options),
      width(width),
      height(height),
      depth(depth),
      options(FrameOption(options|FRAME_OPENGL)),
      init(false) {}

QtFrame::~QtFrame() {

}

bool QtFrame::IsFocused() const {
    return true; //return ((QT_GetAppState() & QT_APPINPUTFOCUS) != 0);
}

unsigned int QtFrame::GetWidth() const {
    return width;
}

unsigned int QtFrame::GetHeight() const {
    return height;
}

unsigned int QtFrame::GetDepth() const {
    return depth;
}

FrameOption QtFrame::GetOptions() const {
    return options;
}

bool QtFrame::GetOption(const FrameOption option) const {
    return (option & GetOptions()) == option;
}

void QtFrame::SetWidth(const int width) {
    if (!init) this->width = width;
}

void QtFrame::SetHeight(const int height) {
    if (!init) this->height = height;
}

void QtFrame::SetDepth(const int depth) {
    if (!init) this->depth = depth;
}

void QtFrame::SetOptions(const FrameOption options) {
    bool reload = false;
    if (options != this->options) reload = true;
    this->options = options;
    if (init && reload) {
        CreateSurface();
    }
}

void QtFrame::ToggleOption(const FrameOption option) {
    FrameOption opt = FrameOption(options ^ option);
    SetOptions(opt);
}

void QtFrame::CreateSurface() {
    // Set QT flags
    //Uint32 flags = options;

    // Use hardware rendered surface as default
    //flags |= QT_HWSURFACE;

    // Create window
    //if (QT_SetVideoMode(width, height, depth, flags) == NULL)
    //throw Exception("QT_SetVideoMode: " + string(QT_GetError()));

    /* @todo
      use signaling to reload gl context objects
      
      see, user comment 2 on:
      http://www.libsdl.org/cgi/docwiki.cgi/QT_SetVideoMode
      causes gl context to unload: textures and vertex arrays
    */
    // QWidget* window = new QWidget();
    // Widget *native = new Widget(&helper, this);
    // GLWidget *openGL = new GLWidget(&helper, this);
    // QLabel *nativeLabel = new QLabel(tr("Native"));
    // nativeLabel->setAlignment(Qt::AlignHCenter);
    // QLabel *openGLLabel = new QLabel(tr("OpenGL"));
    // openGLLabel->setAlignment(Qt::AlignHCenter);
    
    // QGridLayout *layout = new QGridLayout;
    // layout->addWidget(native, 0, 0);
    // layout->addWidget(openGL, 0, 1);
    // layout->addWidget(nativeLabel, 1, 0);
    // layout->addWidget(openGLLabel, 1, 1);
    // setLayout(layout);
    
    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), native, SLOT(animate()));
    //connect(timer, SIGNAL(timeout()), openGL, SLOT(animate()));
    //timer->start(50);

    // QApplication app(0,0);
    // QWidget* win = new QWidget();
    // QGLWidget* gl = new QGLWidget(win);

    // QHBoxLayout *lay = new QHBoxLayout;
    // lay->addWidget(gl);
    // win->setLayout(lay);
    // win->setWindowTitle("2D Painting on Native and OpenGL Widgets");
    // win->show();
    // app.exec();
}

void QtFrame::Handle(InitializeEventArg arg) {
    // Initialize the video frame
    //if (QT_Init(QT_INIT_VIDEO) < 0 )
    //throw Exception("QT_Init: " + string(QT_GetError()));

    CreateSurface();

    // Set the private initialization flag
    init = true;
}

void QtFrame::Handle(ProcessEventArg arg) {
    // Start by flipping the screen which is the
    // result from last engine loop.
    //if (IsOptionSet(FRAME_OPENGL))
    //QT_GL_SwapBuffers();
}

void QtFrame::Handle(DeinitializeEventArg arg) {
    //QT_QuitSubSystem(QT_INIT_VIDEO);
}

} // NS Display
} // NS OpenGL
