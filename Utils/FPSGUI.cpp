#include <Utils/FPSGUI.h>


namespace OpenEngine {
namespace Utils {

    
    FPSGUI::FPSGUI(unsigned int interval)
        : interval(interval)
        , frames(0) {
        
        
        timer.Start();
        

        label = new QLabel();
        label->setText(QString("hello"));

        QVBoxLayout* l = new QVBoxLayout();
        l->addWidget(label);
        setLayout(l);

    }
    
    void FPSGUI::Handle(ProcessEventArg arg) {
        frames += 1;
        unsigned int elapsed = timer.GetElapsedTime().AsInt();
        
        if (elapsed > interval) {
            
            double fps =  (double)frames * 1000000 / (double)elapsed;
            label->setText(QString::number(fps));
            frames = 0;
            timer.Reset();
        }
    }
    

    }
}
