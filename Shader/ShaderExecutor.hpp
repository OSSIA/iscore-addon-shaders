#pragma once
#include <Process/Dataflow/Port.hpp>
#include <Process/Execution/ProcessComponent.hpp>

#include <ossia/dataflow/graph_node.hpp>
#include <ossia/dataflow/node_process.hpp>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWindow>
#include <QVector3D>
class QOpenGLWindow;
namespace Device
{
class DeviceList;
}
namespace Shader
{
class ProcessModel;
class GLWindow;
class ProcessExecutorComponent final
    : public Execution::
          ProcessComponent_T<Shader::ProcessModel, ossia::node_process>
{
  COMPONENT_METADATA("7c959b5d-6b32-4ac4-bbfd-b3ebc939bfcb")
public:
  ProcessExecutorComponent(
      ProcessModel& element,
      const Execution::Context& ctx,
      const Id<score::Component>& id,
      QObject* parent);
};

using ProcessExecutorComponentFactory
    = Execution::ProcessComponentFactory_T<ProcessExecutorComponent>;
}
