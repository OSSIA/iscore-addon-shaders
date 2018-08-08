#include "ShaderLocalTree.hpp"
#include <Shader/ShaderModel.hpp>
#include <LocalTree/Property.hpp>

namespace Shader
{

LocalTreeProcessComponent::LocalTreeProcessComponent(
        const Id<score::Component>& id,
        ossia::net::node_base& parent,
        Shader::ProcessModel& proc,
        LocalTree::DocumentPlugin& sys,
        QObject* parent_obj):
    LocalTree::ProcessComponent_T<Shader::ProcessModel>{parent, proc, sys, id, "ShaderComponent", parent_obj}
{
}

LocalTreeProcessComponent::~LocalTreeProcessComponent()
{

}

}
