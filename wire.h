#pragma once

#include <component.h>

class Wire : public QObject {
	
	private:
	Component *leftComponent;
	Component *rightComponent;
	bool deleteFlag;
	
	public:
	Wire(Component *l = NULL, Component *r = NULL);
	void setLeftComponent(Component*);
	void setRightComponent(Component*);
	Component* getLeftComponent();
	Component* getRightComponent();
	QPoint getLeftPoint();
	QPoint getRightPoint();
	void setDeleteFlag();
	void resetDeleteFlag();
	bool getDeleteFlag();
	
};