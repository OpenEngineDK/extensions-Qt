// Environment for the Qt toolkit.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Display/QtEnvironment.h>

#include <Core/Event.h>

#include <Devices/IMouse.h>
#include <Devices/IKeyboard.h>
#include <Devices/SDLJoystickOnly.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Display {

using namespace Devices;

// ============================================================
// Qt Mouse
// ============================================================

class QtMouse : public IMouse {
public:
    Event<MouseMovedEventArg>  mme;
    Event<MouseButtonEventArg> mbe;
    MouseState state;
    QWidget* widget;

    QtMouse(QWidget* w) : widget(w) {
        state.x = 0;
        state.y = 0;
        state.buttons = BUTTON_NONE;
    }
    
    IEvent<MouseMovedEventArg>&  MouseMovedEvent()  { return mme; }
    IEvent<MouseButtonEventArg>& MouseButtonEvent() { return mbe; }

    MouseState GetState() { return state; }

    void HideCursor() { widget->setCursor(Qt::BlankCursor); }
    void ShowCursor() { widget->setCursor(Qt::ArrowCursor); }
    void SetCursor(int x, int y) {
        if (!widget->hasFocus()) return;
        QPoint p = widget->mapToGlobal(QPoint(x,y));
        QCursor::setPos(p);
    }
};

// ============================================================
// Qt Keyboard
// ============================================================

class QtKeyboard : public IKeyboard {
public:
    QtKeyboard() { }
    Event<KeyboardEventArg> ke;
    IEvent<KeyboardEventArg>& KeyEvent() { return ke; }
};

// ============================================================
// GL wrapper class
// ============================================================

class QtEnvironment::GLWidget : public QGLWidget {
public:
    QtMouse*         mouse;
    QtKeyboard*      keyboard;
    SDLJoystickOnly* joystick;

    GLWidget(QWidget* parent) : QGLWidget(parent) {
        mouse    = new QtMouse(this);
        keyboard = new QtKeyboard();
        joystick = new SDLJoystickOnly();
        setFocusPolicy(Qt::StrongFocus);
    }

    Key ConvertKey(Qt::Key key) {
        // numbers
        if (key>=Qt::Key_0 && key<=Qt::Key_9) {
            unsigned int value = (key - Qt::Key_0);
            return Key(value + KEY_0);
        }
        // chars
        if (key>=Qt::Key_A && key<=Qt::Key_Z) {
            unsigned int value = (key - Qt::Key_A);
            return Key(value + KEY_a);
        }
        // function keys
        if (key>=Qt::Key_F1 && key<=Qt::Key_F15) {
            unsigned int value = (key - Qt::Key_F1);
            return Key(value + KEY_F1);
        }
        // special keys
        if (key == Qt::Key_Escape) return KEY_ESCAPE;
        if (key == Qt::Key_Tab) return KEY_TAB;
        if (key == Qt::Key_Backspace) return KEY_BACKSPACE;
        if (key == Qt::Key_Return) return KEY_RETURN;
        if (key == Qt::Key_Insert) return KEY_INSERT;
        if (key == Qt::Key_Delete) return KEY_DELETE;
        if (key == Qt::Key_Pause) return KEY_PAUSE;
        if (key == Qt::Key_Print) return KEY_PRINT;
        if (key == Qt::Key_SysReq) return KEY_SYSREQ;
        if (key == Qt::Key_Home) return KEY_HOME;
        if (key == Qt::Key_End) return KEY_END;
        if (key == Qt::Key_Left) return KEY_LEFT;
        if (key == Qt::Key_Right) return KEY_RIGHT;
        if (key == Qt::Key_Down) return KEY_DOWN;
        if (key == Qt::Key_Up) return KEY_UP;
        if (key == Qt::Key_PageUp) return KEY_PAGEUP;
        if (key == Qt::Key_PageDown) return KEY_PAGEDOWN;
        if (key == Qt::Key_Shift) return KEY_LSHIFT;
        if (key == Qt::Key_Control) return KEY_LCTRL;
        if (key == Qt::Key_Meta) return KEY_LMETA;
        if (key == Qt::Key_Alt) return KEY_LALT;
        if (key == Qt::Key_AltGr) return KEY_RALT;
        if (key == Qt::Key_CapsLock) return KEY_CAPSLOCK;
        if (key == Qt::Key_NumLock) return KEY_NUMLOCK;
        if (key == Qt::Key_ScrollLock) return KEY_SCROLLOCK;
        if (key == Qt::Key_Plus) return KEY_PLUS;
        if (key == Qt::Key_Comma) return KEY_COMMA;
        if (key == Qt::Key_Minus) return KEY_MINUS;
        if (key == Qt::Key_Period) return KEY_PERIOD;
        if (key == Qt::Key_Slash) return KEY_SLASH;
        if (key == Qt::Key_Colon) return KEY_COLON;
        if (key == Qt::Key_Semicolon) return KEY_SEMICOLON;
        if (key == Qt::Key_Space) return KEY_SPACE;
        // what?
        logger.warning << "unknown key in qt handler:" << key << logger.end;
        return KEY_UNKNOWN;
    }

