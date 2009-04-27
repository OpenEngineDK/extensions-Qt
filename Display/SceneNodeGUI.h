// Qt component to display a scene node.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_QT_SCENE_NODE_GUI_H_
#define _OE_QT_SCENE_NODE_GUI_H_

#include <QtGui>
#include <Display/SceneGraphGUI.h>
#include <Utils/SelectionSet.h>

namespace OpenEngine {
namespace Scene { class ISceneNode; class StrategyVisitor; }
namespace Display {

using OpenEngine::Utils::SelectionSet;

/**
 * Qt component to display a scene node.
 *
 * @class SceneNodeGUI SceneNodeGUI.h Display/SceneNodeGUI.h
 */
    class SceneNodeGUI : public QStackedWidget,
                         public IListener<SelectionSet<ISceneNode>::ChangedEventArg> {
private:
    Scene::StrategyVisitor* visit;
    QWidget* noneWidget;
    
public:
    SceneNodeGUI(Scene::ISceneNode* node = NULL);
    virtual ~SceneNodeGUI();
    void SetNode(Scene::ISceneNode* node);
    void Handle(SelectionSet<ISceneNode>::ChangedEventArg arg);
 
};

} // NS Display
} // NS OpenEngine

#endif // _OE_QT_SCENE_NODE_GUI_H_
