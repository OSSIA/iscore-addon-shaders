#pragma once
#include <Process/LayerView.hpp>

namespace Shader
{
class ShaderView final : public Process::LayerView
{
public:
  explicit ShaderView(QGraphicsItem* parent);

private:
  void paint_impl(QPainter*) const override;
};
}
