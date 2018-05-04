#ifndef ONX_Q3_FRAME_H
#define ONX_Q3_FRAME_H

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qframe.h>
#else
#include <QtWidgets/qframe.h>
#endif

QT_BEGIN_HEADER

//QT_BEGIN_NAMESPACE

//QT_MODULE(Qt3SupportLight)

#define Q3Frame OnX_Q3Frame //G.Barrand

class 
//Q_COMPAT_EXPORT
Q3Frame : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int margin READ margin WRITE setMargin)
    Q_PROPERTY(QRect contentsRect READ contentsRect)

public:
    Q3Frame(QWidget* parent, const char* name = 0, Qt::WindowFlags f = 0);
    ~Q3Frame();
#ifndef qdoc
    bool        lineShapesOk()  const { return true; }
#endif

    int margin() const { return marg; }
    void setMargin(int);

    QRect contentsRect() const;
    int frameWidth() const;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    virtual void frameChanged();
    virtual void drawFrame(QPainter *);
    virtual void drawContents(QPainter *);

private:
    Q_DISABLE_COPY(Q3Frame)

    int marg;
};

//QT_END_NAMESPACE

QT_END_HEADER

#endif // Q3FRAME_H
