#pragma once

#include <QPainter>
#include <QPointer>
#include <QFont>

#include <component.h>

#include <sstream>

class Diver : public Component {
	
	Q_OBJECT
	
	protected:
	void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
	int divider;
    
	public:
	Diver(QWidget*);
    void setDivider(int d);
    int getDivider();
	
};