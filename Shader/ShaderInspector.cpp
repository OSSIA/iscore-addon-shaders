#include "ShaderInspector.hpp"
#include <Shader/EditShader.hpp>

#include <iscore/document/DocumentContext.hpp>
#include <iscore/widgets/MarginLess.hpp>
#include <ossia/detail/logger.hpp>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace Shader
{

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
}
}
