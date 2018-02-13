#pragma once
#include <Process/LayerPresenter.hpp>
#include <Shader/ShaderLayer.hpp>
#include <QPoint>

#include <Process/ZoomHelper.hpp>
#include <score/model/Identifier.hpp>
#include <Process/Focus/FocusDispatcher.hpp>

namespace Shader
{
class ProcessModel;
class ShaderView;
class ShaderPresenter final :
        public Process::LayerPresenter
{
    public:
        explicit ShaderPresenter(
                const ProcessModel& model,
                ShaderView* view,
                const Process::ProcessPresenterContext& ctx,
                QObject* parent);

        void setWidth(qreal width) override;
        void setHeight(qreal height) override;

        void putToFront() override;
        void putBehind() override;

        void on_zoomRatioChanged(ZoomRatio) override;

        void parentGeometryChanged() override;

        const Process::ProcessModel& model() const override;
        const Id<Process::ProcessModel>& modelId() const override;

    private:
        const Shader::ProcessModel& m_layer;
        ShaderView* m_view{};
};
}
