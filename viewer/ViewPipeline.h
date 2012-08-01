#ifndef ViewPipeline_h
#define ViewPipeline_h

#include <QtGui/QPlainTextEdit>
#include <QtCore/QObject>

class viewpipeline_t : public QPlainTextEdit
{
	Q_OBJECT

public:

	viewpipeline_t(QWidget * parent = 0);

	void load(const QStringList & cycles, const QString & pipelines,
		const QStringList & instructions);

	void cycleAreaPaintEvent(QPaintEvent * event);
	void instructionAreaPaintEvent(QPaintEvent * event);

	int cycleAreaWidth();
	int instructionAreaWidth();

protected:

	void resizeEvent(QResizeEvent * event);
	void mousePressEvent(QMouseEvent * event);

private slots:

	void updateWidth(int newBlockCount);

	void highlightCurrentLine(const QPoint & pos);

	void updateCycleArea(const QRect & rect, int dy);
	void updateInstructionArea(const QRect & rect, int dy);

private:

	QWidget * cycleArea_;
	QWidget * instructionArea_;

	QStringList cycles_;
	QStringList instructions_;

}; // class viewpipeline_t

#endif // ViewPipeline_h
