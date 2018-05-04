#ifndef ONX_Q3_HBOX_H
#define ONX_Q3_HBOX_H

//G.Barrand #include <Qt3Support/q3frame.h>
#include "q3_frame.h" //G.Barrand

QT_BEGIN_HEADER

//QT_BEGIN_NAMESPACE

//QT_MODULE(Qt3SupportLight)

class QBoxLayout;

#define Q3HBox OnX_Q3HBox //G.Barrand

class 
//Q_COMPAT_EXPORT
Q3HBox : public Q3Frame
{
    Q_OBJECT
public:
    Q3HBox(QWidget* parent=0, const char* name=0, Qt::WindowFlags f=0);

    void setSpacing(int);
    bool setStretchFactor(QWidget*, int stretch);
    QSize sizeHint() const;

protected:
    Q3HBox(bool horizontal, QWidget* parent, const char* name, Qt::WindowFlags f = 0);
    void frameChanged();

private:
    Q_DISABLE_COPY(Q3HBox)
};

//QT_END_NAMESPACE

QT_END_HEADER

#endif // Q3HBOX_H
