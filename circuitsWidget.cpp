#include <circuitsWidget.h>

// this widget act as a container for components and wires

CircuitsWidget::CircuitsWidget(QWidget *parent) : QWidget(parent) {
	connectingComponent = false;
	componentOnConnection = NULL;
	setMouseTracking(true);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
}

void CircuitsWidget::paintEvent(QPaintEvent*) {
	// drawing all the wires
	QPainter p(this);
	p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	QPen linePen;
	linePen.setWidth(3);
	linePen.setBrush(Qt::red);
	linePen.setCapStyle(Qt::RoundCap);
	p.setPen(linePen);
	for(int it = 0; it < wireList.size(); it++){
		p.drawLine(wireList[it]->getLeftPoint(), wireList[it]->getRightPoint());
	}
}

void CircuitsWidget::drawComponent(Component *c) {
	QPointer<Component> p = c;
	createComponentConnections(p);
	int rx = rand() % (width() - 80);
	int ry = rand() % (height() - 80);
	p->move(rx,ry);
	p->show();
	componentList.append(p);
}

void CircuitsWidget::wheelEvent(QWheelEvent *event) {
    int incPx = 1; 
    if(event->pixelDelta().y()>0){
        // zoom in
        for(int i = 0; i < componentList.size(); i++){
            int w = componentList[i]->width() + incPx;
            int h = componentList[i]->height() + incPx;
            componentList[i]->resize(w,h);
            if(componentList[i]->getComponentType()==3){
                Variable *v = (Variable*) componentList[i].data();
                v->var->resize(v->var->width() + incPx, v->var->height() + incPx);
            }
        }
    } else {
        // zoom out
        for(int i = 0; i < componentList.size(); i++){
            int w = componentList[i]->width() - incPx;
            int h = componentList[i]->height() - incPx;
            componentList[i]->resize(w,h);
            if(componentList[i]->getComponentType()==3){
                Variable *v = (Variable*) componentList[i].data();
                v->var->resize(v->var->width() + incPx, v->var->height() + incPx);
            }
        }
    }
    repaint();
}

void CircuitsWidget::mouseReleaseEvent(QMouseEvent *event) {
	setFocus();
	if(connectingComponent){
		// resetting cursor
		QCursor c;
		c.setShape(Qt::ArrowCursor);
		setCursor(c);
		// checking click on components
		QPoint click = event->pos();
		for(int it = 0; it < componentList.size(); it++){
			QRect r = componentList[it]->rect();
			r.setX(componentList[it]->x());
			r.setY(componentList[it]->y());
			r.setWidth(componentList[it]->width());
			r.setHeight(componentList[it]->height());
			bool c1 = r.contains(click); // the mouserelease is on a component
			bool c2 = !(componentOnConnection == componentList[it]); // not the same component
			bool c3 = (componentOnConnection->outComponentList.indexOf(componentList[it])==-1); // not already connected
			bool c4 = componentOnConnection->getComponentType() != 2; // the left component is not a collector
			bool c5 = componentList[it]->getComponentType() != 3; // the right component is not a variable
            // if the right component is a collector it must be unconnected
            bool c6;
            if(componentList[it]->getComponentType() == 2)
                c6 = componentList[it]->inComponentList.isEmpty();
            else
                c6 = true;  
            // if the right component is a NOT port it must be unconnected
            bool c7;
            if(componentList[it]->getComponentType() == 6)
                c7 = componentList[it]->inComponentList.isEmpty();
            else
                c7 = true;
            // if the right component is a divider, it must be unconnected
            bool c8;
            if(componentList[it]->getComponentType() == 7)
                c8 = componentList[it]->inComponentList.isEmpty();
            else
                c8 = true;
			if(c1 && c2 && c3 && c4 && c5 && c6 && c7 && c8){
				// creating and set new wire
				Wire *w = new Wire(componentOnConnection, componentList[it]);
				wireList.append(w);
				// setting connections
				componentOnConnection->outComponentList.append(componentList[it]);
				componentList[it]->inComponentList.append(componentOnConnection);
				// value injection
				int ctype = componentList[it]->getComponentType();
				switch(ctype){
                    case(8):{
                        // muler
                        int tmp = componentList[it]->getValue();
                        componentList[it]->setValue(tmp * componentOnConnection->getValue());
                        break;
                    }
                    case(7):{
                        // diver
                        Diver* d = (Diver*) componentList[it].data(); // pay attenction
                        int r = componentOnConnection->getValue() / d->getDivider();
                        componentList[it]->setValue(r);
                        break;
                    }
                    case(6):{
                        //noter
                        if(componentOnConnection->getValue() != 0)
                            componentList[it]->setValue(0);
                        else
                            componentList[it]->setValue(1);
                        break;
                    }
                    case(5):{
                        //orer
                        bool tmp = componentList[it]->getValue();
                        componentList[it]->setValue(tmp || componentOnConnection->getValue());
                        break;
                    }
                    case(4):{
                        //ander
                        bool tmp = componentList[it]->getValue();
                        componentList[it]->setValue(tmp && componentOnConnection->getValue());
                        break;
                    }
					case(3):{
						//variabile
						printf("WARNING: This component shouldn't have any incoming component\n");
						break;
					}
					case(2):{
						// collector
						componentList[it]->setValue(componentOnConnection->getValue());
						break;
					}
					case(1):{
						//adder
						int tmp = componentList[it]->getValue();
						componentList[it]->setValue(tmp + componentOnConnection->getValue());
						break;
					}
					default:{
						printf("FATAL ERROR: invalid component type\n");
						exit(1);
					}
				}
				repaint();
			}
		}
		connectingComponent = false;
		componentOnConnection = NULL;
	}
}

