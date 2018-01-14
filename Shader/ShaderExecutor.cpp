#include "ShaderExecutor.hpp"
#include <Shader/ShaderModel.hpp>

#include <Device/Protocol/DeviceList.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <Engine/score2OSSIA.hpp>
#include <ossia/editor/state/state_element.hpp>
#include <ossia/editor/scenario/time_interval.hpp>
#include <QOpenGLWindow>
#include <ossia/dataflow/graph/graph.hpp>
namespace Shader
{
class ProcessExecutor final :
        public ossia::graph_node
{
    public:
        ProcessExecutor(
            GLWindow* w,
            const Process::Inlets& p,
            const Device::DeviceList&);


        void run(ossia::token_request, ossia::execution_state&) override;

    private:
        GLWindow* m_window{};
        std::vector<std::string> m_id;
};


ProcessExecutor::ProcessExecutor(
    GLWindow* w,
    const Process::Inlets& p,
    const Device::DeviceList& devices):
  m_window{w}
{
  using namespace ossia;
  for(const Process::Port* port : p)
  {
    inlet_ptr inlet;
    if(port->type == Process::PortType::Message)
      inlet = ossia::make_inlet<value_port>();
    else
      inlet = ossia::make_inlet<audio_port>();

    if(auto addr = Engine::score_to_ossia::makeDestination(devices, port->address()))
    {
      inlet->address = &addr->address();
    }

    m_id.push_back(port->customData().toStdString());
    inputs().push_back(inlet);
  }
}

void ProcessExecutor::run(ossia::token_request t, ossia::execution_state&)
{
  m_window->sig_setValue("TIME", (float)t.position);

  for(std::size_t i = 0; i < m_inlets.size(); i++)
  {
    const auto& inlet = inputs()[i];
    if(auto val = inlet->data.target<ossia::value_port>())
    {
      auto& dat = val->get_data();
      if(!dat.empty())
        m_window->sig_setValue(m_id[i], dat.back().value);
    }
    else if(auto val = inlet->data.target<ossia::audio_port>())
    {
      m_window->sig_setAudio(m_id[i], val->samples);
    }
  }
}



ProcessExecutorComponent::ProcessExecutorComponent(
    Shader::ProcessModel& element,
    const Engine::Execution::Context& ctx,
    const Id<score::Component>& id,
    QObject* parent):
  ProcessComponent_T{element, ctx, id, "ShaderExecutorComponent", parent}
{
  node = std::make_shared<ProcessExecutor>(element.window(), element.inlets(), ctx.devices.list());
  auto proc = std::make_shared<ossia::node_process>(node);
  m_ossia_process = proc;
}

}
