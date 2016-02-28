#include <circuitsWindow.h>

CircuitsWindow::CircuitsWindow(QWidget *parent) : QMainWindow(parent) {
	widget = new CircuitsWidget();
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    widget->setAutoFillBackground(true);
    widget->setPalette(pal);
	setCentralWidget(widget);
	createActions();
	createMenus();
	QString message = tr("A context menu is available by right-click");
	statusBar()->showMessage(message);
	setWindowTitle("Circuits (Alpha Release) v1.3");
	setMinimumSize(160, 160);
	resize(780, 520);
    saved = true;
}

void CircuitsWindow::keyPressEvent(QKeyEvent *event) {
	switch(event->key()){
		case(Qt::Key_Left):{
			widget->scroll(-10,0);
			break;
		}
		case(Qt::Key_Right):{
			widget->scroll(10,0);
			break;
		}
		case(Qt::Key_Up):{
			widget->scroll(0,-10);
			break;
		}
		case(Qt::Key_Down):{
			widget->scroll(0,10);
			break;
		}
	}
    saved = false;
}

void CircuitsWindow::createActions() {
	// n.b. QAction.setShortcuts(QKeySequence key) binds automatically key event to the current action
	// this is the "new" action
	newAct = new QAction(tr("&New"), this);
	newAct->setShortcuts(QKeySequence::New);
	newAct->setStatusTip(tr("Create a new program"));
	connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
	// this is the "open" action
	openAct = new QAction(tr("&Open"), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Load an existing program"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
	// this is the "save" action
	saveAct = new QAction(tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the current program"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
	// this is the "exit" action
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    // this is the "undo" action
    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));
    // this is the "redo" action
    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));
    // this is the "copy" action
    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));
    // this is the "paste" action
    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));
    // this is the "adder" action
    adderAct = new QAction(tr("&Adder"), this);
    adderAct->setShortcut(tr("Ctrl+A"));
    adderAct->setStatusTip("Draw a draggable adder");
    connect(adderAct, SIGNAL(triggered()), this, SLOT(adder()));
    // this is the "ander" action
    anderAct = new QAction(tr("&AND Port"), this);
    anderAct->setShortcut(tr("Ctrl+E"));
    anderAct->setStatusTip("Draw a draggable AND port");
    connect(anderAct, SIGNAL(triggered()), this, SLOT(ander()));
    // this is the "orer" action
    orerAct = new QAction(tr("&OR Port"), this);
    orerAct->setShortcut(tr("Ctrl+W"));
    orerAct->setStatusTip("Draw a draggable OR port");
    connect(orerAct, SIGNAL(triggered()), this, SLOT(orer()));
    // this is the "noter" action
    noterAct = new QAction(tr("&NOT Port"), this);
    noterAct->setShortcut(tr("Ctrl+G"));
    noterAct->setStatusTip("Draw a draggable NOT port");
    connect(noterAct, SIGNAL(triggered()), this, SLOT(noter()));
    // this is the "diver" action
    diverAct = new QAction(tr("&Divider"), this);
    diverAct->setShortcut(tr("Ctrl+J"));
    diverAct->setStatusTip("Draw a draggable divider");
    connect(diverAct, SIGNAL(triggered()), this, SLOT(diver()));
    // this is the "muler" action
    mulerAct = new QAction(tr("&Multiplier"), this);
    mulerAct->setShortcut(tr("Ctrl+X"));
    mulerAct->setStatusTip("Draw a draggable multiplier");
    connect(mulerAct, SIGNAL(triggered()), this, SLOT(muler()));
    // this is the "variable" action
    varAct = new QAction(tr("&Variable"), this);
    varAct->setShortcut((tr("Ctrl+K")));
    varAct->setStatusTip("Draw a draggable text label");
    connect(varAct, SIGNAL(triggered()), this, SLOT(variable()));
    // this is the "collector" action
    colAct = new QAction(tr("&Collector"), this);
    colAct->setShortcut((tr("Ctrl+M")));
    colAct->setStatusTip("Draw a draggable collector label");
    connect(colAct, SIGNAL(triggered()), this, SLOT(collector()));
    // this is the "clear" action
    clearAct = new QAction(tr("&Clear all"), this);
    clearAct->setShortcut((tr("Ctrl+Y")));
    clearAct->setStatusTip("Clear the canvas");
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clearAll()));
    // this is the "scrollUp" action
    scrollUpAct = new QAction(tr("&Scroll up"), this);
    scrollUpAct->setStatusTip("Scroll up the view");
    connect(scrollUpAct, SIGNAL(triggered()), this, SLOT(scrollUp()));
    // this is the "scrollDown" action
    scrollDownAct = new QAction(tr("&Scroll down"), this);
    scrollDownAct->setStatusTip("Scroll down the view");
    connect(scrollDownAct, SIGNAL(triggered()), this, SLOT(scrollDown()));
    // this is the "scrollLeft" action
    scrollLeftAct = new QAction(tr("&Scroll left"), this);
    scrollLeftAct->setStatusTip("Scroll left the view");
    connect(scrollLeftAct, SIGNAL(triggered()), this, SLOT(scrollLeft()));
    // this is the "scrollRight" action
    scrollRightAct = new QAction(tr("&Scroll right"), this);
    scrollRightAct->setStatusTip("Scroll right the view");
    connect(scrollRightAct, SIGNAL(triggered()), this, SLOT(scrollRight()));
    // this is the "zoomIn" action
    zoomInAct = new QAction(tr("&Zoom in"), this);
    zoomInAct->setStatusTip("Zoom in the view");
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
    // this is the "zoomOut" action
    zoomOutAct = new QAction(tr("&Zoom out"), this);
    zoomOutAct->setStatusTip("Zoom out the view");
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
}

