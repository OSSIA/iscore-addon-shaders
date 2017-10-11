#pragma once
#include <Process/Process.hpp>
#include <Shader/ShaderMetadata.hpp>
#include <score/model/EntityMap.hpp>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <boost/container/flat_map.hpp>
#include <ossia-qt/js_utilities.hpp>
class QOpenGLShader;
class QOpenGLShaderProgram;
Q_DECLARE_METATYPE(std::string)
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

  public slots:
    void slt_setValue(std::string, ossia::value);

  private:
    ProcessModel& model;
  QOpenGLShaderProgram* shaderProgram{};

  QOpenGLBuffer m_vertex;
  QOpenGLVertexArrayObject m_object;
  boost::container::flat_map<std::string, ossia::value> m_values;

  bool m_shaderDirty{false};
};


class ProcessModel final : public Process::ProcessModel
{
    SCORE_SERIALIZE_FRIENDS
    PROCESS_METADATA_IMPL(Shader::ProcessModel)

    Q_OBJECT
    Q_PROPERTY(QString fragment READ fragment WRITE setFragment NOTIFY fragmentChanged)
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

    QString fragment() const;
    GLWindow* window() const { return m_window; }

  public slots:
    void setFragment(QString fragment);
  signals:
    void fragmentChanged(QString fragment);

  private:
    ProcessModel(const ProcessModel& source,
                 const Id<Process::ProcessModel>& id,
                 QObject* parent);

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
    QString m_fragment;
    GLWindow* m_window{};
};
}
