#ifndef ViewHighlight_h
#define ViewHighlight_h

#include <QtGui/QtGui>

class viewhighlight_t : public QWidget
{
public:

	viewhighlight_t(viewpipeline_t * pipeline) : QWidget(pipeline) {
		pipeline_ = pipeline;
		setAttribute(Qt::WA_TransparentForMouseEvents);
	} // viewhighlight_t

	QSize sizeHint() const {
		return QSize(pipeline_->highlightAreaWidth(), 0);
	} // sizeHint

protected:

	void paintEvent(QPaintEvent * event) {
		pipeline_->highlightAreaPaintEvent(event);
	} // paintEvent

private:

	viewpipeline_t * pipeline_;

}; // class viewhighlight_t

#endif // ViewHighlight_h
