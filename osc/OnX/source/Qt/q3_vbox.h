#ifndef ONX_Q3_VBOX_H
#define ONX_Q3_VBOX_H

//G.Barrand #include <Qt3Support/q3hbox.h>
#include "q3_hbox.h" //G.Barrand

QT_BEGIN_HEADER

//QT_BEGIN_NAMESPACE

//QT_MODULE(Qt3SupportLight)

#define Q3VBox OnX_Q3VBox //G.Barrand

class 
//Q_COMPAT_EXPORT
Q3VBox : public Q3HBox
{
    Q_OBJECT
public:
  Q3VBox( QWidget *parent = 0, const char *name = 0, Qt::WindowFlags f = 0)
    :Q3HBox( false, parent, name, f )
    {}

private:
    Q_DISABLE_COPY(Q3VBox)
};

//QT_END_NAMESPACE

QT_END_HEADER

#endif // Q3VBOX_H