void CircuitsWindow::createMenus() {
	// file menu
	fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    // edit menu
	editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(clearAct);
	editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    // view menu
    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(scrollUpAct);
    viewMenu->addAction(scrollDownAct);
    viewMenu->addAction(scrollLeftAct);
    viewMenu->addAction(scrollRightAct);
    viewMenu->addSeparator();
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    // BETA ONLY: These actions will be active on the Beta Release
    undoAct->setEnabled(false);
    redoAct->setEnabled(false);
    copyAct->setEnabled(false);
    pasteAct->setEnabled(false);
    // END BETA ONLY
  	// math menu
    mathMenu = menuBar()->addMenu(tr("&Mathematic"));
    mathMenu->addAction(adderAct);
    mathMenu->addAction(diverAct);
    mathMenu->addAction(mulerAct);
    // logic menu
    logicMenu = menuBar()->addMenu(tr("&Logic"));
    logicMenu->addAction(anderAct);
    logicMenu->addAction(orerAct);
    logicMenu->addAction(noterAct);
    // variables menu
    varMenu = menuBar()->addMenu(tr("&Variables"));
    varMenu->addAction(varAct);
    varMenu->addAction(colAct);
}

void CircuitsWindow::contextMenuEvent(QContextMenuEvent *event) {
	QMenu menu(this);
	menu.addAction(copyAct);
	menu.addAction(pasteAct);
	menu.exec(event->globalPos());
}

void CircuitsWindow::newFile(){
    statusBar()->showMessage("Creating new program");
    clearAll();
    statusBar()->showMessage("Done");
}

void CircuitsWindow::open(){
    statusBar()->showMessage("Opening existing program");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load program"), "", "");
    widget->deserialize(fileName);
    statusBar()->showMessage("Program opened");
    saved = true;
}

void CircuitsWindow::save(){
    statusBar()->showMessage("Saving current program");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save program"), "", "");
    widget->serialize(fileName);
    statusBar()->showMessage("Program saved");
    saved = true;
}

// These are for the beta release

void CircuitsWindow::undo(){
    statusBar()->showMessage("Undoing last action");
    saved = false;
}

void CircuitsWindow::redo(){
    statusBar()->showMessage("Redoing last action");
    saved = false;
}

void CircuitsWindow::copy(){
    statusBar()->showMessage("Copying");
    saved = false;
}

void CircuitsWindow::paste(){
    statusBar()->showMessage("Pasting");
    saved = false;
}

// end beta

void CircuitsWindow::adder() {
	statusBar()->showMessage("Drawing an adder");
	Adder *a = new Adder(widget);
	widget->drawComponent(a);
	statusBar()->showMessage("Done");
    saved = false;
}

void CircuitsWindow::orer() {
    statusBar()->showMessage("Drawing an OR port");
    Orer *o = new Orer(widget);
    widget->drawComponent(o);
    statusBar()->showMessage("Done");
    saved = false;
}

