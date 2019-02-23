#pragma once
#include <Process/Process.hpp>

#include <score/model/EntityMap.hpp>

#include <ossia-qt/js_utilities.hpp>
#include <ossia/dataflow/data.hpp>

#include <boost/container/flat_map.hpp>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWindow>
#include <QVector3D>

#include <Shader/ShaderMetadata.hpp>
#include <isf.hpp>
class QOpenGLShader;
class QOpenGLShaderProgram;
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(ossia::audio_vector)
W_REGISTER_ARGTYPE(std::string)
W_REGISTER_ARGTYPE(ossia::audio_vector)
namespace Shader
{
class ProcessModel;
class GLWindow : public QOpenGLWindow, public QOpenGLFunctions
{
  W_OBJECT(GLWindow)
public:
  GLWindow(ProcessModel& m);

  void initializeGL() override;

  void resizeGL(int w, int h) override;

  void reload();
  void paintGL() override;

  QOpenGLShader* shader{};
  void sig_setValue(std::string s, ossia::value v)
      W_SIGNAL(sig_setValue, s, v);
  void sig_setAudio(std::string s, ossia::audio_vector v)
      W_SIGNAL(sig_setAudio, s, v);

  void slt_setValue(std::string, ossia::value);
  W_SLOT(slt_setValue)
  void slt_setAudio(std::string, ossia::audio_vector);
  W_SLOT(slt_setAudio)

private:
  ProcessModel& model;
  QOpenGLShaderProgram* shaderProgram{};

  QOpenGLBuffer m_vertex;
  QOpenGLVertexArrayObject m_object;

  QOpenGLBuffer m_vbo_position;
  QOpenGLBuffer m_vbo_index;
  QOpenGLBuffer m_vbo_tex_coord;

  boost::container::flat_map<std::string, ossia::value> m_values;
  boost::container::flat_map<std::string, ossia::audio_vector> m_audio;
  boost::container::flat_map<std::string, std::vector<float>> m_audioBuffers;
  boost::container::flat_map<std::string, QOpenGLTexture*> m_audioTextures;

  bool m_shaderDirty{false};
};

class ProcessModel final : public Process::ProcessModel
{
  SCORE_SERIALIZE_FRIENDS
  PROCESS_METADATA_IMPL(Shader::ProcessModel)

  W_OBJECT(ProcessModel)
public:
  ProcessModel(
      const TimeVal& duration,
      const Id<Process::ProcessModel>& id,
      QObject* parent);
  ~ProcessModel() override;
  template <typename Impl>
  ProcessModel(Impl& vis, QObject* parent) : Process::ProcessModel{vis, parent}
  {
    vis.writeTo(*this);
    init();
  }

  QString shader() const;
  std::string fragment() const;
  GLWindow* window() const { return m_window; }

  void setShader(QString shader);
  W_SLOT(setShader)
  void shaderChanged(QString shader) W_SIGNAL(shaderChanged, shader)

      W_PROPERTY(
          QString,
          shader READ shader WRITE setShader NOTIFY shaderChanged) private
      : void init();

  QString prettyName() const noexcept override;
  ProcessStateDataInterface* startStateData() const noexcept override;
  ProcessStateDataInterface* endStateData() const noexcept override;
  Selection selectableChildren() const noexcept override;
  Selection selectedChildren() const noexcept override;
  void setSelection(const Selection& s) const noexcept override;

  void setDurationAndScale(const TimeVal& newDuration) noexcept override;
  void setDurationAndGrow(const TimeVal& newDuration) noexcept override;
  void setDurationAndShrink(const TimeVal& newDuration) noexcept override;
  QString m_shader;
  GLWindow* m_window{};

  // ProcessModel interface
public:
  std::unique_ptr<isf::parser> m_parser;
};
}
