/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#ifndef ViewPipeline_h
#define ViewPipeline_h

#include <QtGui/QPlainTextEdit>
#include <QtCore/QObject>

class viewpipeline_t : public QPlainTextEdit
{
	Q_OBJECT

public:

	viewpipeline_t(QWidget * parent = nullptr);

	void load(const QStringList & cycles, const QStringList & issues,
		const QString & pipelines, const QStringList & instructions);

	void cycleAreaPaintEvent(QPaintEvent * event);
	void issueAreaPaintEvent(QPaintEvent * event);
	void instructionAreaPaintEvent(QPaintEvent * event);
	void highlightAreaPaintEvent(QPaintEvent * event);

	int cycleAreaWidth();
	int issueAreaWidth();
	int instructionAreaWidth();
	int highlightAreaWidth();

signals:
	
	void highlightMove(int y);

protected:

	void resizeEvent(QResizeEvent * event);
	void mouseMoveEvent(QMouseEvent * event);

private slots:

	void updateWidth(int newBlockCount);

	void updateCycleArea(const QRect & rect, int dy);
	void updateIssueArea(const QRect & rect, int dy);
	void updateInstructionArea(const QRect & rect, int dy);
	void updateHighlightArea(const QRect & rect, int dy);

private:

	QWidget * cycleArea_;
	QWidget * issueArea_;
	QWidget * instructionArea_;
	QWidget * highlightArea_;
	QWidget * slopeArea_;

	QStringList cycles_;
	QStringList issues_;
	QStringList instructions_;

	QRect highlight_;
	unsigned x_, y_;

}; // class viewpipeline_t

#endif // ViewPipeline_h
