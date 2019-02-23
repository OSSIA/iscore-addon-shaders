#include "ShaderView.hpp"

#include <Process/Style/ScenarioStyle.hpp>

#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
namespace Shader
{

ShaderView::ShaderView(QGraphicsItem* parent) : LayerView{parent}
{
  this->setFlag(QGraphicsItem::ItemClipsToShape, true);
  this->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
}

void ShaderView::paint_impl(QPainter* painter) const
{
  painter->setPen(Qt::white);
  painter->drawText(boundingRect(), "Shader", QTextOption(Qt::AlignCenter));
}
}
