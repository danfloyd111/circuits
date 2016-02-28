#pragma once

#include <component.h>
#include <variable.h>
#include <collector.h>
#include <adder.h>
#include <ander.h>
#include <orer.h>
#include <noter.h>
#include <diver.h>
#include <muler.h>
#include <wire.h>

#include <QCursor>
#include <QPointer>
#include <QPainter>
#include <QInputDialog>
#include <QTimer>

#include <iostream>
#include <fstream>

using namespace std;

class CircuitsWidget : public QWidget {
	
	Q_OBJECT
	
	private:
	QList< QPointer<Wire> > wireList;
	void createComponentConnections(Component*);
	bool connectingComponent;
	Component* componentOnConnection;
    QTimer* timer;
    float quarticEaseOut(float, float, float, float);
    int timeElapsed;
    int* startposY;
    int finalposY;
    int* interposY;
	
	private slots:
	void deleteComponentSlot(int code);
	void wireSlot(int code, Component *c);
	void valueChangedSlot(int code, Component *c, int oldValue, int newValue);
	void deleteWireSlot(int code, Component *c);
    void dividerSlot(int code, Component *C);
    void timerTick();
	
	protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
	
	public:
	CircuitsWidget(QWidget *parent = 0);
    QList< QPointer<Component> > componentList;
	void drawComponent(Component *c);
	void serialize(QString fileName);
    void deserialize(QString fileName);
    void clearAll();
	
};