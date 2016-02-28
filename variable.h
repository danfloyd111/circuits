#pragma once

#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>

#include <component.h>

class Variable : public Component {
	
	Q_OBJECT
	
	private slots:
	void textChangedSlot(QString text);
	
	protected:
	void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
	
	public:
	Variable(QWidget *parent);
    QLineEdit *var;
	
};