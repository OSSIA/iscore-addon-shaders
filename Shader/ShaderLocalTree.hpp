#pragma once
#include <Engine/LocalTree/Scenario/ProcessComponent.hpp>
#include <Engine/LocalTree/LocalTreeComponent.hpp>

namespace Shader
{
class ProcessModel;

class EffectComponent;
class EffectProcessComponent;
class EffectFactoryList;

//! This component displays informations about an object in the Device Explorer,
//! in the left panel.
class LocalTreeProcessComponent :
        public Engine::LocalTree::ProcessComponent_T<ProcessModel>
{
        COMPONENT_METADATA("3aa84ab6-5fa8-4721-a14d-ff6cefbd37ab")

    public:
       LocalTreeProcessComponent(
               const Id<score::Component>& id,
               ossia::net::node_base& parent,
               Shader::ProcessModel& scenario,
               Engine::LocalTree::DocumentPlugin& doc,
               QObject* parent_obj);

    ~LocalTreeProcessComponent();

    private:
        std::unique_ptr<Engine::LocalTree::BaseProperty> m_bananaProperty;
};

using LocalTreeProcessComponentFactory = Engine::LocalTree::ProcessComponentFactory_T<LocalTreeProcessComponent>;
}
