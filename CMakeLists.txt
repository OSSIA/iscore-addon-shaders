cmake_minimum_required(VERSION 3.1)
project(score_addon_shader LANGUAGES CXX)

# General initialization
score_common_setup()

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

    3rdparty/libisf/src/isf.hpp

    score_addon_shader.hpp
)

set(SRCS
    Shader/EditShader.cpp
    Shader/ShaderExecutor.cpp
    Shader/ShaderInspector.cpp
    Shader/ShaderLocalTree.cpp
    Shader/ShaderModel.cpp
    Shader/ShaderPresenter.cpp
    Shader/ShaderView.cpp

    3rdparty/libisf/src/isf.cpp

    score_addon_shader.cpp
)


# Creation of the library
add_library(${PROJECT_NAME} ${SRCS} ${HDRS})

# Code generation
score_generate_command_list_file(${PROJECT_NAME} "${HDRS}")

# Link
target_link_libraries(${PROJECT_NAME} PUBLIC score_plugin_engine)
target_include_directories(${PROJECT_NAME} PRIVATE 3rdparty/libisf/src/)

# Target-specific options
setup_score_plugin(${PROJECT_NAME})
