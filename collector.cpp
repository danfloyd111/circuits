#include <collector.h>

Collector::Collector(QWidget *parent) : Component(parent) {
	resize(50,50);
	setValue(0);
	var = new QLabel(this);
	var->setAlignment(Qt::AlignCenter);
	var->setNum(getValue());
	QVBoxLayout *l = new QVBoxLayout();
	l->addWidget(var);
	setLayout(l);
	setComponentType(2);
}

void Collector::paintEvent(QPaintEvent*) {
	var->setNum(getValue());
	QPainter p(this);
	p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	//p.fillRect(rect(), Qt::cyan);
	QPen linePen;
	linePen.setWidth(2);
	linePen.setBrush(Qt::black);
	linePen.setCapStyle(Qt::RoundCap);
	linePen.setJoinStyle(Qt::RoundJoin);
	p.setPen(linePen);
	p.setBrush(Qt::cyan);
	p.drawEllipse(rect().x()+1, rect().y()+1, rect().width()-2, rect().height()-2);
	//p.drawEllipse(rect().x(), rect().y(), rect().width()-1, rect().height()-1);
}