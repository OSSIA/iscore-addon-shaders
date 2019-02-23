#pragma once
#include <Process/Inspector/ProcessInspectorWidgetDelegate.hpp>
#include <Process/Inspector/ProcessInspectorWidgetDelegateFactory.hpp>

#include <score/command/Dispatchers/CommandDispatcher.hpp>

#include <QPlainTextEdit>

#include <Shader/ShaderModel.hpp>
namespace Shader
{
class ProcessModel;
class InspectorWidget final
    : public Process::InspectorWidgetDelegate_T<Shader::ProcessModel>,
      public Nano::Observer
{
public:
  explicit InspectorWidget(
      const Shader::ProcessModel& object,
      const score::DocumentContext& context,
      QWidget* parent);

private:
  CommandDispatcher<> m_dispatcher;
  QPlainTextEdit m_edit;
};

class InspectorFactory final
    : public Process::
          InspectorWidgetDelegateFactory_T<ProcessModel, InspectorWidget>
{
  SCORE_CONCRETE("7a4f4191-fce1-485e-9479-e6b1b1804dad")
};
}