void CircuitsWidget::createComponentConnections(Component *c) {
	connect(c, SIGNAL(deleteSig(int)), this, SLOT(deleteComponentSlot(int)));
	connect(c, SIGNAL(wireSig(int, Component*)), this, SLOT(wireSlot(int, Component*)));
	connect(c, SIGNAL(valueChangedSig(int, Component*, int, int)), this, SLOT(valueChangedSlot(int, Component*, int, int)));
	connect(c, SIGNAL(deleteWireSig(int, Component*)), this, SLOT(deleteWireSlot(int, Component*)));
    connect(c, SIGNAL(dividerSig(int, Component*)), this, SLOT(dividerSlot(int, Component*)));
}

// Slots

void CircuitsWidget::dividerSlot(int, Component* c) {
    bool ok;
    // the divider must be not zero!
    int ndiv = QInputDialog::getInt(this, tr("Set new divider"), tr("Divider:"), 1,  1, 2147483647, 1, &ok);
    if (ok){
        Diver* d =  (Diver*) c; // pay attenction
        int oldiv = d->getDivider();
        int oldval = d->getValue();
        int r = (oldiv * oldval) / ndiv;
        d->setDivider(ndiv);
        d->setValue(r);
        repaint();       
    }
}

void CircuitsWidget::deleteComponentSlot(int) {
	for(int it = 0; it < componentList.size(); it++){
		if(componentList[it]->isDead()){
			QPointer<Component> c = componentList.takeAt(it);
			// deleting wire and connections
			deleteWireSlot(100,c);
			// hiding component
			c->hide();
		}
	}
	repaint();
}

