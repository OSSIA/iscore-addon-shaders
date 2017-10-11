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
  m_devices{devices}
, m_window{w}
{
  for(const Process::Port* port : p)
  {
    auto addr = Engine::score_to_ossia::address(port->address().address, devices);
    if(addr)
    {
      m_ports.push_back({addr, port->customData().toStdString()});
    }
  }
}


void ProcessExecutor::start(ossia::state&)
{
  for(auto port : m_ports)
  {
    m_it.push_back(port.first->add_callback([] (const ossia::value&) { }));
  }
}

void ProcessExecutor::stop()
{
  for(int i = 0; i < m_ports.size(); i++)
  {
    m_ports[i].first->remove_callback(m_it[i]);
  }
  m_it.clear();
}

void ProcessExecutor::pause()
{
}

void ProcessExecutor::resume()
{
}

ossia::state_element ProcessExecutor::state(ossia::time_value date, double pos, ossia::time_value tick_offset)
{
  m_window->sig_setValue("time", (float)pos);

  for(auto port : m_ports)
  {
    m_window->sig_setValue(port.second, port.first->value());
  }
  return {};
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
  m_ossia_process = std::make_shared<ProcessExecutor>(
                      element.window(),
                      element.inlets(),
                      ctx.devices.list());
}

}
