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
    QtMouse*    mouse;
    QtKeyboard* keyboard;
    QtJoystick* joystick;

    // qt widgets
    QApplication* app;
    QWidget*      top;
    QGLWidget*    gl;
    QHBoxLayout*  lay;

};

} // NS Display
} // NS OpenEngine

#endif // _OE_QT_ENV_H_
