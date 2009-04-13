//
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Display/QtEnvironment.h>
#include <Logging/Logger.h>
//#include <Display/QtResponder.h>


namespace OpenEngine {
namespace Display {

using namespace Devices;

// ============================================================
// Qt Environment
// ============================================================

QtEnvironment::QtEnvironment(int width,
                             int height,
                             int depth,
                             FrameOption options) {

    //argc inserted to avoid: QApplication: Invalid Display* argument
    int argc = 0;
    app = new QApplication(argc,0);
    top = new QWidget();
    gl  = new GLWidget(top);
    lay = new QHBoxLayout;


    //    QWidget *w1 = new QWidget();
    QVBoxLayout *l = new QVBoxLayout();
    leftLay = l;
    //QLineEdit *le = new QLineEdit();
    //QPushButton *btn = new QPushButton();

    //QtResponder *resp = new QtResponder();

    //QObject::connect(btn, SIGNAL(clicked()), resp, SLOT(doStuff()));

    // l->addWidget(le);
    // l->addWidget(btn);
    //    w1->setLayout(l);
    //le->show();

    frame    = new QtFrame(width, height, depth, options);

    lay->addLayout(l);
    lay->addWidget(gl);

    gl->setFixedSize(width,height);
    
    top->setLayout(lay);
    top->setWindowTitle("My Window");
    //top->move(0,0);
    top->show();


    //gl->grabKeyboard();
    gl->setFocus();
    gl->setMouseTracking(true);
}

void QtEnvironment::AddWidget(QWidget* w) {
    leftLay->addWidget(w);
}

QtEnvironment::~QtEnvironment() {
    /*
    delete joystick;
    delete keyboard;
    delete mouse;
    */
    delete frame;
    delete lay;
    delete gl;
    delete top;
    delete app;
}

void QtEnvironment::Handle(Core::InitializeEventArg arg) {
    // version
    int major = QT_VERSION >> 16;
    int minor = (QT_VERSION >> 8) & 0xFF;
    logger.info << "Qt version: " << major << "." << minor << logger.end;
}

void QtEnvironment::Handle(Core::ProcessEventArg arg) {
    app->processEvents();
    gl->swapBuffers();

    ((SDLJoystickOnly*)GetJoystick())->Handle(arg);
          
    //logger.info << SDL_JoystickGetAxis(firstStick, 0) << logger.end;
}

void QtEnvironment::Handle(Core::DeinitializeEventArg arg) {

}

IFrame& QtEnvironment::GetFrame() {
    return *frame;
}

IMouse* QtEnvironment::GetMouse() {
    return gl->GetMouse();
}

IKeyboard* QtEnvironment::GetKeyboard() {
    return gl->GetKeyboard();
}

IJoystick* QtEnvironment::GetJoystick() {
    return gl->GetJoystick();
}

// ============================================================
// Qt Mouse
// ============================================================

    QtEnvironment::QtMouse::QtMouse(QWidget *w) : widget(w) {

}

IEvent<MouseMovedEventArg>& QtEnvironment::QtMouse::MouseMovedEvent() {
    return mme;
}

IEvent<MouseButtonEventArg>& QtEnvironment::QtMouse::MouseButtonEvent() {
    return mbe;
}

void QtEnvironment::QtMouse::HideCursor() {    
    widget->setCursor(Qt::BlankCursor);
}

void QtEnvironment::QtMouse::ShowCursor() {
    widget->setCursor(Qt::ArrowCursor);
}

void QtEnvironment::QtMouse::SetCursor(int x, int y) {
    // invariant, we know that state contains the most current
    // position

    //QPoint absPoint = QCursor::pos();
    // QPoint origin(absPoint.x() - state.x,
    //               absPoint.y() - state.y);

    if (widget->hasFocus()) {

        QPoint p = widget->mapToGlobal(QPoint(x,y));

    
        QCursor::setPos(p);
    }
}

MouseState QtEnvironment::QtMouse::GetState() {
    return state;
}

// ============================================================
// Qt Keyboard
// ============================================================

QtEnvironment::QtKeyboard::QtKeyboard() {

}

IEvent<KeyboardEventArg>& QtEnvironment::QtKeyboard::KeyEvent() {
    return ke;
}

// ============================================================
// Qt Joystick
// ============================================================

QtEnvironment::QtJoystick::QtJoystick() {

}

IEvent<JoystickButtonEventArg>& QtEnvironment::QtJoystick::JoystickButtonEvent() {
    return jbe;
}

IEvent<JoystickAxisEventArg>& QtEnvironment::QtJoystick::JoystickAxisEvent() {
    return jae;
}

} // NS Display
} // NS OpenEngine
