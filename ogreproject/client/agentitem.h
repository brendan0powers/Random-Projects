#ifndef AGENTITEM_H
#define AGENTITEM_H

#include <QGraphicsWidget>

/**
	@author Brendan Powers <brendan@resara.com>
*/
class AgentItem : public QGraphicsWidget
{
	Q_OBJECT
public:
	AgentItem(QGraphicsItem *parent = 0);

	~AgentItem();

	//QRectF boundingRect() const;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	QPainterPath shape() const;
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

private:
	bool _hovered;

};

#endif
