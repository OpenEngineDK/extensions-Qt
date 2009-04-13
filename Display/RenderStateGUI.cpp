#include "RenderStateGUI.h"
#include "Qt/ui_RenderStateUI.h"

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Display {

RenderStateGUI::RenderStateGUI(RenderStateNode& rn) : rnode(rn) {
    Ui::RenderStateGUI ui;
    
    ui.setupUi(this);
    
}


void RenderStateGUI::SetWireframe(bool t) {
    if (t)
        rnode.EnableOption(RenderStateNode::WIREFRAME);
    else 
        rnode.DisableOption(RenderStateNode::WIREFRAME);
}

void RenderStateGUI::SetBackface(bool t) {
    if (t)
        rnode.EnableOption(RenderStateNode::BACKFACE);
    else 
        rnode.DisableOption(RenderStateNode::BACKFACE);
}


void RenderStateGUI::SetTexture(bool t) {
    if (t)
        rnode.EnableOption(RenderStateNode::TEXTURE);
    else 
        rnode.DisableOption(RenderStateNode::TEXTURE);
}

void RenderStateGUI::SetLight(bool t) {
    if (t)
        rnode.EnableOption(RenderStateNode::LIGHTING);
    else 
        rnode.DisableOption(RenderStateNode::LIGHTING);
}

void RenderStateGUI::SetDepth(bool t) {
    if (t)
        rnode.EnableOption(RenderStateNode::DEPTH_TEST);
    else 
        rnode.DisableOption(RenderStateNode::DEPTH_TEST);
}

    
}
}
