#include <Utils/FPSGUI.h>

namespace OpenEngine {
namespace Utils {

using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;

FPSGUI::FPSGUI(const unsigned int interval,
               const unsigned int decimal)
    : interval(interval)
    , decimal(decimal)
    , frames(0) {
    timer.Start();
    label = new QLabel(QString::number(0));
    QHBoxLayout* l = new QHBoxLayout();
    l->addWidget(label);
    setLayout(l);
}

void FPSGUI::Handle(ProcessEventArg arg) {
    frames += 1;
    unsigned int elapsed = timer.GetElapsedTime().AsInt();
    if (elapsed > interval) {
        double fps =  (double)frames * 1000000 / (double)elapsed;
        label->setText(QString::number(fps, 'f', decimal));
        frames = 0;
        timer.Reset();
    }
}

} // NS Utils
} // NS OpenEngine
