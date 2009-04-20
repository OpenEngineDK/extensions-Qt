#include "TransformationNodeGUI.h"
#include <Scene/TransformationNode.h>

// Qt generated ui
#include "Qt/ui_TransformationNodeGUI.h"

namespace OpenEngine {
namespace Display {

using namespace Scene;

TransformationNodeGUI::TransformationNodeGUI() : node(NULL) {
    ui = new Ui::TransformationNodeGUI();
    ui->setupUi(this);

    // setup validation of input fields
    QDoubleValidator* v = new QDoubleValidator(this);
    ui->translate_x->setValidator(v);
    ui->translate_y->setValidator(v);
    ui->translate_z->setValidator(v);
    ui->rotate_x->setValidator(v);
    ui->rotate_y->setValidator(v);
    ui->rotate_z->setValidator(v);
    ui->scale_x->setValidator(v);
    ui->scale_y->setValidator(v);
    ui->scale_z->setValidator(v);
}

void TransformationNodeGUI::SetNode(TransformationNode* node) {
    this->node = node;
    if (!node) return;
    Vector<3,float> pos = node->GetPosition();
    Vector<3,float> scl = node->GetScale();
    Vector<3,float> rot = node->GetRotation().GetEulerAngles();

    ui->translate_x->setText(QString().setNum(pos[0]));
    ui->translate_y->setText(QString().setNum(pos[1]));
    ui->translate_z->setText(QString().setNum(pos[2]));

    ui->rotate_x->setText(QString().setNum(rot[0]));
    ui->rotate_y->setText(QString().setNum(rot[1]));
    ui->rotate_z->setText(QString().setNum(rot[2]));

    ui->scale_x->setText(QString().setNum(scl[0]));
    ui->scale_y->setText(QString().setNum(scl[1]));
    ui->scale_z->setText(QString().setNum(scl[2]));
}

void TransformationNodeGUI::Update() {
    if (!node) return;
    bool bx,by,bz;
    Vector<3,float> v;

    v[0] = ui->translate_x->text().toFloat(&bx);
    v[1] = ui->translate_y->text().toFloat(&by);
    v[2] = ui->translate_z->text().toFloat(&bz);
    if (bx && by && bz)
        node->SetPosition(v);

    v[0] = ui->scale_x->text().toFloat(&bx);
    v[1] = ui->scale_y->text().toFloat(&by);
    v[2] = ui->scale_z->text().toFloat(&bz);
    if (bx && by && bz)
        node->SetScale(v);

    v[0] = ui->rotate_x->text().toFloat(&bx);
    v[1] = ui->rotate_y->text().toFloat(&by);
    v[2] = ui->rotate_z->text().toFloat(&bz);    
    if (bx && by && bz)
        node->SetRotation(Quaternion<float>(v[0],v[1],v[2]));
}

}
}
