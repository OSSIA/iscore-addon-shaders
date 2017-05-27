#pragma once
#include <Shader/ShaderModel.hpp>
#include <Process/Inspector/ProcessInspectorWidgetDelegate.hpp>
#include <Process/Inspector/ProcessInspectorWidgetDelegateFactory.hpp>
#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
#include <QPlainTextEdit>
namespace Shader
{
class ProcessModel;
class InspectorWidget final :
        public Process::InspectorWidgetDelegate_T<Shader::ProcessModel>,
        public Nano::Observer
{
    public:
        explicit InspectorWidget(
                const Shader::ProcessModel& object,
                const iscore::DocumentContext& context,
                QWidget* parent);

    private:
        CommandDispatcher<> m_dispatcher;
        QPlainTextEdit m_edit;
};


class InspectorFactory final :
        public Process::InspectorWidgetDelegateFactory_T<ProcessModel, InspectorWidget>
{
        ISCORE_CONCRETE("7a4f4191-fce1-485e-9479-e6b1b1804dad")
};
}
