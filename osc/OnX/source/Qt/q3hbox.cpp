/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt3Support module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "q3_hbox.h" //G.Barrand

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qapplication.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qapplication.h>
#endif

//QT_BEGIN_NAMESPACE

/*!
    \class Q3HBox
    \brief The Q3HBox widget provides horizontal geometry management
    for its child widgets.

    \compat

    All the horizontal box's child widgets will be placed alongside
    each other and sized according to their sizeHint()s.

    Use setMargin() to add space around the edges, and use
    setSpacing() to add space between the widgets. Use
    setStretchFactor() if you want the widgets to be different sizes
    in proportion to one another. (See \link layout.html
    Layouts\endlink for more information on stretch factors.)

    \img qhbox-m.png Q3HBox

    \sa QHBoxLayout Q3VBox Q3Grid
*/


/*!
    Constructs an hbox widget with parent \a parent, called \a name.
    The parent, name and widget flags, \a f, are passed to the Q3Frame
    constructor.
*/
Q3HBox::Q3HBox(QWidget *parent, const char *name, Qt::WindowFlags f)
    :Q3Frame(parent, name, f)
{
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    (new QHBoxLayout(this, frameWidth(), frameWidth(), name))->setAutoAdd(true);
#else
    QHBoxLayout* lay = new QHBoxLayout(this);
    int margin = frameWidth();
    int spacing = frameWidth();
    lay->setMargin(margin);
    lay->setSpacing(spacing<0 ? margin : spacing);
#if QT_VERSION < 0x050000
    lay->setObjectName(QString::fromAscii(name));
#else
    lay->setObjectName(QString::fromLatin1(name));
#endif
    //FIXME lay->setAutoAdd(true);
#endif
}


/*!
    Constructs a horizontal hbox if \a horizontal is TRUE, otherwise
    constructs a vertical hbox (also known as a vbox).

    This constructor is provided for the QVBox class. You should never
    need to use it directly.

    The \a parent, \a name and widget flags, \a f, are passed to the
    Q3Frame constructor.
*/

Q3HBox::Q3HBox(bool horizontal, QWidget *parent , const char *name, Qt::WindowFlags f)
    :Q3Frame(parent, name, f)
{
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    (new QBoxLayout(this, horizontal ? QBoxLayout::LeftToRight : QBoxLayout::Down,frameWidth(), frameWidth(), name))->setAutoAdd(true);
#else
    QBoxLayout* lay = 
      new QBoxLayout(horizontal?QBoxLayout::LeftToRight:QBoxLayout::Down,this);
    int margin = frameWidth();
    int spacing = frameWidth();
    lay->setMargin(margin);
    lay->setSpacing(spacing<0 ? margin : spacing);
#if QT_VERSION < 0x050000
    lay->setObjectName(QString::fromAscii(name));
#else
    lay->setObjectName(QString::fromLatin1(name));
#endif
    //FIXME lay->setAutoAdd(true);
#endif
}

/*!\reimp
 */
void Q3HBox::frameChanged()
{
    if (layout())
        layout()->setMargin(frameWidth());
}


/*!
    Sets the spacing between the child widgets to \a space.
*/

void Q3HBox::setSpacing(int space)
{
    if (layout())
        layout()->setSpacing(space);
}


/*!
  \reimp
*/

QSize Q3HBox::sizeHint() const
{
    QWidget *mThis = (QWidget*)this;
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    QApplication::sendPostedEvents(mThis, QEvent::ChildInserted);
#else
    QApplication::sendPostedEvents(mThis, QEvent::ChildAdded);
#endif
    return Q3Frame::sizeHint();
}

/*!
    Sets the stretch factor of widget \a w to \a stretch. Returns true if
    \a w is found. Otherwise returns false.

    \sa QBoxLayout::setStretchFactor() \link layout.html Layouts\endlink
*/
bool Q3HBox::setStretchFactor(QWidget* w, int stretch)
{
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    QApplication::sendPostedEvents(this, QEvent::ChildInserted);
#else
    QApplication::sendPostedEvents(this, QEvent::ChildAdded);
#endif
    if (QBoxLayout *lay = qobject_cast<QBoxLayout *>(layout()))
        return lay->setStretchFactor(w, stretch);
    return false;
}

//QT_END_NAMESPACE
