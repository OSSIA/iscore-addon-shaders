#include <score/command/Dispatchers/CommandDispatcher.hpp>

#include <Shader/ShaderModel.hpp>
#include <Shader/ShaderPresenter.hpp>
#include <Shader/ShaderView.hpp>

namespace Shader
{
ShaderPresenter::ShaderPresenter(
    const ProcessModel& layer,
    ShaderView* view,
    const Process::ProcessPresenterContext& ctx,
    QObject* parent)
    : LayerPresenter{ctx, parent}, m_layer{layer}, m_view{view}
{
}

void ShaderPresenter::setWidth(qreal val)
{
  m_view->setWidth(val);
}

void ShaderPresenter::setHeight(qreal val)
{
  m_view->setHeight(val);
}

void ShaderPresenter::putToFront() {}

void ShaderPresenter::putBehind() {}

void ShaderPresenter::on_zoomRatioChanged(ZoomRatio) {}

void ShaderPresenter::parentGeometryChanged() {}

const Process::ProcessModel& ShaderPresenter::model() const
{
  return m_layer;
}

const Id<Process::ProcessModel>& ShaderPresenter::modelId() const
{
  return m_layer.id();
}
}
