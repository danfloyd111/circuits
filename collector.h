#pragma once

#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>

#include <component.h>

class Collector : public Component {
	
	Q_OBJECT
	
	private:
	QLabel *var;
	
	protected:
	void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
	
	public:
	Collector(QWidget* parent);	
};