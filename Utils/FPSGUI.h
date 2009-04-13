#ifndef _FPS_GUI_H_
#define _FPS_GUI_H_

#include <Core/IListener.h>
#include <Core/EngineEvents.h>

#include <QtGui>

namespace OpenEngine {
namespace Utils {

    using OpenEngine::Core::IListener;
    using OpenEngine::Core::ProcessEventArg;
    

    class FPSGUI : public IListener<ProcessEventArg>,
                   public QWidget {
      
    private:
        Timer timer;
        unsigned int interval;
        int frames;
        QLabel* label;

    public:
        FPSGUI(const unsigned int interval);
        void Handle(ProcessEventArg arg);

    };

}
}

#endif
