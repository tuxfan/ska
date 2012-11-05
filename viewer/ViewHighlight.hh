/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#ifndef ViewHighlight_h
#define ViewHighlight_h

#include <QtGui/QtGui>

template<typename parent_t>
class viewhighlight_T : public QWidget
{
public:

	viewhighlight_T(parent_t * parent) : QWidget(parent) {
		parent_ = parent;
		setAttribute(Qt::WA_TransparentForMouseEvents);
	} // viewhighlight_T

	QSize sizeHint() const {
		return QSize(parent_->highlightAreaWidth(), 0);
	} // sizeHint

protected:

	void paintEvent(QPaintEvent * event) {
		parent_->highlightAreaPaintEvent(event);
	} // paintEvent

private:

	parent_t * parent_;

}; // class viewhighlight_T

#endif // ViewHighlight_h
