#include "iscore_addon_shader.hpp"
#include <iscore_addon_shader_commands_files.hpp>

#include <Shader/ShaderFactory.hpp>
#include <Shader/ShaderExecutor.hpp>
#include <Shader/ShaderInspector.hpp>
#include <Shader/ShaderLocalTree.hpp>
#include <Shader/ShaderLayerFactory.hpp>

#include <iscore/plugins/customfactory/FactorySetup.hpp>

iscore_addon_shader::iscore_addon_shader()
{

}

iscore_addon_shader::~iscore_addon_shader()
{

}

std::vector<std::unique_ptr<iscore::InterfaceBase> >
iscore_addon_shader::factories(
        const iscore::ApplicationContext& ctx,
        const iscore::InterfaceKey& key) const
{
    return instantiate_factories<
            iscore::ApplicationContext,
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
iscore_addon_shader::make_commands()
{
    using namespace Shader;
    std::pair<const CommandGroupKey, CommandGeneratorMap> cmds{
        CommandFactoryName(),
        CommandGeneratorMap{}};

    using Types = TypeList<
#include <iscore_addon_shader_commands.hpp>
      >;

    for_each_type<Types>(iscore::commands::FactoryInserter{cmds.second});

    return cmds;
}
