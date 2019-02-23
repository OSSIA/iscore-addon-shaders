#include "score_addon_shader.hpp"

#include <score/plugins/FactorySetup.hpp>

#include <Shader/ShaderExecutor.hpp>
#include <Shader/ShaderFactory.hpp>
#include <Shader/ShaderInspector.hpp>
#include <Shader/ShaderLayerFactory.hpp>
#include <Shader/ShaderLocalTree.hpp>
#include <score_addon_shader_commands_files.hpp>

score_addon_shader::score_addon_shader() {}

score_addon_shader::~score_addon_shader() {}

std::vector<std::unique_ptr<score::InterfaceBase>>
score_addon_shader::factories(
    const score::ApplicationContext& ctx,
    const score::InterfaceKey& key) const
{
  return instantiate_factories<
      score::ApplicationContext,
      FW<Process::ProcessModelFactory, Shader::ProcessFactory>,
      FW<Process::LayerFactory, Shader::LayerFactory>,
      FW<Process::InspectorWidgetDelegateFactory, Shader::InspectorFactory>,
      FW<Execution::ProcessComponentFactory,
         Shader::ProcessExecutorComponentFactory>,
      FW<LocalTree::ProcessComponentFactory,
         Shader::LocalTreeProcessComponentFactory>>(ctx, key);
}
std::pair<const CommandGroupKey, CommandGeneratorMap>
score_addon_shader::make_commands()
{
  using namespace Shader;
  std::pair<const CommandGroupKey, CommandGeneratorMap> cmds{
      CommandFactoryName(), CommandGeneratorMap{}};

  ossia::for_each_type<
#include <score_addon_shader_commands.hpp>
      >(score::commands::FactoryInserter{cmds.second});
  return cmds;
}

#include <score/plugins/PluginInstances.hpp>
SCORE_EXPORT_PLUGIN(score_addon_shader)
