#include <component.h>

Component::Component(QWidget* parent) : QWidget(parent) {
	deleteFlag = false;
	MENU_OFFSET = 20;
    id = std::time(0) + (rand() % 1000); // setting the id (epoch)
	createActions();
}

bool Component::isDead() {
	return deleteFlag;
}

void Component::setValue(int v){
	int oldv = val;
	val = v;
	emitValueChangedSig(oldv, v);
}

int Component::getValue(){
	return val;
}

void Component::mousePressEvent(QMouseEvent *event) {
	moveOffset = event->pos();
}

void Component::mouseMoveEvent(QMouseEvent *event) {
	int eX = (int) event->windowPos().x();
	int eY = ((int) event->windowPos().y()) - MENU_OFFSET; // y_offset correction
	move(eX - moveOffset.x(), eY - moveOffset.y());
	QWidget * p = (QWidget*) parent();
	p->repaint();
}

void Component::contextMenuEvent(QContextMenuEvent *event) {
	QMenu menu(this);
	if(getComponentType()!=2) // if is not a collector
		menu.addAction(wireAct);
    if(getComponentType()==7) // if is a diver
        menu.addAction(dividerAct);
	if(outComponentList.isEmpty() && inComponentList.isEmpty())
		deleteWireAct->setEnabled(false);
	else
		deleteWireAct->setEnabled(true);
	menu.addAction(deleteWireAct);
	menu.addSeparator();
	menu.addAction(deleteAct);
	menu.exec(event->globalPos());
}

void Component::createActions() {
    // delete
	deleteAct = new QAction(tr("&Delete"), this);
	deleteAct->setStatusTip(tr("Delete this variable"));
	connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteSlot()));
    // connect
	wireAct = new QAction(tr("&Connect"), this);
	wireAct->setStatusTip(tr("Connect this element with another"));
	connect(wireAct, SIGNAL(triggered()), this, SLOT(wireSlot()));
    // disconnect
	deleteWireAct = new QAction(tr("&Disconnect"), this);
	deleteWireAct->setStatusTip(tr("Disconnect this element from others"));
	connect(deleteWireAct, SIGNAL(triggered()), this, SLOT(deleteWireSlot()));
    // change divider if the component is a diver
    dividerAct = new QAction(tr("&Change divider"), this);
    dividerAct->setStatusTip(tr("Change the current divider, by default it's neutral"));
    connect(dividerAct, SIGNAL(triggered()), this, SLOT(dividerSlot()));
}

// Slots

void Component::dividerSlot() {
    emitDividerSig();
}

void Component::deleteSlot() {
	deleteFlag = true;
	emitDeleteSig();
}

void Component::wireSlot() {
	emitWireSig();
}

void Component::deleteWireSlot() {
	emitDeleteWireSig();
}

// Emitters

void Component::emitDividerSig() {
    emit dividerSig(100, this);
}

void Component::emitDeleteSig() {
	emit deleteSig(100);
}

void Component::emitWireSig() {
	emit wireSig(100, this);
}

void Component::emitValueChangedSig(int oldval, int newval) {
	emit valueChangedSig(100, this, oldval, newval);
}

void Component::emitDeleteWireSig() {
	emit deleteWireSig(100, this);
}

// Other stuff

void Component::setComponentType(int t) {
	componentType = t;
}

int Component::getComponentType() {
	return componentType;
}

int Component::getId() {
    return id;
}

void Component::setId(int nid) {
    id = nid;
}