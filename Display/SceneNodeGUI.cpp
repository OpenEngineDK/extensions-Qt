// Qt component to display a scene node.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Display/SceneNodeGUI.h>
#include <Scene/StrategyVisitor.h>
#include <Scene/SceneNodes.h>

namespace OpenEngine {
namespace Display {

using namespace Scene;

// ============================================================
// Some testing strategies to display node information.
// We will want to create these guis with the QtCreator
// ============================================================

class TransGui : public StrategyVisitor::Strategy<TransformationNode> {
    SceneNodeGUI& gui;
    QLayout* layout;
public:
    TransGui(SceneNodeGUI& gui) : gui(gui) {
        layout = new QHBoxLayout();
        layout->addWidget(new QLabel(QString("A transformation node is selected")));
    }
    ~TransGui() {
        delete layout;
    }
    void Visit(TransformationNode* node) {
        gui.setLayout(layout);
    }
};

class NodeGui : public StrategyVisitor::Strategy<SceneNode> {
    SceneNodeGUI& gui;
    QLayout* layout;
public:
    NodeGui(SceneNodeGUI& gui) : gui(gui) {
        layout = new QHBoxLayout();
        layout->addWidget(new QLabel(QString("A scene node is selected")));
    }
    ~NodeGui() {
        delete layout;
    }
    void Visit(SceneNode* node) {
        gui.setLayout(layout);
    }
};

// ============================================================
// A display visitor to find node dependent displays
// ============================================================

class DisplayVisitor : public StrategyVisitor {
    SceneNodeGUI& gui;
    QLayout* layout;
public:
    DisplayVisitor(SceneNodeGUI& gui) : gui(gui) {
        // Create a default layout for nodes without strategies
        layout = new QHBoxLayout();
        layout->addWidget(new QLabel(QString("Found no display for selected node")));
        // Set up node strategies
        SetSceneNodeStrategy(new NodeGui(gui));
        SetTransformationNodeStrategy(new TransGui(gui));
    }
    ~DisplayVisitor() {
        delete layout;
    }
    void DefaultVisitNode(ISceneNode* node) {
        gui.setLayout(layout);
    }
};


/**
 * Create a scene node gui display.
 * @param node Initial node to display [default: NULL for a none-display]
 */
SceneNodeGUI::SceneNodeGUI(ISceneNode* node) {
    // Create the strategy to select node displays
    visit = new DisplayVisitor(*this);
    // Create a "none" display that is used if no nodes are selected
    noneLayout = new QHBoxLayout();
    noneLayout->addWidget(new QLabel(QString("No scene node is selected")));
    // Set the node display or default to "none"
    if (node) node->Accept(*visit);
    else setLayout(noneLayout);
}

/**
 * Destructor.
 * Will delete internal layouts and strategies.
 */
SceneNodeGUI::~SceneNodeGUI() {
    delete visit;
    delete noneLayout;
}

/**
 * Set/change which node to display.
 * @param node Node to display [NULL for a none-display]
 */
void SceneNodeGUI::SetNode(ISceneNode* node) {
    if (node) node->Accept(*visit);
    else setLayout(noneLayout);
}

} // NS Display
} // NS OpenEngine
