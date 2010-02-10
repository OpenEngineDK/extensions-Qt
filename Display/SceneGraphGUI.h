#ifndef _SCENE_GRAPH_GUI_H_
#define _SCENE_GRAPH_GUI_H_

#include <QtGui>
#include <Scene/ISceneNode.h>
#include <Scene/SceneNode.h>
#include <Scene/ISceneNodeVisitor.h>
#include <Core/EngineEvents.h>
#include <Core/IListener.h>

#include <Core/Event.h>
#include <Core/IEvent.h>

#include <Utils/SelectionSet.h>

namespace OpenEngine {
namespace Renderers { class TextureLoader; }
namespace Display {

using namespace OpenEngine::Scene;
using namespace OpenEngine::Core;
using namespace OpenEngine::Utils;
using namespace std;


class SceneGraphGUI : public QWidget
                    , public IListener<InitializeEventArg>
                    , public IListener<SelectionSet<ISceneNode >::ChangedEventArg > {
            
    Q_OBJECT;

    class SceneModel : public QAbstractItemModel {
        friend class SceneGraphGUI;
        ISceneNode* root;
        QModelIndexList* lastIndexes;

        SceneGraphGUI* gui;        

    public:
        SceneModel(ISceneNode* node, SceneGraphGUI* gui);
        ~SceneModel();
                
        Qt::DropActions supportedDropActions() const;
         QStringList mimeTypes() const;
         QMimeData* mimeData(const QModelIndexList &indexes) const;
        bool dropMimeData(const QMimeData *data,Qt::DropAction action, int row, int column, const QModelIndex &parent);        

        //bool insertRows ( int row, int count, const QModelIndex & parent);
        //bool removeRows ( int row, int count, const QModelIndex & parent);
        int rowCount(const QModelIndex& parent) const;
        int columnCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QModelIndex index(int row, int column, const QModelIndex& parent) const;
        QModelIndex parent(const QModelIndex& index) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;
        QModelIndex FindNode(ISceneNode *node);

    };
    


    ISceneNode* root;
    SceneModel* model;
    QTreeView* tv;
    Renderers::TextureLoader* textureLoader;
    SelectionSet<ISceneNode>& selectionList;
    //Event<NodeSelectionEventArg> selectionEvent;

public slots:
    void select(const QItemSelection & selected, const QItemSelection & deselected);
    void CreateSceneNode(QAction* act);
    void DeleteSceneNode();
    void LoadSceneNode();
    void SaveSceneNode();
    
public:
    
    SceneGraphGUI(ISceneNode* node, Renderers::TextureLoader* tl, SelectionSet<ISceneNode>& ss);
    ~SceneGraphGUI();
    
    void Handle(InitializeEventArg arg);
    void Handle(SelectionSet<ISceneNode>::ChangedEventArg arg);

    IEvent<SelectionSet<ISceneNode>::ChangedEventArg >& SelectionEvent();


private:
    unsigned int ignoredSelection;
    
};
    
}
}


#endif

// #define SG_USE_NEW 0

// #if SG_USE_NEW
// #include "SceneGraphGUI-new.h"
// #else
// #include "SceneGraphGUI-old.h"
// #endif
