#ifndef _RENDER_STATE_GUI_H_
#define _RENDER_STATE_GUI_H_

#include <Core/IListener.h>
#include <Core/EngineEvents.h>

#include <Scene/RenderStateNode.h>

#include <QtGui>

using namespace OpenEngine::Core;
using OpenEngine::Scene::RenderStateNode;

namespace OpenEngine {
namespace Display {

    class RenderStateGUI : public QWidget {
        RenderStateNode& rnode;

        Q_OBJECT;

    public slots:
        void SetWireframe(bool);
        void SetBackface(bool);
        void SetTexture(bool);
        void SetLight(bool);
        void SetDepth(bool);

    public:
        RenderStateGUI(RenderStateNode& rn);

    };

}
}

#endif
