
#include "SceneGraphGUI.h"

#include "Qt/ui_SceneGraphUI.h"

#include <Scene/SceneNodes.h>
#include <Resources/ResourceManager.h>
#include <Resources/IModelResource.h>
#include <Renderers/TextureLoader.h>

#include <Logging/Logger.h>
#include <stack>


namespace OpenEngine {
namespace Display {

using namespace Resources;
using namespace Renderers;

    SceneGraphGUI::SceneModel::SceneModel(ISceneNode* node)
        : QAbstractItemModel(), root(new SceneNode()) {
        root->AddNode(node);
    }

    SceneGraphGUI::SceneModel::~SceneModel() {}
    

    Qt::DropActions SceneGraphGUI::SceneModel::supportedDropActions() const {        
        return Qt::CopyAction | Qt::MoveAction;
    }


    QStringList SceneGraphGUI::SceneModel::mimeTypes() const {
        QStringList types;
        types << "application/oe";
        return types;
    }

    QMimeData* SceneGraphGUI::SceneModel::mimeData(const QModelIndexList &indexes) const {

        QMimeData *mimeData = new QMimeData();
        QByteArray encodedData;
        
        QDataStream stream(&encodedData, QIODevice::WriteOnly);
        
        foreach (QModelIndex index, indexes) {
            if (index.isValid()) {
                
                //QString text = data(index, Qt::DisplayRole).toString();                
                QPersistentModelIndex* pi = new QPersistentModelIndex(index);
                int a = (int)pi;
                logger.info << "pointer " << a << logger.end;                
                stream << a;
            }
        }
        
        mimeData->setData("application/oe", encodedData);
        return mimeData;
    }

    bool SceneGraphGUI::SceneModel::dropMimeData(const QMimeData *data,Qt::DropAction action, int row, int column, const QModelIndex &parent) {
         QByteArray encodedData = data->data("application/oe");
         QDataStream stream(&encodedData, QIODevice::ReadOnly);

         ISceneNode* parentNode = (ISceneNode*)parent.internalPointer();
         
         logger.info << "parent: " << parentNode->ToString() << logger.end;

         while(!stream.atEnd()) {
             int p;
             //QString q;
             stream >> p;
             
             //ISceneNode* node;
             //node = (ISceneNode*)p;
             QPersistentModelIndex *pi = (QPersistentModelIndex*)p;
             ISceneNode* node = (ISceneNode*)(pi->internalPointer());
             
             
             // check cyclic!
             
             //int idx = node->GetParent()->IndexOfNode(node);
             beginRemoveRows(pi->parent(), pi->row(), pi->row());
             
             node->GetParent()->RemoveNode(node);
             endRemoveRows();

             int nidx = parentNode->GetNumberOfNodes();
             logger.info << nidx << logger.end;
             parentNode->AddNode(node);
             beginInsertRows(parent, nidx, nidx);
             
             endInsertRows();
             
             delete pi;

             //logger.info << "removed " << idx  << logger.end;
         }         
         // logger.info << p << logger.end;         
         // logger.info << "drop" << logger.end;
         return true;
     }

    int SceneGraphGUI::SceneModel::rowCount(const QModelIndex& parent) const {

        ISceneNode* parentItem;
        if (!parent.isValid()) 
            parentItem = root;
        else
            parentItem = static_cast<ISceneNode*>(parent.internalPointer());
        //logger.info << parentItem->children.size() << logger.end;
        return parentItem->GetNumberOfNodes();
    }

    int SceneGraphGUI::SceneModel::columnCount(const QModelIndex& parent) const {
        return 1;
    }

    QVariant SceneGraphGUI::SceneModel::data(const QModelIndex& index, int role) const {
        if (role != Qt::DisplayRole)
            return QVariant();

        ISceneNode* item = static_cast<ISceneNode*>(index.internalPointer());
        //std::string* s = new string(item->node->GetClassName());
        
        //return QVariant(QString(s->c_str()));
        return QString(item->GetClassName().c_str());
    }

