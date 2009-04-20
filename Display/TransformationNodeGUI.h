#ifndef _OE_QT_TRANSFORMATION_NODE_GUI_H_
#define _OE_QT_TRANSFORMATION_NODE_GUI_H_

#include <QtGui>

namespace Ui { class TransformationNodeGUI; }

namespace OpenEngine {

namespace Scene { class TransformationNode; }

namespace Display {

class TransformationNodeGUI : public QWidget {
    Q_OBJECT;
    Ui::TransformationNodeGUI* ui;
    Scene::TransformationNode* node;

public:
    TransformationNodeGUI();
    void SetNode(Scene::TransformationNode* node);

public slots:
    void Update();

};

}
}

#endif // _OE_QT_TRANSFORMATION_NODE_GUI_H_
