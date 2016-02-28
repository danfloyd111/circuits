#include <variable.h>

Variable::Variable(QWidget *parent) : Component(parent) {
	resize(85,35);
	setValue(0);
	var = new QLineEdit(this);
	var->setPlaceholderText("0");
	//var->setFixedWidth(width()-20);
	var->resize(width()-20,var->y());
    var->setAlignment(Qt::AlignCenter);
	QVBoxLayout *l = new QVBoxLayout();
	l->addWidget(var);
	setLayout(l);
	connect(var, SIGNAL(textChanged(QString)), this, SLOT(textChangedSlot(QString)));
	setComponentType(3);
}

void Variable::paintEvent(QPaintEvent*) {
	QPainter p(this);
	p.fillRect(rect(), Qt::darkGreen);
	QPen linePen;
	linePen.setWidth(2);
	linePen.setBrush(Qt::black);
	linePen.setCapStyle(Qt::RoundCap);
	linePen.setJoinStyle(Qt::RoundJoin);
	p.setPen(linePen);
	p.drawRect(rect().x(), rect().y(), rect().width()-1, rect().height()-1);
}

void Variable::textChangedSlot(QString text) {
	// retrieving text and setting val
	int val = getValue();
	sscanf(text.toStdString().c_str(), "%d", &val);
	if(text.size() == 0)
		val = 0;
	setValue(val);
}