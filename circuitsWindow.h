#pragma once

#include <QMainWindow>
#include <QtWidgets>
#include <QWidget>
#include <QCloseEvent>

#include <circuitsWidget.h>
#include <variable.h>
#include <collector.h>
#include <adder.h>
#include <ander.h>
#include <orer.h>
#include <noter.h>
#include <diver.h>
#include <muler.h>

class CircuitsWindow : public QMainWindow {
	
	Q_OBJECT
	
	private:
	CircuitsWidget *widget;
	void createActions();
	void createMenus();
	QAction *newAct;
	QAction *openAct;
	QAction *saveAct;
	QAction *undoAct; // here for the beta release
	QAction *redoAct; // here for the beta release
	QAction *copyAct; // here for the beta release
	QAction *pasteAct; // here for the beta release
	QAction *adderAct;
    QAction *anderAct;
    QAction *orerAct;
    QAction *noterAct;
    QAction *diverAct;
    QAction *mulerAct;
	QAction *exitAct;
	QAction *varAct;
	QAction *colAct;
    QAction *clearAct;
    QAction *scrollUpAct;
    QAction *scrollDownAct;
    QAction *scrollLeftAct;
    QAction *scrollRightAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *mathMenu;
    QMenu *logicMenu;
	QMenu *varMenu;
    QMenu *viewMenu;
    bool saved;
	
	private slots:
	void newFile();
	void open();
	void save();
	void undo(); // here for the beta release
	void redo(); // here for the beta release
	void copy();
	void paste();
	void adder();
    void ander();
    void orer();
    void noter();
    void diver();
    void muler();
	void variable();
	void collector();
    void clearAll();
    void scrollUp();
    void scrollDown();
    void scrollLeft();
    void scrollRight();
    void zoomIn();
    void zoomOut();
	
	protected:
	void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
	
	public:
	CircuitsWindow(QWidget *parent = 0);
};