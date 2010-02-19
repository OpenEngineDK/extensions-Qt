#include "RenderStateNodeGUI.h"
#include <Scene/RenderStateNode.h>
#include "Qt/ui_RenderStateNodeGUI.h"

namespace OpenEngine {
namespace Display {

using namespace Scene;

int StateToIndex(RenderStateNode* node, RenderStateNode::RenderStateOption o) {
    if (node->IsOptionEnabled(o)) return 1;
    if (node->IsOptionDisabled(o)) return 2;
    return 0;
}

void SetState(int p, RenderStateNode* node, RenderStateNode::RenderStateOption o) {
    if (p == 0) node->InheritOption(o);
    if (p == 1) node->EnableOption(o);
    if (p == 2) node->DisableOption(o);
}

RenderStateNodeGUI::RenderStateNodeGUI() : node(NULL) {
    ui = new Ui::RenderStateNodeGUI();
    ui->setupUi(this);
}

void RenderStateNodeGUI::SetNode(RenderStateNode* node) {
    this->node = node;
    if (!node) return;
    ui->    texture_state->setCurrentIndex(StateToIndex(node, RenderStateNode::TEXTURE));
    ui->     shader_state->setCurrentIndex(StateToIndex(node, RenderStateNode::SHADER));
    ui->   backface_state->setCurrentIndex(StateToIndex(node, RenderStateNode::BACKFACE));
    ui->   lighting_state->setCurrentIndex(StateToIndex(node, RenderStateNode::LIGHTING));
    ui->color_material_state->setCurrentIndex(StateToIndex(node, RenderStateNode::COLOR_MATERIAL));
    ui-> depth_test_state->setCurrentIndex(StateToIndex(node, RenderStateNode::DEPTH_TEST));
    ui->  wireframe_state->setCurrentIndex(StateToIndex(node, RenderStateNode::WIREFRAME));
    ui->soft_normal_state->setCurrentIndex(StateToIndex(node, RenderStateNode::SOFT_NORMAL));
    ui->hard_normal_state->setCurrentIndex(StateToIndex(node, RenderStateNode::HARD_NORMAL));
    ui->   binormal_state->setCurrentIndex(StateToIndex(node, RenderStateNode::BINORMAL));
    ui->    tangent_state->setCurrentIndex(StateToIndex(node, RenderStateNode::TANGENT));
}

void RenderStateNodeGUI::SetTexture(int p)       { SetState(p, node, RenderStateNode::TEXTURE); }
void RenderStateNodeGUI::SetShader(int p)        { SetState(p, node, RenderStateNode::SHADER); }
void RenderStateNodeGUI::SetBackface(int p)      { SetState(p, node, RenderStateNode::BACKFACE); }
void RenderStateNodeGUI::SetLighting(int p)      { SetState(p, node, RenderStateNode::LIGHTING); }
void RenderStateNodeGUI::SetColorMaterial(int p) { SetState(p, node, RenderStateNode::COLOR_MATERIAL); }
void RenderStateNodeGUI::SetDepthTest(int p)     { SetState(p, node, RenderStateNode::DEPTH_TEST); }
void RenderStateNodeGUI::SetWireframe(int p)     { SetState(p, node, RenderStateNode::WIREFRAME); }
void RenderStateNodeGUI::SetSoftNormal(int p)    { SetState(p, node, RenderStateNode::SOFT_NORMAL); }
void RenderStateNodeGUI::SetHardNormal(int p)    { SetState(p, node, RenderStateNode::HARD_NORMAL); }
void RenderStateNodeGUI::SetBinormal(int p)      { SetState(p, node, RenderStateNode::BINORMAL); }
void RenderStateNodeGUI::SetTangent(int p)       { SetState(p, node, RenderStateNode::TANGENT); }

}
}