    KeyMod ConvertModKey(Qt::KeyboardModifiers mkey) {
        KeyMod rtn = KEY_MOD_NONE;
        if (!mkey)
            return rtn;
        if (mkey & Qt::ShiftModifier) {
            rtn = KeyMod(rtn |KEY_LSHIFT);
            rtn = KeyMod(rtn |KEY_RSHIFT);
            mkey = Qt::KeyboardModifiers(mkey & ~Qt::ShiftModifier);
        }
        if (mkey & Qt::ControlModifier) {
            rtn = KeyMod(rtn |KEY_LCTRL);
            rtn = KeyMod(rtn |KEY_RCTRL);
            mkey = Qt::KeyboardModifiers(mkey & ~Qt::ControlModifier);
        }
        if (mkey & Qt::AltModifier) {
            rtn = KeyMod(rtn |KEY_LALT);
            rtn = KeyMod(rtn |KEY_RALT);
            mkey = Qt::KeyboardModifiers(mkey & ~Qt::AltModifier);
        }
        if (mkey & Qt::MetaModifier) {
            rtn = KeyMod(rtn |KEY_LMETA);
            rtn = KeyMod(rtn |KEY_RMETA);
            mkey = Qt::KeyboardModifiers(mkey & ~Qt::MetaModifier);
        }
        if (mkey & Qt::KeypadModifier) {
            // TODO how do we get this into our version?
            mkey = Qt::KeyboardModifiers(mkey & ~Qt::KeypadModifier);
        }

        if (mkey!=0)
            logger.warning << "MODIFIER KEY, not handled in Qt key handler: "
                           << mkey
                           << logger.end;
        return rtn;
    }

    // function triggered by Qt
    void keyPressEvent(QKeyEvent* e) {
        KeyboardEventArg arg;
        arg.type = EVENT_PRESS;

        Qt::Key key = (Qt::Key)e->key();
        arg.sym = ConvertKey(key);

        Qt::KeyboardModifiers keym = (Qt::KeyboardModifiers)e->modifiers();
        arg.mod = ConvertModKey(keym);

        keyboard->ke.Notify( arg );
    }
        
    // function triggered by Qt
    void keyReleaseEvent(QKeyEvent* e) {
        KeyboardEventArg arg;
        arg.type = EVENT_RELEASE;

        Qt::Key key = (Qt::Key)e->key();
        arg.sym = ConvertKey(key);

        Qt::KeyboardModifiers keym = (Qt::KeyboardModifiers)e->modifiers();
        arg.mod = ConvertModKey(keym);

        keyboard->ke.Notify( arg );
    }

    MouseButton ConvertMouseButton(Qt::MouseButtons buttons) {
        MouseButton rtn = BUTTON_NONE;

        if (buttons == Qt::NoButton)
            return rtn;
        if (buttons & Qt::LeftButton) {
            rtn = MouseButton(rtn | BUTTON_LEFT);
            buttons = Qt::MouseButtons(buttons & ~Qt::LeftButton);
        }
        if (buttons & Qt::RightButton) {
            rtn = MouseButton(rtn | BUTTON_RIGHT);
            buttons = Qt::MouseButtons(buttons & ~Qt::RightButton);
        }
        if (buttons & Qt::MidButton) {
            rtn = MouseButton(rtn | BUTTON_MIDDLE);
            buttons = Qt::MouseButtons(buttons & ~Qt::MidButton);
        }
        if (buttons!=0)
            logger.warning <<
                "UNKNOWN MOUSE BUTTON, not handled in Qt key handler"
                           << logger.end;
        return rtn;
    }

