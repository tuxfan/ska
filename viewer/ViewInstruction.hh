/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#ifndef ViewInstruction_h
#define ViewInstruction_h

#include <QtGui/QtGui>

class viewinstruction_t : public QWidget
{
public:

	viewinstruction_t(viewpipeline_t * pipeline) : QWidget(pipeline) {
		pipeline_ = pipeline;
	} // viewinstruction_t

	QSize sizeHint() const {
		return QSize(pipeline_->instructionAreaWidth(), 0);
	} // sizeHint

protected:

	void paintEvent(QPaintEvent * event) {
		pipeline_->instructionAreaPaintEvent(event);
	} // paintEvent

private:

	viewpipeline_t * pipeline_;

}; // class viewinstruction_t

#endif // ViewInstruction_h