    QModelIndex SceneGraphGUI::SceneModel::index(int row, 
                                                 int col, 
                                                 const QModelIndex& parent) const {
        if (!hasIndex(row,col, parent))
            return QModelIndex();

        ISceneNode* parentItem;
        if (!parent.isValid())
            parentItem = root;
        else
            parentItem = static_cast<ISceneNode*>(parent.internalPointer());
        
        ISceneNode* child = parentItem->GetNode(row);
        if (child)
            return createIndex(row,col,child);
        else
            return QModelIndex();
    }

    QModelIndex SceneGraphGUI::SceneModel::parent(const QModelIndex& index) const {        
        if (!index.isValid())
            return QModelIndex();

        ISceneNode *childItem = static_cast<ISceneNode*>(index.internalPointer());
        ISceneNode *parentItem = childItem->GetParent();
        
        if (parentItem == root)
            return QModelIndex();
        
        // int row = 0;
        ISceneNode *pparent = parentItem->GetParent();
        // for(unsigned int i=0;i<pparent->GetNumberOfNodes();i++) {
        //     if (pparent->GetNode(i) == parentItem) {
        //         row = i;
        //         break;
        //     }
        // }
        int row = pparent->IndexOfNode(parentItem);

        //logger.info << row << logger.end;

        return createIndex(row, 0, parentItem);

    }

    Qt::ItemFlags SceneGraphGUI::SceneModel::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return 0;

        return Qt::ItemIsEnabled 
            | Qt::ItemIsSelectable
            | Qt::ItemIsDragEnabled 
            | Qt::ItemIsDropEnabled ;
    }

