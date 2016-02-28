#include <wire.h>

Wire::Wire(Component *l, Component *r) : QObject() {
	leftComponent = l;
	rightComponent = r;
	resetDeleteFlag();
}

void Wire::setLeftComponent(Component *c) {
	leftComponent = c;
}

void Wire::setRightComponent(Component *c) {
	rightComponent = c;
}

Component* Wire::getLeftComponent() {
	return leftComponent;
}

Component* Wire::getRightComponent() {
	return rightComponent;
}

QPoint Wire::getLeftPoint() {
	int x = leftComponent->x();
	int w = leftComponent->width() / 2;
	int y = leftComponent->y();
	int h = leftComponent->height() / 2;
	return QPoint(x+w, y+h);
}

QPoint Wire::getRightPoint() {
	int x = rightComponent->x();
	int w = rightComponent->width() / 2;
	int y = rightComponent->y();
	int h = rightComponent->height() / 2;
	return QPoint(x+w, y+h);
}

void Wire::setDeleteFlag() {
	deleteFlag = true;
}

void Wire::resetDeleteFlag() {
	deleteFlag = false;
}

bool Wire::getDeleteFlag() {
	return deleteFlag;
}