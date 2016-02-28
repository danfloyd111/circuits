#include <muler.h>

Muler::Muler(QWidget *parent) : Component(parent) {
	resize(50,50);
	setComponentType(8);
	setValue(1);
}

void Muler::paintEvent(QPaintEvent*) {
	// painter and symbol settings
	QPainter p(this);
	p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	QString star = "*";
	// font customization
	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	p.setFont(font);
	// pen and brush customizations
	QPen linePen;
	linePen.setWidth(2);
	linePen.setBrush(Qt::black);
	linePen.setCapStyle(Qt::RoundCap);
	linePen.setJoinStyle(Qt::RoundJoin);
	p.setPen(linePen);
	p.setBrush(Qt::green);
	// drawing border, ellipse and symbol, the offset is needed beacuse the pen's font is 2px wide
	p.drawEllipse(rect().x()+1, rect().y()+1, rect().width()-2, rect().height()-2);
	p.drawText(rect().x(), rect().y(), rect().width()-1, rect().height()-1, Qt::AlignCenter, star);
}