/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#ifndef ViewSlope_hh
#define ViewSlope_hh

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

#include "ViewHighlight.h"

/*----------------------------------------------------------------------------*
 * viewslope_t class
 *----------------------------------------------------------------------------*/

class viewslope_t : public QwtPlot
{
	Q_OBJECT

public:

	viewslope_t(QWidget * parent = nullptr);

	void load(const QString & dataset, const QVector<double> & x_points,
		const QVector<double> & y_points);

	void highlightAreaPaintEvent(QPaintEvent * event);

	int highlightAreaWidth();

public slots:

	void updateHighlightArea(const QRect & rect, int dy);
	void moveHighlight(int y);

protected:

	void resizeEvent(QResizeEvent * event);

private:

	void updateOffsets();

	QwtPlotCurve * plot_;	
	QWidget * highlightArea_;
	QRect highlight_;
	int points_;
	int current_;
	double ratio_;

	int left_;
	int right_;
	int top_;
	int bottom_;
	static const int width_ = 2;
	static const int pad_ = 3;

}; // class viewslope_t

#endif // ViewSlope_hh
