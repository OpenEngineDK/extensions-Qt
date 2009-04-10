//
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_QT_ENV_H_
#define _OE_QT_ENV_H_

#include <QtGui>
#include <QtOpenGL>

#include <Display/IEnvironment.h>
#include <Display/QtFrame.h>
#include <Core/EngineEvents.h>
#include <Logging/Logger.h>

// remove this!!
#include <Devices/IMouse.h>
#include <Devices/IKeyboard.h>
#include <Devices/IJoystick.h>
#include <Core/Event.h>

namespace OpenEngine {
namespace Display {

// remove this!!!
using namespace Devices;
using namespace Core;

/**
 * Short description.
 *
 * @class QtEnvironment QtEnvironment.h Display/QtEnvironment.h
 */
class QtEnvironment : public IEnvironment {
public:

    // classes for all devices (with private constructors)
    class QtMouse : public IMouse {
        friend class QtEnvironment;
        QtMouse();
        Event<MouseMovedEventArg>  mme;
        Event<MouseButtonEventArg> mbe;
    public:
        IEvent<MouseMovedEventArg>&  MouseMovedEvent();
        IEvent<MouseButtonEventArg>& MouseButtonEvent();
        void HideCursor();
        void ShowCursor();
        void SetCursor(int x, int y);
        MouseState GetState();
    };

    class QtKeyboard : public IKeyboard {
        friend class QtEnvironment;
        QtKeyboard();
        Event<KeyboardEventArg> ke;
    public:
        IEvent<KeyboardEventArg>& KeyEvent();
    };

    class QtJoystick : public IJoystick {
        friend class QtEnvironment;
        QtJoystick();
        Event<JoystickButtonEventArg> jbe;
        Event<JoystickAxisEventArg>   jae;
    public:
        IEvent<JoystickButtonEventArg>& JoystickButtonEvent();
        IEvent<JoystickAxisEventArg>&   JoystickAxisEvent();
    };

    class GLWidget : public QGLWidget {
    private:
        QtMouse*    mouse;
        QtKeyboard* keyboard;
        QtJoystick* joystick;

    public:
        GLWidget(QWidget* parent) : QGLWidget(parent) {
            mouse    = new QtMouse();
            keyboard = new QtKeyboard();
            joystick = new QtJoystick();
        }

        Key ConvertKey(Qt::Key key) {
            // numbers
            if (key>=Qt::Key_0 && key<=Qt::Key_9) {
                unsigned int value = (key - Qt::Key_0);
                //logger.info << "int:" << value << logger.end;
                return Key(value + KEY_0);
            }
            // chars
            if (key>=Qt::Key_A && key<=Qt::Key_Z) {
                unsigned int value = (key - Qt::Key_A);
                //logger.info << "char:" << value << logger.end;
                return Key(value + KEY_a);
            }

            // function keys
            if (key>=Qt::Key_F1 && key<=Qt::Key_F15) {
                unsigned int value = (key - Qt::Key_F1);
                //logger.info << "func key:" << value << logger.end;
                return Key(value + KEY_F1);
            }

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
                //logger.info << "modifier key: shift down" << logger.end;
            }
            if (mkey & Qt::ControlModifier) {
                rtn = KeyMod(rtn |KEY_LCTRL);
                rtn = KeyMod(rtn |KEY_RCTRL);
                mkey = Qt::KeyboardModifiers(mkey & ~Qt::ControlModifier);
                //logger.info << "modifier key: ctrl down" << logger.end;
            }
            if (mkey & Qt::AltModifier) {
                rtn = KeyMod(rtn |KEY_LALT);
                rtn = KeyMod(rtn |KEY_RALT);
                mkey = Qt::KeyboardModifiers(mkey & ~Qt::AltModifier);
                //logger.info << "modifier key: alt down" << logger.end;
            }
            if (mkey & Qt::MetaModifier) {
                rtn = KeyMod(rtn |KEY_LMETA);
                rtn = KeyMod(rtn |KEY_RMETA);
                mkey = Qt::KeyboardModifiers(mkey & ~Qt::MetaModifier);
                //logger.info << "modifier key: meta down" << logger.end;
            }

            if (mkey!=0)
                logger.warning << "MODIFIER KEY, not handled in Qt key handler"
                               << logger.end;
            return rtn;
        }

        void keyPressEvent(QKeyEvent* e) {
            //logger.info << "press event" << logger.end;
            KeyboardEventArg arg;
            arg.type = EVENT_PRESS;

            Qt::Key key = (Qt::Key)e->key();
            arg.sym = ConvertKey(key);

            Qt::KeyboardModifiers keym = (Qt::KeyboardModifiers)e->modifiers();
            arg.mod = ConvertModKey(keym);

            keyboard->ke.Notify( arg );
        }
        
        void keyReleaseEvent(QKeyEvent *e) {
            //logger.info << "release event" << logger.end;
            KeyboardEventArg arg;
            arg.type = EVENT_RELEASE;

            Qt::Key key = (Qt::Key)e->key();
            arg.sym = ConvertKey(key);

            Qt::KeyboardModifiers keym = (Qt::KeyboardModifiers)e->modifiers();
            arg.mod = ConvertModKey(keym);

            keyboard->ke.Notify( arg );
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

    QtEnvironment(int width  = 640,
                  int height = 480,
                  int depth  = 32,
                  FrameOption options = FrameOption());

    virtual ~QtEnvironment();

    void Handle(Core::InitializeEventArg arg);
    void Handle(Core::ProcessEventArg arg);
    void Handle(Core::DeinitializeEventArg arg);

    IFrame&             GetFrame();
    Devices::IMouse*    GetMouse();
    Devices::IKeyboard* GetKeyboard();
    Devices::IJoystick* GetJoystick();

private:

    // private device members
    QtFrame*    frame;

    // qt widgets
    QApplication* app;
    QWidget*      top;
    GLWidget*    gl;
    QHBoxLayout*  lay;

};

} // NS Display
} // NS OpenEngine

#endif // _OE_QT_ENV_H_
