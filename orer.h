#pragma once

#include <QPainter>
#include <QPointer>
#include <QFont>

#include <component.h>

class Orer : public Component {
    
    Q_OBJECT
    
    protected:
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
    
    public:
    Orer(QWidget*);
    
};