void CircuitsWidget::deleteWireSlot(int, Component* c) {
	// deleting wires
	for(int i = 0; i < wireList.size(); i++)
		if((wireList[i]->getLeftComponent() == c) || (wireList[i]->getRightComponent() == c))
			wireList[i]->setDeleteFlag();
	QMutableListIterator< QPointer<Wire> > iterator(wireList);
	while(iterator.hasNext())
		if(iterator.next()->getDeleteFlag())
			iterator.remove();
	// deleting in/out connections!!!
	for(int i =  0; i < c->inComponentList.size(); i++){
		int idx = c->inComponentList[i]->outComponentList.indexOf(c);
		c->inComponentList[i]->outComponentList.removeAt(idx);
	}
	c->inComponentList.clear();
	for(int i = 0; i < c->outComponentList.size(); i++){
		int ctype = c->outComponentList[i]->getComponentType();
		switch(ctype){
            case(8):{
                // muler
                c->outComponentList[i]->setValue(c->outComponentList[i]->getValue() / c->getValue());
                break;
            }
            case(7):{
                // diver
                Diver* d = (Diver*) c->outComponentList[i].data(); // pay attenction
                d->setDivider(1);
                c->outComponentList[i]->setValue(0);
                break;
            }
            case(6):{
                // noter
                c->outComponentList[i]->setValue(0);
                break;
            }
            case(5):{
                // orer
                int s = c->outComponentList[i]->inComponentList.size();
                int tmp = 0;
                for(int z = 0; z < s; z++){
                    if(c->outComponentList[i]->inComponentList[z] != c)
                        tmp = tmp || c->outComponentList[i]->inComponentList[z]->getValue();
                }
                if(tmp == 0)
                    c->outComponentList[i]->setValue(0);
                else
                    c->outComponentList[i]->setValue(1);
                break;
            }
            case(4):{
                // ander
                int s = c->outComponentList[i]->inComponentList.size();
                int tmp = 1;
                for(int z = 0; z < s; z++){
                    if(c->outComponentList[i]->inComponentList[z] != c)
                        tmp = tmp && c->outComponentList[i]->inComponentList[z]->getValue();
                }
                if(tmp == 0)
                    c->outComponentList[i]->setValue(0);
                else
                    c->outComponentList[i]->setValue(1);
                break;
            }
			case(3):{
				// variable
				printf("WARNING: This component shouldn't have any incoming component\n");
				break;
			}
			case(2):{
				// collector
				c->outComponentList[i]->setValue(0);
				break;
			}
			case(1):{
				// adder
				c->outComponentList[i]->setValue(c->outComponentList[i]->getValue() - c->getValue());
				break;
			}
			default:{
				printf("FATAL ERROR: invalid component type\n");
				exit(1);
			}
		}
		int idx = c->outComponentList[i]->inComponentList.indexOf(c);
		c->outComponentList[i]->inComponentList.removeAt(idx);
	}
	c->outComponentList.clear();
	// resetting component
	int ctype = c->getComponentType();
	switch(ctype){
        case(8):c->setValue(1);break;//muler
        case(7):{
            // diver
            Diver* d = (Diver*) c; // pay attenction
            d->setDivider(1);
            c->setValue(0);
            break;
        }
        case(6):c->setValue(0);break;//noter
        case(5):c->setValue(0);break;//orer
        case(4):c->setValue(1);break;//ander
		case(3):break;//variable
		case(2):c->setValue(0);break;//collector
		case(1):c->setValue(0);break;//adder
		default:{
			printf("FATAL ERROR: invalid component type\n");
			exit(1);
		}
	}
	repaint();
}

void CircuitsWidget::wireSlot(int, Component *c) {
	QCursor cr;
	cr.setShape(Qt::CrossCursor);
	setCursor(cr);
	connectingComponent = true;
	componentOnConnection = c;
}

void CircuitsWidget::valueChangedSlot(int, Component *c, int oldValue, int newValue) {
	// refreshing value on outcoming components
	for(int i = 0; i < c->outComponentList.size(); i++){
		int ctype = c->outComponentList[i]->getComponentType();
        int olval = c->outComponentList[i]->getValue();
		switch (ctype) {
            case(8):{
                // muler
                int tmp = c->outComponentList[i]->getValue();
                if(oldValue != 0)
				    tmp = tmp / oldValue;
                else{
                    // recalculating correct value
                    int s = c->outComponentList[i]->inComponentList.size();
                    int r = 1;
                    for(int z = 0; z < s; z ++){
                        if(c->outComponentList[i]->inComponentList[z] != c)
                            r = r * c->outComponentList[i]->inComponentList[z]->getValue();
                    }
                    tmp = r;
                }
				c->outComponentList[i]->setValue(tmp * newValue);
                break;
            }
            case(7):{
                //diver
                Diver *d = (Diver*) c->outComponentList[i].data(); // pay attenction
                int r = newValue / d->getDivider();
                c->outComponentList[i]->setValue(r);
                break;
            }
            case(6):{
                // noter
                if(newValue == 0)
                    c->outComponentList[i]->setValue(1);
                else
                    c->outComponentList[i]->setValue(0);
                break;
            }
            case(5):{
                // orer
                if(newValue == 1)
                    c->outComponentList[i]->setValue(1);
                else
                    if(olval == 0)
                        c->outComponentList[i]->setValue(0);
                    else{
                        // recalculating the correct value
                        int s = c->outComponentList[i]->inComponentList.size();
                        int tmp = 0;
                        for(int z = 0; z < s; z++){
                            tmp = tmp || c->outComponentList[i]->inComponentList[z]->getValue();
                        }
                        if(tmp == 0)
                            c->outComponentList[i]->setValue(0);
                        else
                            c->outComponentList[i]->setValue(1);
                    }
                break;
            }
            case(4):{
                // ander
                if(newValue == 0)
                    c->outComponentList[i]->setValue(0);
                else
                    if(olval != 0)
                        c->outComponentList[i]->setValue(1);
                    else{
                        // recalculating the correct value
                        int s = c->outComponentList[i]->inComponentList.size();
                        int tmp = 1;
                        for(int z = 0; z < s; z++){
                            tmp = tmp && c->outComponentList[i]->inComponentList[z]->getValue();
                        }
                        if(tmp == 0)
                            c->outComponentList[i]->setValue(0);
                        else
                            c->outComponentList[i]->setValue(1);
                    }
                break;
            }
			case(3):{
				// variable
				printf("WARNING: This component shouldn't have any incoming component\n");
				break;
			}
			case(2):{
				// collector
				c->outComponentList[i]->setValue(newValue);
				break;
			}
			case(1):{
				// adder
				int tmp = c->outComponentList[i]->getValue();
				tmp -= oldValue;
				c->outComponentList[i]->setValue(tmp + newValue);
				break;
			}
			default:{
				printf("FATAL ERROR: invalid component type\n");
				exit(1);
			}
		}
	}
	c->repaint();
}