    // function triggered by Qt
    void mousePressEvent(QMouseEvent* e) {
        // set mouse position and get button modifiers
        mouse->state.buttons = ConvertMouseButton(e->buttons());
        mouse->state.x = e->x();
        mouse->state.y = e->y();

        // create a mouse event
        MouseButtonEventArg marg;
        marg.state = mouse->state;
        marg.button = ConvertMouseButton(e->button());
        marg.type = EVENT_PRESS;

        // send the event
        mouse->mbe.Notify( marg );
    }

    // function triggered by Qt
    void mouseReleaseEvent(QMouseEvent* e) {
        // set mouse position and get button modifiers
        mouse->state.buttons = ConvertMouseButton(e->buttons());
        mouse->state.x = e->x(); // todo: should it be e->globalX() ???
        mouse->state.y = e->y(); // todo: should it be e->globalY() ???

        // create a mouse event
        MouseButtonEventArg marg;
        marg.state = mouse->state;
        marg.button = ConvertMouseButton(e->button());
        marg.type = EVENT_RELEASE;

        // send the event
        mouse->mbe.Notify( marg );
    }

    void wheelEvent(QWheelEvent* e) {
        // set mouse position and get button modifiers
        mouse->state.buttons = ConvertMouseButton(e->buttons());
        mouse->state.x = e->x();
        mouse->state.y = e->y();

        // create a mouse event
        MouseButtonEventArg marg;
        marg.state = mouse->state;

        int delta = e->delta(); //todo:use the delta to generate more events
        if (delta > 0) {
            marg.button = BUTTON_WHEEL_UP;
        } else {
            marg.button = BUTTON_WHEEL_DOWN;
        }
        marg.type = EVENT_ANY; // todo: ?!?

        // send the event
        mouse->mbe.Notify( marg );
    }


    void mouseMoveEvent(QMouseEvent* e) {
        // set mouse position and get button modifiers
        MouseMovedEventArg mmarg;
        mouse->state.buttons = mmarg.buttons;
        mmarg.x = e->x();
        mmarg.y = e->y();
        mmarg.dx = mmarg.x - mouse->state.x;
        mmarg.dy = mmarg.y - mouse->state.y;
        mouse->state.x = mmarg.x;
        mouse->state.y = mmarg.y;
        mouse->mme.Notify(mmarg);
    }

    IMouse* GetMouse() {
        return mouse;
    }

    IKeyboard* GetKeyboard() {
        return keyboard;
    }

    IJoystick* GetJoystick() {
        return joystick;
    }
};

// ============================================================
// Qt Environment
// ============================================================

QtEnvironment::QtEnvironment(int width,
                             int height,
                             int depth,
                             FrameOption options) {

    // Create some dummy arguments
    // (must be valid throughout the life of the application)
    int*  argc = new int(1);
    char* argv = new char('\0');
    app = new QApplication(*argc, &argv);
    top = new QWidget();
    gl  = new GLWidget(top);
    lay = new QHBoxLayout();
    leftLay = new QVBoxLayout();
    frame = new QtFrame(width, height, depth, options);

    lay->addLayout(leftLay);
    lay->addWidget(gl);

    gl->setFixedSize(width,height);
    
    top->setLayout(lay);
    top->setWindowTitle("My Window");
    top->show();

    gl->setFocus();
    gl->setMouseTracking(true);
}

void QtEnvironment::AddWidget(QWidget* w) {
    leftLay->addWidget(w);
}

QtEnvironment::~QtEnvironment() {
    delete frame;
    delete lay;
    delete gl;
    delete top;
    delete app;
}

void QtEnvironment::Handle(Core::InitializeEventArg arg) {
    // print version
    int major = QT_VERSION >> 16;
    int minor = (QT_VERSION >> 8) & 0xFF;
    logger.info << "Qt version: " << major << "." << minor << logger.end;
}

void QtEnvironment::Handle(Core::ProcessEventArg arg) {
    app->processEvents();
    gl->swapBuffers();
    gl->joystick->Handle(arg);
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

} // NS Display
} // NS OpenEngine
