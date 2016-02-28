#include <diver.h>

Diver::Diver(QWidget *parent) : Component(parent) {
	resize(50,50);
	setComponentType(7);
	setValue(0);
    setDivider(1);
}

void Diver::paintEvent(QPaintEvent*) {
	// painter and symbol settings
	QPainter p(this);
	p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    std::stringstream sstm;
    sstm << "/" << getDivider();
    std::string div;
    div = sstm.str();
    QString ddiv = QString::fromStdString(div);
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
	p.drawText(rect().x(), rect().y(), rect().width()-1, rect().height()-1, Qt::AlignCenter, ddiv);
}

int Diver::getDivider() {
    return divider;
}

void Diver::setDivider(int d) {
    divider = d;
}