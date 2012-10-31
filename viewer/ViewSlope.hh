#ifndef ViewSlope_hh
#define ViewSlope_hh

#include<QtGui/QFrame>
#include<qwt/qwt_plot_curve.h>
#include<qwt/qwt_scale_map.h>

class viewslope_t : public QFrame
{
public:

	viewslope_t(const QVector<double> & x_points,
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

		xmap_.setScaleInterval(xmin, xmax);
		ymap_.setScaleInterval(ymin, ymax);

		setFrameStyle(QFrame::Box | QFrame::Raised);
		setLineWidth(2);
		setMidLineWidth(3);

		// set plot style
		plot_.setStyle(QwtPlotCurve::Lines);

		plot_.setRawSamples(x_points.data(), y_points.data(), x_points.size());
	} // viewslope_t

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
		ymap_.setPaintInterval(r.top(), r.bottom());

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
