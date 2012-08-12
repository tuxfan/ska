/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#ifndef ViewCycle_h
#define ViewCycle_h

#include <QtGui/QtGui>

class viewcycle_t : public QWidget
{
public:

	viewcycle_t(viewpipeline_t * pipeline) : QWidget(pipeline) {
		pipeline_ = pipeline;
	} // viewcycle_t

	QSize sizeHint() const {
		return QSize(pipeline_->cycleAreaWidth(), 0);
	} // sizeHint

protected:

	void paintEvent(QPaintEvent * event) {
		pipeline_->cycleAreaPaintEvent(event);
	} // paintEvent

private:

	viewpipeline_t * pipeline_;

}; // class viewcycle_t

#endif // ViewCycle_h
