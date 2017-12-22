#pragma once
#include <Process/Process.hpp>
#include <Shader/ShaderMetadata.hpp>
#include <score/model/EntityMap.hpp>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QVector3D>
#include <boost/container/flat_map.hpp>
#include <ossia-qt/js_utilities.hpp>
#include <ossia/dataflow/data.hpp>
#include <isf.hpp>
class QOpenGLShader;
class QOpenGLShaderProgram;
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(ossia::audio_vector)
namespace Shader
{
class ProcessModel;
class GLWindow : public QOpenGLWindow, public QOpenGLFunctions
{
    Q_OBJECT
  public:
    GLWindow(ProcessModel& m);

  void initializeGL() override;

  void resizeGL(int w, int h) override;

  void reload();
  void paintGL() override;

  QOpenGLShader* shader{};
  signals:
    void sig_setValue(std::string, ossia::value);
    void sig_setAudio(std::string, ossia::audio_vector);

  public slots:
    void slt_setValue(std::string, ossia::value);
    void slt_setAudio(std::string, ossia::audio_vector);

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

    Q_OBJECT
    Q_PROPERTY(QString shader READ shader WRITE setShader NOTIFY shaderChanged)
    public:
      ProcessModel(const TimeVal& duration,
                   const Id<Process::ProcessModel>& id,
                   QObject* parent);
      ~ProcessModel();
    template<typename Impl>
    ProcessModel(Impl& vis, QObject* parent) :
      Process::ProcessModel{vis, parent}
    {
      vis.writeTo(*this);
    }

    QString shader() const;
    std::string fragment() const;
    GLWindow* window() const { return m_window; }

  public slots:
    void setShader(QString shader);
  signals:
    void shaderChanged(QString shader);

  private:

    QString prettyName() const override;
    void startExecution() override;
    void stopExecution() override;
    void reset() override;
    ProcessStateDataInterface*startStateData() const override;
    ProcessStateDataInterface*endStateData() const override;
    Selection selectableChildren() const override;
    Selection selectedChildren() const override;
    void setSelection(const Selection& s) const override;

    void setDurationAndScale(const TimeVal& newDuration) override;
    void setDurationAndGrow(const TimeVal& newDuration) override;
    void setDurationAndShrink(const TimeVal& newDuration) override;
    QString m_shader;
    GLWindow* m_window{};

    // ProcessModel interface
public:
    Process::Inlets inlets() const override;
    Process::Outlets outlets() const override;

    Process::Inlets m_inlets;
    Process::Outlets m_outlets;
    std::unique_ptr<isf::parser> m_parser;
};
}
