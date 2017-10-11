#include "score_addon_shader.hpp"
#include <score_addon_shader_commands_files.hpp>

#include <Shader/ShaderFactory.hpp>
#include <Shader/ShaderExecutor.hpp>
#include <Shader/ShaderInspector.hpp>
#include <Shader/ShaderLocalTree.hpp>
#include <Shader/ShaderLayerFactory.hpp>

#include <score/plugins/customfactory/FactorySetup.hpp>

score_addon_shader::score_addon_shader()
{

}

score_addon_shader::~score_addon_shader()
{

}

std::vector<std::unique_ptr<score::InterfaceBase> >
score_addon_shader::factories(
        const score::ApplicationContext& ctx,
        const score::InterfaceKey& key) const
{
    return instantiate_factories<
            score::ApplicationContext,
        FW<Process::ProcessModelFactory,
           Shader::ProcessFactory>,
        FW<Process::LayerFactory,
        Shader::LayerFactory>,
        FW<Process::InspectorWidgetDelegateFactory,
           Shader::InspectorFactory>,
        FW<Engine::Execution::ProcessComponentFactory,
           Shader::ProcessExecutorComponentFactory>,
        FW<Engine::LocalTree::ProcessComponentFactory,
           Shader::LocalTreeProcessComponentFactory>
    >(ctx, key);
}
std::pair<const CommandGroupKey, CommandGeneratorMap>
score_addon_shader::make_commands()
{
    using namespace Shader;
    std::pair<const CommandGroupKey, CommandGeneratorMap> cmds{
        CommandFactoryName(),
        CommandGeneratorMap{}};

    using Types = TypeList<
#include <score_addon_shader_commands.hpp>
      >;

    for_each_type<Types>(score::commands::FactoryInserter{cmds.second});

    return cmds;
}