void CircuitsWindow::ander() {
    statusBar()->showMessage("Drawing an AND port");
    Ander *a = new Ander(widget);
    widget->drawComponent(a);
    statusBar()->showMessage("Done");
    saved = false;
}

void CircuitsWindow::noter() {
    statusBar()->showMessage("Drawing a NOT port");
    Noter *n = new Noter(widget);
    widget->drawComponent(n);
    statusBar()->showMessage("Done");
    saved = false;
}

void CircuitsWindow::diver() {
    statusBar()->showMessage("Drawing a divider");
    Diver *d = new Diver(widget);
    widget->drawComponent(d);
    statusBar()->showMessage("Done");
    saved = false;
}

void CircuitsWindow::muler() {
    statusBar()->showMessage("Drawing a multiplier");
    Muler *m = new Muler(widget);
    widget->drawComponent(m);
    statusBar()->showMessage("Done");
    saved = false;
}

void CircuitsWindow::variable() {
    statusBar()->showMessage("Drawing a text label");
	Variable *v = new Variable(widget);
	widget->drawComponent(v);
    statusBar()->showMessage("Done");
    saved = false;
}

void CircuitsWindow::collector() {
    statusBar()->showMessage("Drawing a collector");
	Collector *c = new Collector(widget);
	widget->drawComponent(c);
    statusBar()->showMessage("Done");
    saved = false;
}

void CircuitsWindow::clearAll() {
    statusBar()->showMessage("Clearing canvas");
    if(!saved){
        QMessageBox msgBox;
        msgBox.setText("The program has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save:{
                // Save was clicked
                QString fileName = QFileDialog::getSaveFileName(this, tr("Save program"), "", "");
                widget->serialize(fileName);
                widget->clearAll();
                break;
            }
            case QMessageBox::Discard:{
                // Don't Save was clicked
                widget->clearAll();
                break;
            }
            case QMessageBox::Cancel:{
                // Cancel was clicked, do nothing
                break;
            }
            default:{
                // should never be reached
                break;
            }
        }
    }
    else {
        widget->clearAll();
    }
    statusBar()->showMessage("Done");
}

void CircuitsWindow::closeEvent(QCloseEvent *event) {
    if(!saved){
        QMessageBox msgBox;
        msgBox.setText("The program has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save:{
                // Save was clicked
                QString fileName = QFileDialog::getSaveFileName(this, tr("Save program"), "", "");
                widget->serialize(fileName);
                widget->clearAll();
                //close();
                break;
            }
            case QMessageBox::Discard:{
                // Don't Save was clicked
                widget->clearAll();
                //close();
                break;
            }
            case QMessageBox::Cancel:{
                // Ignore the event
                event->ignore();
                break;
            }
            default:{
                // should never be reached
                break;
            }
        }
    }
}

void CircuitsWindow::scrollUp() {
    widget->scroll(0,-10);
    saved = false;
}

void CircuitsWindow::scrollDown() {
    widget->scroll(0,10);
    saved = false;
}

void CircuitsWindow::scrollLeft() {
    widget->scroll(-10,0);
    saved = false;
}

void CircuitsWindow::scrollRight() {
    widget->scroll(10,0);
    saved = false;
}

void CircuitsWindow::zoomIn() {
    int incPx = 1;
    for(int i = 0; i < widget->componentList.size(); i++){
        int w = widget->componentList[i]->width() + incPx;
        int h = widget->componentList[i]->height() + incPx;
        widget->componentList[i]->resize(w,h);
        if(widget->componentList[i]->getComponentType()==3){
            Variable *v = (Variable*) widget->componentList[i].data();
            v->var->resize(v->var->width() + incPx, v->var->height() + incPx);
        }
    }
    saved = false;
}

void CircuitsWindow::zoomOut() {
    int incPx = 1;
    for(int i = 0; i < widget->componentList.size(); i++){
        int w = widget->componentList[i]->width() + incPx;
        int h = widget->componentList[i]->height() + incPx;
        widget->componentList[i]->resize(w,h);
        if(widget->componentList[i]->getComponentType()==3){
            Variable *v = (Variable*) widget->componentList[i].data();
            v->var->resize(v->var->width() - incPx, v->var->height() + incPx);
        }
    }
    saved = false;
}