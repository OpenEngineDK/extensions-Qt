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
    gl  = new QGLWidget(top);
    lay = new QHBoxLayout;

    frame    = new QtFrame(width, height, depth, options);
    mouse    = new QtMouse();
    keyboard = new QtKeyboard();
    joystick = new QtJoystick();

    lay->addWidget(gl);
    top->setLayout(lay);
    top->setWindowTitle("My Window");
    top->show();

}

QtEnvironment::~QtEnvironment() {
    delete joystick;
    delete keyboard;
    delete mouse;
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
}

void QtEnvironment::Handle(Core::DeinitializeEventArg arg) {

}

IFrame& QtEnvironment::GetFrame() {
    return *frame;
}

IMouse* QtEnvironment::GetMouse() {
    return mouse;
}

IKeyboard* QtEnvironment::GetKeyboard() {
    return keyboard;
}

IJoystick* QtEnvironment::GetJoystick() {
    return joystick;
}

// ============================================================
// Qt Mouse
// ============================================================

QtEnvironment::QtMouse::QtMouse() {

}

IEvent<MouseMovedEventArg>& QtEnvironment::QtMouse::MouseMovedEvent() {
    return mme;
}

IEvent<MouseButtonEventArg>& QtEnvironment::QtMouse::MouseButtonEvent() {
    return mbe;
}

void QtEnvironment::QtMouse::HideCursor() {

}

void QtEnvironment::QtMouse::ShowCursor() {

}

void QtEnvironment::QtMouse::SetCursor(int x, int y) {

}

MouseState QtEnvironment::QtMouse::GetState() {
    MouseState state;
    state.x = 0;
    state.y = 0;
    state.buttons = BUTTON_NONE;
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
