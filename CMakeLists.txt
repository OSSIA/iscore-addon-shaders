cmake_minimum_required(VERSION 3.1)
project(iscore_addon_shader LANGUAGES CXX)

# General initialization
iscore_common_setup()

# Source files
set(HDRS
    Shader/EditShader.hpp
    Shader/ShaderCommandFactory.hpp
    Shader/ShaderExecutor.hpp
    Shader/ShaderFactory.hpp
    Shader/ShaderInspector.hpp
    Shader/ShaderLayerFactory.hpp
    Shader/ShaderLayer.hpp
    Shader/ShaderLocalTree.hpp
    Shader/ShaderMetadata.hpp
    Shader/ShaderModel.hpp
    Shader/ShaderPresenter.hpp
    Shader/ShaderView.hpp

    iscore_addon_shader.hpp
)

set(SRCS
    Shader/EditShader.cpp
    Shader/ShaderExecutor.cpp
    Shader/ShaderInspector.cpp
    Shader/ShaderLocalTree.cpp
    Shader/ShaderModel.cpp
    Shader/ShaderPresenter.cpp
    Shader/ShaderView.cpp

    iscore_addon_shader.cpp
)


# Creation of the library
add_library(${PROJECT_NAME} ${SRCS} ${HDRS})

# Code generation
iscore_generate_command_list_file(${PROJECT_NAME} "${HDRS}")

# Link
target_link_libraries(${PROJECT_NAME} PUBLIC iscore_plugin_engine)

# Target-specific options
setup_iscore_plugin(${PROJECT_NAME})
