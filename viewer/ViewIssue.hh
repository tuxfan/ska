#ifndef ViewIssue_h
#define ViewIssue_h

#include <QtGui/QtGui>

class viewissue_t : public QWidget
{
public:

	viewissue_t(viewpipeline_t * pipeline) : QWidget(pipeline) {
		pipeline_ = pipeline;
	} // viewissue_t

	QSize sizeHint() const {
		return QSize(pipeline_->issueAreaWidth(), 0);
	} // sizeHint

protected:

	void paintEvent(QPaintEvent * event) {
		pipeline_->issueAreaPaintEvent(event);
	} // paintEvent

private:

	viewpipeline_t * pipeline_;

}; // class viewissue_t

#endif // ViewIssue_h
