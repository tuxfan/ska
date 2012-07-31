#ifndef ViewPipeline_h
#define ViewPipeline_h

#include <QtGui/QPlainTextEdit>
#include <QtCore/QObject>

class viewpipeline_t : public QPlainTextEdit
{
	Q_OBJECT

public:

	viewpipeline_t(QWidget * parent = 0);

	int cycleAreaWidth();
	int instructionAreaWidth();

protected:

	void resizeEvent(QResizeEvent * event);

private slots:

	void updateCycleAreaWidth(int newCount);
	void updateInstructionAreaWidth(int newMax);

	void highlightCurrentLine();

	void updateCycleArea(const QRect &, int);
	void updateInstructionArea(const QRect &, int);

private:

	QWidget * cycleArea_;
	QWidget * instructionArea_;

}; // class viewpipeline_t

#endif // ViewPipeline_h
