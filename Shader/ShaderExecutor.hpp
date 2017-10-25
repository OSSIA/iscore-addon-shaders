#pragma once
#include <Engine/Executor/ProcessComponent.hpp>
#include <Process/Dataflow/DataflowObjects.hpp>
#include <QOpenGLShader>
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <ossia/dataflow/graph_node.hpp>
#include <ossia/dataflow/node_process.hpp>
class QOpenGLWindow;
namespace Device
{
class DeviceList;
}
namespace Shader
{
class ProcessModel;
class GLWindow;
class ProcessExecutor final :
        public ossia::graph_node
{
    public:
        ProcessExecutor(
            GLWindow* w,
            const std::vector<Process::Port*>& p,
            const Device::DeviceList&);


        void run(ossia::token_request, ossia::execution_state&) override;

    private:
        GLWindow* m_window{};
        std::vector<std::string> m_id;
};

class ProcessExecutorComponent final :
        public Engine::Execution::ProcessComponent_T<
            Shader::ProcessModel, ossia::node_process>
{
        COMPONENT_METADATA("7c959b5d-6b32-4ac4-bbfd-b3ebc939bfcb")
    public:
        ProcessExecutorComponent(
                Engine::Execution::IntervalComponent& parentInterval,
                ProcessModel& element,
                const Engine::Execution::Context& ctx,
                const Id<score::Component>& id,
                QObject* parent);
};


using ProcessExecutorComponentFactory =
    Engine::Execution::ProcessComponentFactory_T<ProcessExecutorComponent>;

}