    QVariant SceneGraphGUI::SceneModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            return QString("Node");
        }
        

        return QVariant();
    }
    QModelIndex SceneGraphGUI::SceneModel::FindNode(ISceneNode *node) {
        // find indexes in all parents
        ISceneNode *parent;
        ISceneNode *n = node;
        stack<ISceneNode*> parents;

        parents.push(node);
        while ((parent = n->GetParent())) {
            parents.push(parent);            
            n = parent;
        }

        QModelIndex idx;
        parent = parents.top();
        parents.pop();
        while (!parents.empty()) {            
            n = parents.top();
            parents.pop();
            int i = parent->IndexOfNode(n);
            logger.info << i << logger.end;
            idx = index(i, 0, idx);
            parent = n;
        }
        logger.info << node << logger.end;
        logger.info << idx.internalPointer() << logger.end;


        return idx;
    }

    SceneGraphGUI::SceneGraphGUI(ISceneNode* n, TextureLoader* tl, SelectionSet<ISceneNode>& ss)
        : root(n), textureLoader(tl), selectionList(ss) {
        Ui::SceneGraphGUI* ui = new Ui::SceneGraphGUI();
        ui->setupUi(this);
        tv = ui->treeView;

        // Generate creation entry for each scene node
        QMenu* newMenu = new QMenu(ui->btnNew);
        ui->btnNew->setMenu(newMenu);
#define SCENE_NODE(type)                        \
        newMenu->addAction(QString(#type));
#include <Scene/SceneNodes.def>
#undef SCENE_NODE
    }

    SceneGraphGUI::~SceneGraphGUI() {

    }
        
    void SceneGraphGUI::Handle(InitializeEventArg arg) {
        model = new SceneModel(root);
        tv->setModel(model);
        QObject::connect(tv->selectionModel(), 
                         SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                         this, 
                         SLOT(select(QItemSelection,QItemSelection)));
    }
    
    void SceneGraphGUI::Handle(SelectionSet<ISceneNode>::ChangedEventArg arg) {
        if (arg.selection.size() == 1) {
            QModelIndex idx = model->FindNode(*(arg.selection.begin()));
            tv->selectionModel()->select(idx, QItemSelectionModel::Select);
        }
    }

    void SceneGraphGUI::select(const QItemSelection & selected, 
                               const QItemSelection & deselected) {
        QModelIndexList indexes = selected.indexes();

        logger.info << "selected " << indexes.count()  << logger.end;
        if (indexes.count() == 1) {
            QModelIndex index = indexes.first();
            ISceneNode *node = (ISceneNode*)index.internalPointer();
            logger.info << node->ToString() << logger.end;
            
            model->FindNode(node);

            //NodeSelectionEventArg arg;
            //arg.node = node;
            //selectionEvent.Notify(arg);
            selectionList.Clear();
            
            selectionList.Select(node);
        }
        
    }


    IEvent<SelectionSet<ISceneNode>::ChangedEventArg >& SceneGraphGUI::SelectionEvent() {
        return selectionList.ChangedEvent();
    }


    void SceneGraphGUI::CreateSceneNode(QAction* act) {
        // Creation only works if a single node is selected
        if (selectionList.GetSelection().size() != 1) {
            logger.info << "Could not create a new scene node without a parent."
                        << logger.end;
            return;
        }
        ISceneNode* selected = *selectionList.GetSelection().begin();
        // Now we test the action-text against the name of all known
        // nodes.  This is a hack, but I cannot find a better
        // solution as Qt slots can not be defined by a macro :(
        // I also use a hack to force the index to update its display.
        // Please feel free to do that in a nicer fashion.
        // Also, we should change the currently selected node to the
        // newly created node.
        string name(act->text().toAscii().data());
#define SCENE_NODE(type)                       \
        if (name == #type) {                   \
            try {                              \
                QModelIndex i;                 \
                ISceneNode* node = new type(); \
                selected->AddNode(node);       \
                i = model->FindNode(selected); \
                tv->setExpanded(i, false);     \
                tv->setExpanded(i, true);      \
            } catch (Core::Exception e) {      \
                logger.warning << e.what()     \
                               << logger.end;  \
            }                                  \
        }
#include <Scene/SceneNodes.def>
#undef SCENE_NODE
    }

    void SceneGraphGUI::DeleteSceneNode() {
        // Hmmm, this is not so easy to implement as we currently have
        // references to scene nodes that may live past our deletion
        // of the node. This means that the selection list and all
        // other structures that have pointers to scene nodes will
        // need to do so via weak pointers or something similar and
        // they must handle "deleted" nodes gracefully.
        logger.info << "Deleting a scene node is currently not implemented"
                    << logger.end;
    }

    void SceneGraphGUI::LoadSceneNode() {
        // Loading only works if a single node is selected
        if (selectionList.GetSelection().size() != 1) {
            logger.info << "Could not load a new scene without a parent."
                        << logger.end;
            return;
        }
        ISceneNode* selected = *selectionList.GetSelection().begin();

        QString fileTmp = QFileDialog::getOpenFileName(this, tr("Load Scene"));
        string file(fileTmp.toAscii().data());
        logger.info << "Loading a scene from file: " << file << logger.end;

        try {
            IModelResourcePtr res = ResourceManager<IModelResource>::Create(file);
            res->Load();
            ISceneNode* node = res->GetSceneNode();
            if (node == NULL) {
                logger.info << "Model did not load a scene" << logger.end;
            } else {
                try {
                    if (textureLoader) textureLoader->Load(*node);
                } catch (Exception e) {
                    logger.warning << e.what() << logger.end;
                }
                selected->AddNode(node);
                QModelIndex i = model->FindNode(selected);    
                tv->setExpanded(i, false);
                tv->setExpanded(i, true);
                logger.info << "Successfully loaded the model" << logger.end;
            }
            res->Unload();
        } catch (Exception e) {
            logger.warning << e.what() << logger.end;
        }
    }

    void SceneGraphGUI::SaveSceneNode() {
        logger.info << "Saving a scene node is currently not implemented"
                    << logger.end;
    }

}
}
