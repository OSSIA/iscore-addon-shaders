#include "ShaderView.hpp"
#include <Process/Style/ScenarioStyle.hpp>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QQuickWidget>
#include <QGraphicsProxyWidget>
#include <QApplication>
namespace Shader
{

ShaderView::ShaderView(
        QGraphicsItem* parent):
    LayerView{parent}
{
  this->setFlag(QGraphicsItem::ItemClipsToShape, true);
  this->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
}

void ShaderView::paint_impl(QPainter* painter) const
{
}

}
