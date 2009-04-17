// Environment for the Qt toolkit.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_QT_ENV_H_
#define _OE_QT_ENV_H_

// hack so the Qt Display typedef does not conflict with our name-space
#define Display QtShadowedDisplay
#include <QtGui>
#include <QtOpenGL>
#undef Display

#include <Display/IEnvironment.h>
#include <Display/QtFrame.h>
#include <Core/EngineEvents.h>

namespace OpenEngine {
namespace Display {

// remove this!!!
using namespace Devices;
using namespace Core;

/**
 * Environment for the Qt toolkit.
 *
 * @class QtEnvironment QtEnvironment.h Display/QtEnvironment.h
 */
class QtEnvironment : public IEnvironment {
public:

    QtEnvironment(int width  = 640,
                  int height = 480,
                  int depth  = 32,
                  FrameOption options = FrameOption());

    virtual ~QtEnvironment();

    void Handle(Core::InitializeEventArg arg);
    void Handle(Core::ProcessEventArg arg);
    void Handle(Core::DeinitializeEventArg arg);

    void AddWidget(QWidget* w);

    IFrame&             GetFrame();
    Devices::IMouse*    GetMouse();
    Devices::IKeyboard* GetKeyboard();
    Devices::IJoystick* GetJoystick();

private:

    // private forward declaration
    class GLWidget;

    // private device members
    QtFrame*    frame;

    // qt widgets
    QApplication* app;
    QWidget*      top;
    GLWidget*     gl;
    QHBoxLayout*  lay;
    QLayout*      leftLay;

};

} // NS Display
} // NS OpenEngine

#endif // _OE_QT_ENV_H_
