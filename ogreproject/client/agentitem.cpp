#include "agentitem.h"
#include <QDebug>
#include <QPainter>

AgentItem::AgentItem(QGraphicsItem *parent)
		: QGraphicsWidget(parent)
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setAcceptHoverEvents(true);
	_hovered=false;
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	setMaximumSize(100,100);
	setMinimumSize(100,100);
}


AgentItem::~AgentItem()
{
}

void AgentItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if(_hovered)
		painter->setPen(QPen(QBrush(QColor(255,255,255)),4));
	else
		painter->setPen(QPen(QBrush(QColor(255,255,255)),2));
	
	if(isSelected())
		painter->setBrush(QBrush(QColor(135,170,222)));
	else
		painter->setBrush(QBrush(QColor(255,230,128)));
	
	painter->drawRoundedRect(boundingRect(),5,5);
}

/*

QRectF AgentItem::boundingRect() const
{
	return(QRectF(0,0,100,100));
}

*/

QPainterPath AgentItem::shape () const
{
	QPainterPath path;
	path.addRoundedRect(boundingRect(),5,5);
	return(path);
}

void AgentItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	_hovered=true;
	qDebug() << "Hover In Event!";
	update(boundingRect());
	QGraphicsWidget::hoverEnterEvent(event);
}

void AgentItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	_hovered=false;
	qDebug() << "Hover Out Event!";
	update(boundingRect());
	QGraphicsWidget::hoverLeaveEvent(event);
}
