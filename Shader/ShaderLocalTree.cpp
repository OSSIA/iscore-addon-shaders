#include "ShaderLocalTree.hpp"
#include <Shader/ShaderModel.hpp>
#include <Engine/LocalTree/Property.hpp>

namespace Shader
{

LocalTreeProcessComponent::LocalTreeProcessComponent(
        const Id<score::Component>& id,
        ossia::net::node_base& parent,
        Shader::ProcessModel& proc,
        Engine::LocalTree::DocumentPlugin& sys,
        QObject* parent_obj):
    Engine::LocalTree::ProcessComponent_T<Shader::ProcessModel>{parent, proc, sys, id, "ShaderComponent", parent_obj}
{
}

LocalTreeProcessComponent::~LocalTreeProcessComponent()
{

}

}
