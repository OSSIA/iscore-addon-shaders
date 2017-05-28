#include "ShaderInspector.hpp"
#include <Shader/EditShader.hpp>

#include <iscore/document/DocumentContext.hpp>
#include <iscore/widgets/MarginLess.hpp>
#include <iscore/widgets/ClearLayout.hpp>
#include <State/Expression.hpp>
#include <ossia/detail/logger.hpp>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QAbstractTableModel>

namespace Shader
{
class PortModel: public QAbstractTableModel
{
    const Process::DataflowProcess& m_proc;
    std::vector<Process::Port> m_data;
    CommandDispatcher<> m_disp;
  public:
    PortModel(const Process::DataflowProcess& p,
              const iscore::CommandStackFacade& f):
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
    int rowCount(const QModelIndex& parent) const
    {
      return m_data.size();
    }

    int columnCount(const QModelIndex& parent) const
    {
      return 2;
    }

    QVariant data(const QModelIndex& index, int role) const
    {
      auto i = index.row();
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
    {
      auto i = index.row();
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
                m_disp.submitCommand<SetInletData>(m_proc, value.toString(), i);
                return true;
              case 1:
                if(auto addr = State::parseAddressAccessor(value.toString()))
                {
                  m_disp.submitCommand<SetInletAddress>(m_proc, *addr, i);
                  return true;
                }
                break;
            }
            break;
        }
      }
      return false;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
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
    QStringList mimeTypes() const
    {
      return {};
    }

    QMimeData* mimeData(const QModelIndexList& indexes) const
    {
      return nullptr;
    }

    bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
    {
      return false;
    }

    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
    {
      return false;
    }

    Qt::DropActions supportedDropActions() const
    {
      return {};
    }
    Qt::DropActions supportedDragActions() const
    {
      return {};
    }

    Qt::ItemFlags flags(const QModelIndex& index) const
    {
      return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }
};

class DataflowWidget : public QWidget
{
    const Process::DataflowProcess& m_proc;
    QVBoxLayout m_lay;
    PortModel m_model;
    QTableView m_view;

    QPushButton m_button;
    CommandDispatcher<> m_disp;

  public:
    DataflowWidget(const Process::DataflowProcess& proc, const iscore::DocumentContext& c):
      m_proc{proc}
    , m_lay{this}
    , m_model{proc, c.commandStack}
    , m_button{tr("Add")}
    , m_disp{c.commandStack}
    {
      m_view.setModel(&m_model);
      m_view.setAlternatingRowColors(true);
      m_lay.addWidget(&m_view);
      m_lay.addWidget(&m_button);

      con(m_button, &QPushButton::clicked,
          this, [&] {
        m_disp.submitCommand<AddInlet>(m_proc);
      });

      con(proc, &Process::DataflowProcess::inletsChanged,
          this, &DataflowWidget::recreate);
      recreate();
    }

    void recreate()
    {
      m_model.reload();
    }

};

InspectorWidget::InspectorWidget(
    const Shader::ProcessModel& object,
    const iscore::DocumentContext& context,
    QWidget* parent):
  InspectorWidgetDelegate_T {object, parent},
  m_dispatcher{context.commandStack}
{
  m_edit.setPlainText(object.fragment());
  con(m_edit, &QPlainTextEdit::textChanged,
      this, [&] {
    m_dispatcher.submitCommand<EditShader>(object, m_edit.toPlainText());
  });

  con(object, &Shader::ProcessModel::fragmentChanged,
      this, [&] (const auto& txt) {
    if(txt != m_edit.toPlainText())
      m_edit.setPlainText(txt);
  });

  auto lay = new iscore::MarginLess<QVBoxLayout>{this};
  lay->addWidget(&m_edit);
  lay->addWidget(new DataflowWidget{object, context});
}
}
