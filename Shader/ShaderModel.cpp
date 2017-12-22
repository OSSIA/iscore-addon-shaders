#include "ShaderModel.hpp"
#include <QOpenGLShaderProgram>
#include <Process/Dataflow/Port.hpp>

namespace Shader
{

ProcessModel::ProcessModel(
    const TimeVal& duration,
    const Id<Process::ProcessModel>& id,
    QObject* parent):
  Process::ProcessModel{duration, id, "Shader", parent}
{
  metadata().setInstanceName(*this);
  m_shader =
      R"_(
/*
{
  "CATEGORIES" : [
    "Generator"
  ],
  "DESCRIPTION" : "Visualizes an FFT analysis image with custom set colors for frequency domain",
  "INPUTS" : [
    {
      "NAME" : "waveImage",
      "TYPE" : "audio"
    }
  ],
  "CREDIT" : "by VIDVOX"
}
*/



void main()
{
 gl_FragColor.r = IMG_PIXEL(waveImage, vec2(isf_FragNormCoord[0], isf_FragNormCoord[1] / 256.)).r;
 gl_FragColor.g = IMG_PIXEL(waveImage, vec2(isf_FragNormCoord[0], isf_FragNormCoord[1] / 256.)).g;
 gl_FragColor.b = IMG_PIXEL(waveImage, vec2(isf_FragNormCoord[0], isf_FragNormCoord[1] / 256.)).b;
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

QString ProcessModel::shader() const
{
  return m_shader;
}

std::string ProcessModel::fragment() const
{
  if(m_parser)
    return m_parser->fragment();
  return {};
}
struct shader_visitor
{
    int& i;
    ProcessModel& parent;
    Process::Inlet* operator()(const isf::float_input& v)
    {
      auto p = new Process::ControlInlet(Id<Process::Port>(i++), &parent);
      p->type = Process::PortType::Message;
      return p;
    }
    Process::Inlet* operator()(const isf::long_input& v)
    {
      auto p = new Process::Inlet(Id<Process::Port>(i++), &parent);
      p->type = Process::PortType::Message;
      return p;
    }
    Process::Inlet* operator()(const isf::event_input& v)
    {
      auto p = new Process::Inlet(Id<Process::Port>(i++), &parent);
      p->type = Process::PortType::Message;
      return p;
    }
    Process::Inlet* operator()(const isf::bool_input& v)
    {
      auto p = new Process::Inlet(Id<Process::Port>(i++), &parent);
      p->type = Process::PortType::Message;
      return p;
    }
    Process::Inlet* operator()(const isf::point2d_input& v)
    {
      auto p = new Process::Inlet(Id<Process::Port>(i++), &parent);
      p->type = Process::PortType::Message;
      return p;
    }
    Process::Inlet* operator()(const isf::point3d_input& v)
    {
      auto p = new Process::Inlet(Id<Process::Port>(i++), &parent);
      p->type = Process::PortType::Message;
      return p;
    }
    Process::Inlet* operator()(const isf::color_input& v)
    {
      auto p = new Process::Inlet(Id<Process::Port>(i++), &parent);
      p->type = Process::PortType::Message;
      return p;
    }
    Process::Inlet* operator()(const isf::image_input& v)
    {
      return nullptr;
    }
    Process::Inlet* operator()(const isf::audio_input& v)
    {
      auto p = new Process::Inlet(Id<Process::Port>(i++), &parent);
      p->type = Process::PortType::Audio;
      return p;
    }
    Process::Inlet* operator()(const isf::audioFFT_input& v)
    {
      return nullptr;
    }

};

void ProcessModel::setShader(QString shader)
{
  if (m_shader != shader)
  {
    m_shader = shader;
    qDeleteAll(m_inlets);
    qDeleteAll(m_outlets);
    m_inlets.clear();
    m_outlets.clear();
    m_parser.reset();

    try {
      m_parser = std::make_unique<isf::parser>(shader.toStdString());

      auto f = QString::fromStdString(m_parser->fragment());

      int i = 0;
      for(auto input : m_parser->data().inputs)
      {
        if(auto p = std::visit(shader_visitor{i, *this}, input.data))
        {
          p->setCustomData(QString::fromStdString(input.name));
          m_inlets.push_back(p);
        }
      }
    }
    catch(const std::exception& e)
    {
    }

    m_window->reload();
    emit shaderChanged(m_shader);
    emit inletsChanged();
    emit outletsChanged();

  }
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

Process::Inlets ProcessModel::inlets() const
{
  return m_inlets;
}

Process::Outlets ProcessModel::outlets() const
{
  return m_outlets;
}

GLWindow::GLWindow(ProcessModel& m):
  model{m}
{
  QSurfaceFormat f;
  f.setProfile(QSurfaceFormat::CoreProfile);
  f.setVersion(4, 1);
  setFormat(f);
  connect(this, &GLWindow::sig_setValue,
          this, &GLWindow::slt_setValue,
          Qt::QueuedConnection);
  connect(this, &GLWindow::sig_setAudio,
          this, &GLWindow::slt_setAudio,
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


  shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, model.fragment().c_str());
  if(shaderProgram->shaders().size() < 2)
  {
    qDebug() << "INVALID SHADERS;";
    return;
  }

  shader = shaderProgram->shaders()[1];

  shaderProgram->link();
  shaderProgram->bind();

  m_vertex.create();
  m_vertex.bind();
  m_vertex.setUsagePattern(QOpenGLBuffer::DynamicDraw);
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

  // Recompute the shader if necessary
  if(m_shaderDirty)
  {
    // Cleanup
    m_audio.clear();
    m_audioBuffers.clear();
    for(auto tex : m_audioTextures)
    {
      delete tex.second;
    }
    m_audioTextures.clear();


    if(!shader)
      initializeGL();
    if(!shader)
      return;
    shader->compileSourceCode(model.fragment().c_str());
    shaderProgram->link();
    qDebug() << shaderProgram->log();

    // Recreate audio textures
    for(const auto& inlet: model.inlets())
    {
      if(inlet->type == Process::PortType::Audio)
      {
        auto tex = new QOpenGLTexture{QOpenGLTexture::Target2D};
        tex->setFormat(QOpenGLTexture::R32F);
        tex->setMagnificationFilter(QOpenGLTexture::Linear);
        tex->setMinificationFilter(QOpenGLTexture::Linear);
        tex->setSize(512, 2);

        tex->allocateStorage();
        // tex->setSize(val.second[0].size(), val.second.size());
        m_audioTextures[inlet->customData().toStdString()] = tex;
      }
    }
    m_shaderDirty = false;
  }

  shaderProgram->bind();

  {
    // Bind audio textures
    ossia::small_vector<QOpenGLTexture*, 8> ok;

    for(auto& val : m_audio)
    {
      if(val.second.empty())
        continue;

      auto tex = m_audioTextures.at(val.first);

      const auto bs = val.second[0].size();
      if(bs == 0)
        continue;

      // Copy audio data in a flat buffer
      auto& v = m_audioBuffers[val.first];

      v.resize(std::max(val.second.size() * bs, 1024UL));

      for(std::size_t chan = 0; chan < val.second.size(); chan++)
      {
        for(std::size_t i = 0; i < bs; i++)
        {
          v[chan * bs + i] = (val.second[chan][i] + 1.) / 2.;
        }
      }

      // Put the data in the texture
      tex->setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, v.data());

      tex->bind(4);

      shaderProgram->setUniformValue(val.first.data(), 4);
      ok.push_back(tex);
    }
    m_audio.clear();


    // Bind floats
    for(auto& val : m_values)
    {
      shaderProgram->setUniformValue(val.first.data(), ossia::convert<float>(val.second));
    }

    // Draw
    {
      m_object.bind();
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      m_object.release();
    }
    for(auto& t : ok)
    {
      t->release();
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

void GLWindow::slt_setAudio(std::string s, ossia::audio_vector v)
{
  auto& local = m_audio[std::move(s)];
  local.resize(std::max(local.size(), v.size()));

  for(std::size_t i = 0; i < local.size(); i++)
  {
    local[i].insert(local[i].end(), v[i].begin(), v[i].end());
  }



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
