#include "ShaderExecutor.hpp"
#include <Shader/ShaderModel.hpp>

#include <Device/Protocol/DeviceList.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <Engine/score2OSSIA.hpp>
#include <ossia/editor/state/state_element.hpp>
#include <ossia/editor/scenario/time_interval.hpp>
#include <QOpenGLWindow>
namespace Shader
{

ProcessExecutor::ProcessExecutor(
    GLWindow* w,
    const std::vector<Process::Port*>& p,
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
      if(!val->data.empty())
        m_window->sig_setValue(m_id[i], val->data.front());
    }
    else if(auto val = inlet->data.target<ossia::audio_port>())
    {
      m_window->sig_setAudio(m_id[i], val->samples);
    }
  }
}



ProcessExecutorComponent::ProcessExecutorComponent(
    Engine::Execution::IntervalComponent& parentInterval,
    Shader::ProcessModel& element,
    const Engine::Execution::Context& ctx,
    const Id<score::Component>& id,
    QObject* parent):
  ProcessComponent_T{
    parentInterval, element, ctx, id, "ShaderExecutorComponent", parent}
{
  auto node = std::make_shared<ProcessExecutor>(element.window(), element.inlets(), ctx.devices.list());
  auto proc = std::make_shared<ossia::node_process>(node);
  ctx.plugin.execGraph->add_node(node);

  int i = 0;
  for(auto p : element.inlets())
    ctx.plugin.inlets.insert({p, {node, node->inputs()[i++]}});

  m_ossia_process = proc;
}

}
