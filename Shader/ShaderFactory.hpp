#pragma once
#include <Process/GenericProcessFactory.hpp>
#include <Shader/ShaderModel.hpp>

namespace Shader
{
using ProcessFactory =
    Process::ProcessFactory_T<
        Shader::ProcessModel>;
}
