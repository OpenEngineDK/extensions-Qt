#ifndef _QT_FPS_GUI_H_
#define _QT_FPS_GUI_H_

#include <Core/IListener.h>
#include <Core/EngineEvents.h>

#include <QtGui>

namespace OpenEngine {
namespace Utils {

class FPSGUI : public Core::IListener<Core::ProcessEventArg>,
               public QWidget {

private:
    Timer timer;
    unsigned int interval, decimal, frames;
    QLabel* label;
    
public:
    FPSGUI(const unsigned int interval = 1000000,
           const unsigned int decimal = 0);
    
    void Handle(Core::ProcessEventArg arg);

};

} // NS Utils
} // NS OpenEngine

#endif // _QT_FPS_GUI_H_
