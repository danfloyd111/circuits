#pragma once

#include <QWidget>
#include <QPointer>
#include <QMenu>
#include <QMouseEvent>
#include <ctime>

class Component : public QWidget {
	
	Q_OBJECT
	
	signals:
	void deleteSig(int code); // emitted to the parent while deleting
	void wireSig(int code, Component* c); // emitted to the parent while connecting
	void valueChangedSig(int code, Component* c, int oldValue, int newValue); // emitted to the parent while changing the value
	void deleteWireSig(int code, Component *c); // emitted to the parent while detaching from other components
    void dividerSig(int code, Component *c);// emitted to the parent while changing the diver's divider
	
	private slots:
	void deleteSlot(); // called when the user wants to delete a component
	void wireSlot(); // called when the user wants to connect a compoenent
	void deleteWireSlot(); // called when the user wants to disconnect a compoenent
    void dividerSlot(); // called when the user wants to change the divider of a diver
	
	private:
    void emitDividerSig();
	void emitDeleteSig();
	void emitWireSig();
	void emitValueChangedSig(int, int);
	void emitDeleteWireSig();
	void createActions();
	bool deleteFlag;
	QPoint moveOffset;
	QAction *deleteAct;
	QAction *wireAct;
	QAction *deleteWireAct;
    QAction *dividerAct;
	int val; // value of this component
	int componentType;
    int id; // the id of this component, it's unique because is the epoch
	
	protected:
	void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void setComponentType(int t);
	int MENU_OFFSET;
	
	public:
	Component(QWidget *parent);
	QList< QPointer<Component> > inComponentList; // list of incoming components
	QList< QPointer<Component> > outComponentList; // list of outcoming components
	bool isDead();
	void setValue(int);
	int getValue();
	int getComponentType();
    int getId();
    void setId(int); // used only to restore a saved file
};