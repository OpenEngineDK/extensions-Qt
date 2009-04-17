
#include "SceneGraphGUI.h"

#include "Qt/ui_SceneGraphUI.h"

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Display {


    SceneGraphGUI::SceneModel::SceneModel(ISceneNode* node)
        : QAbstractItemModel(), root(new SceneNode()) {
        root->AddNode(node);
    }

    SceneGraphGUI::SceneModel::~SceneModel() {}
    

    Qt::DropActions SceneGraphGUI::SceneModel::supportedDropActions() const
    {
        
        return Qt::CopyAction | Qt::MoveAction;
    }


    QStringList SceneGraphGUI::SceneModel::mimeTypes() const
    {
        QStringList types;
        types << "application/oe";
        return types;
    }

    QMimeData* SceneGraphGUI::SceneModel::mimeData(const QModelIndexList &indexes) const
    {

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

    bool SceneGraphGUI::SceneModel::dropMimeData(const QMimeData *data,Qt::DropAction action, int row, int column, const QModelIndex &parent)
     {
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

    QModelIndex SceneGraphGUI::SceneModel::index(int row, int col, const QModelIndex& parent) const {
        if (!hasIndex(row,col, parent))
            return QModelIndex();

        ISceneNode* parentItem;
        if (!parent.isValid())
            parentItem = root;
        else
            parentItem = static_cast<ISceneNode*>(parent.internalPointer());
        
        //logger.info << parentItem << " " << row << logger.end;
        //1logger.info << parentItem->children[row] << logger.end;

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


    SceneGraphGUI::SceneGraphGUI(ISceneNode* n) : root(n) {

        Ui::SceneGraphGUI* ui = new Ui::SceneGraphGUI();
        ui->setupUi(this);

        tv = ui->treeView;
        // QVBoxLayout* lay = new QVBoxLayout();
        
        // lay->addWidget(tv);

        // setLayout(lay);
    }

    void SceneGraphGUI::Handle(InitializeEventArg arg) {
        model = new SceneModel(root);
        tv->setModel(model);
    }
    

    SceneGraphGUI::~SceneGraphGUI() {}

}
}

// #include "SceneGraphGUI.h"

// #if SG_USE_NEW
// #include "SceneGraphGUI-new.cpp"
// #else
// #include "SceneGraphGUI-old.cpp"
// #endif

