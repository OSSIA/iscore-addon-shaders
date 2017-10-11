#include "ShaderModel.hpp"
#include <QOpenGLShaderProgram>

namespace Shader
{

ProcessModel::ProcessModel(
        const TimeVal& duration,
        const Id<Process::ProcessModel>& id,
        QObject* parent):
    Process::ProcessModel{duration, id, "Shader", parent}
{
    metadata().setInstanceName(*this);
    m_fragment =
        R"_(
        uniform float time;
        uniform float tata;
        float rand(float n){return fract(sin(n) * 43758.5453123);}

        float noise(float p){
        float fl = floor(p);
        float fc = fract(p);
        return mix(rand(fl), rand(fl + 1.0), fc);
        }

        void mainImage( out vec4 fragColor, in vec4 fragCoord )
        {
        fragColor = vec4(tata / 127.0, mod(time, 1.) * noise(fragCoord.y), noise(fragCoord.z), 1.);
        }
        )_";

    m_window = new GLWindow{*this};
    m_window->show();
}

ProcessModel::~ProcessModel()
{
  m_window->close();
  m_window->deleteLater();
}

QString ProcessModel::fragment() const
{
  return m_fragment;
}

void ProcessModel::setFragment(QString fragment)
{
  if (m_fragment == fragment)
    return;

  m_fragment = fragment;
  m_window->reload();
  emit fragmentChanged(m_fragment);
}

ProcessModel::ProcessModel(
    const ProcessModel& source,
    const Id<Process::ProcessModel>& id,
    QObject* parent):
  Process::ProcessModel{source, id, "Shader", parent}
{

}

QString ProcessModel::prettyName() const
{
    return tr("Shader Process");
}

void ProcessModel::startExecution()
{
}

void ProcessModel::stopExecution()
{
}

void ProcessModel::reset()
{
}

ProcessStateDataInterface* ProcessModel::startStateData() const
{
    return nullptr;
}

ProcessStateDataInterface* ProcessModel::endStateData() const
{
    return nullptr;
}

Selection ProcessModel::selectableChildren() const
{
    return {};
}

Selection ProcessModel::selectedChildren() const
{
    return {};
}

void ProcessModel::setSelection(const Selection& s) const
{
}

void ProcessModel::setDurationAndScale(const TimeVal& newDuration)
{
}

void ProcessModel::setDurationAndGrow(const TimeVal& newDuration)
{
}

void ProcessModel::setDurationAndShrink(const TimeVal& newDuration)
{
}

GLWindow::GLWindow(ProcessModel& m):
 model{m}
{
  connect(this, &GLWindow::sig_setValue,
          this, &GLWindow::slt_setValue,
          Qt::QueuedConnection);
}

void GLWindow::initializeGL()
{  // Initialize OpenGL Backend
  initializeOpenGLFunctions();

  // Set global information
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  shaderProgram = new QOpenGLShaderProgram();

  shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                         R"_(
                                         #version 330 core
                                         const vec2 quadVertices[4] = vec2[4]( vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) );

                                         void main()
                                         {
                                           gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
                                         }
                                         )_");

  auto frag = model.fragment() +
R"_(
 void main(void)
 {
  mainImage(gl_FragColor, gl_FragCoord);
 }
)_";

  shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, frag);
  if(shaderProgram->shaders().size() < 2)
    return;

  shader = shaderProgram->shaders()[1];

  shaderProgram->link();
  shaderProgram->bind();

  m_vertex.create();
  m_vertex.bind();
  m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_object.create();
  m_vertex.release();
  shaderProgram->release();
  update();
}

void GLWindow::resizeGL(int w, int h)
{

}

void GLWindow::reload()
{
  m_shaderDirty = true;
  update();
}

void GLWindow::paintGL()
{

  glClear(GL_COLOR_BUFFER_BIT);
  if(m_shaderDirty)
  {
    auto frag = model.fragment() +
  R"_(
   void main(void)
   {
     mainImage(gl_FragColor, gl_FragCoord);
   };
  )_";
    shader->compileSourceCode(frag.toUtf8().constData());
    shaderProgram->link();
    m_shaderDirty = false;
  }

  {
    shaderProgram->bind();
    for(auto& val : m_values)
    {
      shaderProgram->setUniformValue(val.first.data(), ossia::convert<float>(val.second));
    }

    {
      m_object.bind();
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      m_object.release();
    }
    shaderProgram->release();
  }
  update();
}

void GLWindow::slt_setValue(std::string s, ossia::value v)
{
  m_values[std::move(s)] = std::move(v);
  update();
}

}

template <>
void DataStreamReader::read(
    const Shader::ProcessModel& proc)
{
  insertDelimiter();
}

template <>
void DataStreamWriter::write(
    Shader::ProcessModel& proc)
{
  checkDelimiter();
}

template <>
void JSONObjectReader::read(
    const Shader::ProcessModel& proc)
{
}

template <>
void JSONObjectWriter::write(
    Shader::ProcessModel& proc)
{
}
