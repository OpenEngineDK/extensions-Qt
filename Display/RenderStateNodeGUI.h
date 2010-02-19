#ifndef _OE_QT_RENDER_STATE_NODE_GUI_H_
#define _OE_QT_RENDER_STATE_NODE_GUI_H_

#include <QtGui>

namespace Ui { class RenderStateNodeGUI; }
namespace OpenEngine {
namespace Scene { class RenderStateNode; }
namespace Display {

class RenderStateNodeGUI : public QWidget {
    Q_OBJECT;
    Ui::RenderStateNodeGUI* ui;
    Scene::RenderStateNode* node;

public:
    RenderStateNodeGUI();
    void SetNode(Scene::RenderStateNode* node);

public slots:
    void SetTexture(int);
    void SetShader(int);
    void SetBackface(int);
    void SetLighting(int);
    void SetColorMaterial(int);
    void SetDepthTest(int);
    void SetWireframe(int);
    void SetSoftNormal(int);
    void SetHardNormal(int);
    void SetBinormal(int);
    void SetTangent(int);

};

}
}

#endif // _OE_QT_RENDER_STATE_NODE_GUI_H_
