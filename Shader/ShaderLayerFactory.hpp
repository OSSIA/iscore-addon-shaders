#pragma once
#include <Process/GenericProcessFactory.hpp>
#include <Shader/ShaderPresenter.hpp>
#include <Shader/ShaderView.hpp>

#include <Process/LayerModelPanelProxy.hpp>

namespace Shader
{
    using LayerFactory = Process::LayerFactory_T<
    Shader::ProcessModel,
    Shader::ShaderPresenter,
    Shader::ShaderView,
    Process::GraphicsViewLayerPanelProxy>;
}
