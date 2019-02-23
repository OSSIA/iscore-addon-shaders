#include "ShaderInspector.hpp"

#include <State/Expression.hpp>

#include <score/document/DocumentContext.hpp>
#include <score/widgets/ClearLayout.hpp>
#include <score/widgets/MarginLess.hpp>

#include <ossia/detail/logger.hpp>

#include <QAbstractTableModel>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

#include <Shader/EditShader.hpp>

namespace Shader
{ /*
 class PortModel final: public QAbstractTableModel
 {
     const Process::ProcessModel& m_proc;
     std::vector<Process::Port> m_data;
     CommandDispatcher<> m_disp;
   public:
     PortModel(const Process::ProcessModel& p,
               const score::CommandStackFacade& f):
       m_proc{p},
       m_disp{f}
     {

     }

     void reload()
     {
       beginResetModel();
       m_data = m_proc.inlets();
       endResetModel();
     }

   private:
     int rowCount(const QModelIndex& parent) const override
     {
       return m_data.size();
     }

     int columnCount(const QModelIndex& parent) const override
     {
       return 2;
     }

     QVariant data(const QModelIndex& index, int role) const override
     {
       std::size_t i = index.row();
       auto c = index.column();

       if(index.isValid() && i < m_data.size() && c < 2)
       {
         const Process::Port& p = m_data[i];
         switch(role)
         {
           case Qt::DisplayRole:
             switch(c)
             {
               case 0:
                 return p.customData;
                 break;
               case 1:
                 return p.address.toString();
                 break;
             }
             break;
         }
       }
       return {};
     }

     bool setData(const QModelIndex& index, const QVariant& value, int role)
 override
     {
       std::size_t i = index.row();
       auto c = index.column();

       if(index.isValid() && i < m_data.size() && c < 2)
       {
         qDebug() << role << c << i;
         switch(role)
         {
           case Qt::EditRole:
             switch(c)
             {
               case 0:
                 m_disp.submit<SetInletData>(m_proc, value.toString(),
 i); return true; case 1: if(auto addr =
 State::parseAddressAccessor(value.toString()))
                 {
                   m_disp.submit<SetInletAddress>(m_proc, *addr, i);
                   return true;
                 }
                 break;
             }
             break;
         }
       }
       return false;
     }

     QVariant headerData(int section, Qt::Orientation orientation, int role)
 const override
     {
       if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
       {
         switch(section)
         {
           case 0:
             return tr("Variable");
           case 1:
             return tr("Address");
         }
       }
       return {};
     }
     QStringList mimeTypes() const override
     {
       return {};
     }

     QMimeData* mimeData(const QModelIndexList& indexes) const override
     {
       return nullptr;
     }

     bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int
 row, int column, const QModelIndex& parent) const override
     {
       return false;
     }

     bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row,
 int column, const QModelIndex& parent) override
     {
       return false;
     }

     Qt::DropActions supportedDropActions() const override
     {
       return {};
     }
     Qt::DropActions supportedDragActions() const override
     {
       return {};
     }

     Qt::ItemFlags flags(const QModelIndex& index) const override
     {
       return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
     }
 };

 class DataflowWidget : public QWidget
 {
     const Process::ProcessModel& m_proc;
     QVBoxLayout m_lay;
     PortModel m_model;
     QTableView m_view;

     QPushButton m_button;
     CommandDispatcher<> m_disp;

   public:
     DataflowWidget(const Process::ProcessModel& proc, const
 score::DocumentContext& c): m_proc{proc} , m_lay{this} , m_model{proc,
 c.commandStack} , m_button{tr("Add")} , m_disp{c.commandStack}
     {
       m_view.setModel(&m_model);
       m_view.setAlternatingRowColors(true);
       m_lay.addWidget(&m_view);
       m_lay.addWidget(&m_button);

       con(m_button, &QPushButton::clicked,
           this, [&] {
         m_disp.submit<AddInlet>(m_proc);
       });

       con(proc, &Process::ProcessModel::inletsChanged,
           this, &DataflowWidget::recreate);
       recreate();
     }

     void recreate()
     {
       m_model.reload();
     }

 };*/

InspectorWidget::InspectorWidget(
    const Shader::ProcessModel& object,
    const score::DocumentContext& context,
    QWidget* parent)
    : InspectorWidgetDelegate_T{object, parent}
    , m_dispatcher{context.commandStack}
{
  m_edit.setPlainText(object.shader());
  con(m_edit, &QPlainTextEdit::textChanged, this, [&] {
    m_dispatcher.submit<EditShader>(object, m_edit.toPlainText());
  });

  con(object,
      &Shader::ProcessModel::shaderChanged,
      this,
      [&](const auto& txt) {
        if (txt != m_edit.toPlainText())
          m_edit.setPlainText(txt);
      });

  auto lay = new score::MarginLess<QVBoxLayout>{this};
  lay->addWidget(&m_edit);
  // lay->addWidget(new DataflowWidget{object, context});
}
}
