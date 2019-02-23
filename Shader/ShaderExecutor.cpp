#include "ShaderExecutor.hpp"

#include <Explorer/DeviceList.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>

#include <ossia/dataflow/port.hpp>
#include <ossia/editor/state/state_element.hpp>

#include <QOpenGLWindow>

#include <Shader/ShaderModel.hpp>
namespace Shader
{
class shader_node final : public ossia::graph_node
{
public:
  shader_node(GLWindow* w, const Process::Inlets& p);

  void run(ossia::token_request, ossia::exec_state_facade) noexcept override;

private:
  GLWindow* m_window{};
  std::vector<std::string> m_id;
};

shader_node::shader_node(GLWindow* w, const Process::Inlets& p) : m_window{w}
{
  using namespace ossia;
  for (const Process::Port* port : p)
  {
    inlet_ptr inlet;
    if (port->type == Process::PortType::Message)
      inlet = ossia::make_inlet<value_port>();
    else
      inlet = ossia::make_inlet<audio_port>();

    m_id.push_back(port->customData().toStdString());
    inputs().push_back(inlet);
  }
}

void shader_node::run(
    ossia::token_request t,
    ossia::exec_state_facade) noexcept
{
  m_window->sig_setValue("TIME", (float)t.position);

  for (std::size_t i = 0; i < m_inlets.size(); i++)
  {
    const auto& inlet = inputs()[i];
    if (auto val = inlet->data.target<ossia::value_port>())
    {
      auto& dat = val->get_data();
      if (!dat.empty())
        m_window->sig_setValue(m_id[i], dat.back().value);
    }
    else if (auto val = inlet->data.target<ossia::audio_port>())
    {
      m_window->sig_setAudio(m_id[i], val->samples);
    }
  }
}

ProcessExecutorComponent::ProcessExecutorComponent(
    Shader::ProcessModel& element,
    const Execution::Context& ctx,
    const Id<score::Component>& id,
    QObject* parent)
    : ProcessComponent_T{element, ctx, id, "ShaderExecutorComponent", parent}
{
  node = std::make_shared<shader_node>(element.window(), element.inlets());
  auto proc = std::make_shared<ossia::node_process>(node);
  m_ossia_process = proc;
}
}
