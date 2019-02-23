#pragma once
#include <Process/ProcessMetadata.hpp>

namespace Shader
{
class ProcessModel;
}

PROCESS_METADATA(
    ,
    Shader::ProcessModel,
    "b5603b73-2ad2-47ea-8ead-05432848b648",
    "Shader",
    "Shader",
    Process::ProcessCategory::Other,
    "Graphics",
    "Shader",
    "ossia team",
    {},
    {},
    {},
    Process::ProcessFlags::SupportsTemporal
        | Process::ProcessFlags::PutInNewSlot)