void CircuitsWidget::serialize(QString fileName) {
	// Either this if you use UTF-8 anywhere
	std::string fn = fileName.toUtf8().constData();
	// n.b. Use this to convert QString in std::string on Windows!
	//std::string fn = qs.toLocal8Bit().constData();
	ofstream file;
	file.open(fn.c_str());
    // listing all the components
	for(int i = 0; i < componentList.size(); i++){ // id & component info
		file << "#" << std::endl;
		file << (componentList[i]->getComponentType()) << std::endl;
		file << (componentList[i]->getId()) << std::endl;
        file << (componentList[i]->x()) << std::endl;
		file << (componentList[i]->y()) << std::endl;
		file << (componentList[i]->width()) << std::endl;
		file << (componentList[i]->height()) << std::endl;
		file << (componentList[i]->getValue()) << std::endl;
	}
	for(int i = 0; i < componentList.size(); i++){
		file << "@" << std::endl;
		file << (componentList[i]->getId()) << std::endl; 
		file << componentList[i]->inComponentList.size() << std::endl;
        file << componentList[i]->outComponentList.size() << std::endl;
        // here begins the list of incoming components
		for(int j = 0; j < componentList[i]->inComponentList.size(); j++)
            file << (componentList[i]->inComponentList[j]->getId()) << std::endl;
		// here begins the list of outcoming components
        for(int j = 0; j < componentList[i]->outComponentList.size(); j++)
            file << (componentList[i]->outComponentList[j]->getId()) << std::endl;
	}
	file.close();
}

