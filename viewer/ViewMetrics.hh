#ifndef ViewMetrics_hh
#define ViewMetrics_hh

#include <QtGui/QPlainTextEdit>

class viewmetrics_t : public QPlainTextEdit
{
public:

	viewmetrics_t(QWidget * parent = nullptr) {
		// set properties
		setReadOnly(true);
		setLineWrapMode(QPlainTextEdit::NoWrap);
		setFont(QFont("Courier", 12));
	} // viewmetrics_t

	void load(const QString & dataset, const QString & metrics) {
		QString title = "Statistics view (" + dataset + ")";
		setWindowTitle(title);

		setPlainText(metrics);
	} // load

private:

	QStringList metrics_;

}; // class viewmetrics_t

#endif // ViewMetrics_hh
