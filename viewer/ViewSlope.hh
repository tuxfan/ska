/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#ifndef ViewSlope_hh
#define ViewSlope_hh

#include<QtGui/QFrame>
#include<qwt/qwt_plot_curve.h>
#include<qwt/qwt_scale_map.h>

/*----------------------------------------------------------------------------*
 * viewslope_t class
 *----------------------------------------------------------------------------*/

class viewslope_t : public QFrame
{
public:

	viewslope_t(QWidget * parent = nullptr) {
		setFrameStyle(QFrame::Box);

		// set plot style
		plot_.setStyle(QwtPlotCurve::Lines);
		plot_.setPen(QPen(Qt::red, 1));

		// set initial size
		resize(300, 300);
	} // viewslope_t

	void load(const QVector<double> & x_points,
		const QVector<double> & y_points) {

		double xmin(std::numeric_limits<double>::max());
		double xmax(std::numeric_limits<double>::min());
		double ymin(std::numeric_limits<double>::max());
		double ymax(std::numeric_limits<double>::min());

		for(signed i(0); i<x_points.size(); ++i) {
			xmin = std::min(xmin, x_points[i]);
			xmax = std::max(xmax, x_points[i]);
		} // for

		for(signed i(0); i<y_points.size(); ++i) {
			ymin = std::min(ymin, y_points[i]);
			ymax = std::max(ymax, y_points[i]);
		} // for

		xmap_.setScaleInterval(xmin-1, xmax+1);
		ymap_.setScaleInterval(ymin-1, ymax+1);

		plot_.setRawSamples(x_points.data(), y_points.data(), x_points.size());
	} // load

protected:

	virtual void paintEvent(QPaintEvent * event) {
		QFrame::paintEvent(event);

		QPainter painter(this);
		painter.setClipRect(contentsRect());
		drawContents(&painter);
	} // paintEvent

	void drawContents(QPainter * painter) {
		QRect r = contentsRect();

		xmap_.setPaintInterval(r.left(), r.right());
		ymap_.setPaintInterval(r.bottom(), r.top());

		painter->setRenderHint(QPainter::Antialiasing,
			plot_.testRenderHint(QwtPlotItem::RenderAntialiased));
		plot_.draw(painter, xmap_, ymap_, r);	
	} // drawContents

private:

	QwtPlotCurve plot_;	
	QwtScaleMap xmap_;
	QwtScaleMap ymap_;

}; // class viewslope_t

#endif // ViewSlope_hh