void CircuitsWidget::deserialize(QString fileName) {
    std::string fn = fileName.toUtf8().constData();
    // n.b. Use this to convert QString in std::string on Windows!
	//std::string fn = qs.toLocal8Bit().constData();
	string line;
    ifstream file;
	file.open(fn.c_str());
    // test
    while(getline(file,line)){
        if(line.compare("#") == 0){
            getline(file,line); // reading type
            int t = atoi(line.c_str());
            getline(file,line); // reading id
            int id = atoi(line.c_str());
            getline(file,line); // reading x position
            int x = atoi(line.c_str());
            getline(file,line); // reading y position
            int y = atoi(line.c_str());
            getline(file,line); // reading width
            int w = atoi(line.c_str());
            getline(file,line); // reading height
            int h = atoi(line.c_str());
            getline(file,line); // reading value
            string sval = line;
            int val = atoi(line.c_str());
            switch(t) {
                case(8):{
                    //muler
                    Muler *m = new Muler(this);
                    drawComponent(m);
                    m->setId(id);
                    m->move(x,y);
                    m->resize(w,h);
                    m->setValue(val);
                    break;
                }
                case(7):{
                    //diver
                    Diver *d = new Diver(this);
                    drawComponent(d);
                    d->setId(id);
                    d->move(x,y);
                    d->resize(w,h);
                    d->setValue(val);
                    // NOTE: this is a particular case, please see under in "restoring connections" 
                    break;
                }
                case(6):{
                    //noter
                    Noter *n = new Noter(this);
                    drawComponent(n);
                    n->setId(id);
                    n->move(x,y);
                    n->resize(w,h);
                    n->setValue(val);
                    break;
                }
                case(5):{
                    //orer
                    Orer *o = new Orer(this);
                    drawComponent(o);
                    o->setId(id);
                    o->move(x,y);
                    o->resize(w,h);
                    o->setValue(val);
                    break;
                }
                case(4):{
                    // ander
                    Ander *a = new Ander(this);
                    drawComponent(a);
                    a->setId(id);
                    a->move(x,y);
                    a->resize(w,h);
                    a->setValue(val);
                    break;
                }
                case(3):{
                    // variable
                    Variable *v = new Variable(this);
	                drawComponent(v);
                    v->setId(id);
                    v->move(x,y);
                    v->resize(w,h);
                    v->setValue(val);
                    v->var->setPlaceholderText(sval.c_str());
                    break;
                }
                case(2):{
                    // collector
                    Collector *c = new Collector(this);
                    drawComponent(c);
                    c->setId(id);
                    c->move(x,y);
                    c->resize(w,h);
                    c->setValue(val);
                    break;
                }
                case(1):{
                    // adder
                    Adder *a = new Adder(this);
                    drawComponent(a);
                    a->setId(id);
                    a->move(x,y);
                    a->resize(w,h);
                    a->setValue(val);
                    break;
                }
                default:{
                    printf("FATAL ERROR: bad format\n");
				    exit(1);
                }
            }
        }
        if(line.compare("@") == 0){
            getline(file,line); // reading id
            int id = atoi(line.c_str());
            getline(file,line); // reading incoming components number
            int nin = atoi(line.c_str());
            getline(file,line); // reading outcoming components number
            int nou = atoi(line.c_str());
            // restoring connections
            int i = 0;
            while(componentList[i]->getId() != id)
                i++;
            for(int j = 0; j < nin; j++){
                getline(file,line);
                int z = atoi(line.c_str());
                int q = 0;
                while(componentList[q]->getId() != z)
                    q++;
                componentList[i]->inComponentList.append(componentList[q]);
                if(componentList[i]->getComponentType()==7){
                    // restoring correct divider if the component is a diver
                    Diver* d = (Diver*) componentList[i].data(); // pay attenction
                    d->setDivider(componentList[q]->getValue() / componentList[i]->getValue());
                }
                Wire *w = new Wire(componentList[i], componentList[q]);
				wireList.append(w);
            }
            for(int j = 0; j < nou; j++){
                getline(file,line);
                int z = atoi(line.c_str());
                int q = 0;
                while(componentList[q]->getId() != z)
                    q++;
                componentList[i]->outComponentList.append(componentList[q]);
                Wire *w = new Wire(componentList[i], componentList[q]);
				wireList.append(w);
            }
        }
    }
    file.close();
    repaint();
    setFocus();
}

void CircuitsWidget::clearAll() {
    // setting final position
    finalposY = height();
    // starting positions
    startposY = (int*) malloc(componentList.size() * sizeof(int));
    // interpoled positions
    interposY = (int*) malloc(componentList.size() * sizeof(int));
    // setting starting point of the animation
    for(int i = 0; i < componentList.size(); i++)
        startposY[i] = componentList[i]->y();
    timeElapsed = 0;
    timer->start(20);
    repaint();
}

void CircuitsWidget::timerTick() {
    int duration = 1500;
    timeElapsed+=20;
    for(int i = 0; i < componentList.size(); i++){
        interposY[i] = quarticEaseOut(timeElapsed, startposY[i], finalposY, duration);
        componentList[i]->move(componentList[i]->x(), interposY[i]);
    }
    repaint();
    if(timeElapsed >= duration){
        timer->stop();
        free(startposY);
        free(interposY);
        for(int i = 0; i < componentList.size(); i++)
            componentList[i]->hide();
        componentList.clear();
        wireList.clear();
    }
}

float CircuitsWidget::quarticEaseOut(float t, float b, float c, float d){
    // t: elapsed time
    // b: initial position
    // c: final position
    // d: animation duration
    t /= d;
	return c*t*t*t*t + b;
}