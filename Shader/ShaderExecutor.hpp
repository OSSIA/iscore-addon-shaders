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
        public ossia::time_process
{
    public:
        ProcessExecutor(
            GLWindow* w,
            const std::vector<Process::Port>& p,
            const Device::DeviceList&);

        void start() override;
        void stop() override;
        void pause() override;
        void resume() override;

        ossia::state_element state(ossia::time_value date, double pos) override;

    private:
        const Device::DeviceList& m_devices;
        GLWindow* m_window{};
        std::vector<std::pair<ossia::net::address_base*, std::string>> m_ports;
        std::vector<ossia::net::address_base::iterator> m_it;
};

class ProcessExecutorComponent final :
        public Engine::Execution::ProcessComponent_T<
            Shader::ProcessModel,
            Shader::ProcessExecutor>
{
        COMPONENT_METADATA("7c959b5d-6b32-4ac4-bbfd-b3ebc939bfcb")
    public:
        ProcessExecutorComponent(
                Engine::Execution::ConstraintComponent& parentConstraint,
                ProcessModel& element,
                const Engine::Execution::Context& ctx,
                const Id<iscore::Component>& id,
                QObject* parent);
};


using ProcessExecutorComponentFactory =
    Engine::Execution::ProcessComponentFactory_T<ProcessExecutorComponent>;

}
