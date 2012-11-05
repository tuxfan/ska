/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#include <iostream>

#include <ViewPipeline.hh>
#include <ViewCycle.hh>
#include <ViewIssue.hh>
#include <ViewInstruction.hh>
#include <ViewHighlight.hh>

typedef viewhighlight_T<viewpipeline_t> viewhighlight_t;

/*----------------------------------------------------------------------------*
 * viewpipeline_t constructor.
 *----------------------------------------------------------------------------*/

viewpipeline_t::viewpipeline_t(QWidget * parent)
{
	// set properties
	setReadOnly(true);
	setLineWrapMode(QPlainTextEdit::NoWrap);
	setFont(QFont("Courier", 12));
	setMouseTracking(true);

	// create widget areas
	cycleArea_ = new viewcycle_t(this);
	issueArea_ = new viewissue_t(this);
	instructionArea_ = new viewinstruction_t(this);
	highlightArea_ = new viewhighlight_t(this);

	// set-up callbacks
	connect(this, SIGNAL(blockCountChanged(int)),
		this, SLOT(updateWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)),
		this, SLOT(updateCycleArea(QRect,int)));
	connect(this, SIGNAL(updateRequest(QRect,int)),
		this, SLOT(updateIssueArea(QRect,int)));
	connect(this, SIGNAL(updateRequest(QRect,int)),
		this, SLOT(updateInstructionArea(QRect,int)));
	connect(this, SIGNAL(updateRequest(QRect,int)),
		this, SLOT(updateHighlightArea(QRect,int)));

	updateWidth(0);
} // viewpipeline_t::viewpipeline_t

/*----------------------------------------------------------------------------*
 * Load input data.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::load(const QStringList & cycles,
	const QStringList & issues, const QString & pipelines,
	const QStringList & instructions)
{
	cycles_ = cycles;
	issues_ = issues;
	instructions_ = instructions;
	setPlainText(pipelines);
} // viewpipeline_t::setText

/*----------------------------------------------------------------------------*
 * Paint cycle area.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::cycleAreaPaintEvent(QPaintEvent * event)
{
	if(cycles_.size() > 0) {
		QPainter painter(cycleArea_);
		painter.fillRect(event->rect(), Qt::lightGray);

		QTextBlock block = firstVisibleBlock();
		int blockNumber = block.blockNumber();
		int top =
			(int)blockBoundingGeometry(block).translated(contentOffset()).top();
		int bottom = top + (int)blockBoundingRect(block).height();

		while(blockNumber < cycles_.size() &&
			block.isValid() && top <= event->rect().bottom()) {
			if(block.isVisible() && bottom >= event->rect().top()) {
				painter.setPen(Qt::black);
				painter.drawText(4, top, cycleArea_->width(),
					fontMetrics().height(), Qt::AlignRight, cycles_[blockNumber]);
			}

			block = block.next();
			top = bottom;
			bottom = top + (int) blockBoundingRect(block).height();
			++blockNumber;
		} // while
	} // if
} // viewpipeline_t::cycleAreaPaintEvent

/*----------------------------------------------------------------------------*
 * Paint issue area.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::issueAreaPaintEvent(QPaintEvent * event)
{
	if(issues_.size() > 0) {
		QPainter painter(issueArea_);
		painter.fillRect(event->rect(), QColor(110,240,230));

		QTextBlock block = firstVisibleBlock();
		int blockNumber = block.blockNumber();
		int top =
			(int)blockBoundingGeometry(block).translated(contentOffset()).top();
		int bottom = top + (int)blockBoundingRect(block).height();

		while(blockNumber < issues_.size() &&
			block.isValid() && top <= event->rect().bottom()) {
			if(block.isVisible() && bottom >= event->rect().top()) {
				painter.setPen(Qt::black);
				painter.drawText(0, top, issueArea_->width(),
					fontMetrics().height(), Qt::AlignLeft, issues_[blockNumber]);
			}

			block = block.next();
			top = bottom;
			bottom = top + (int) blockBoundingRect(block).height();
			++blockNumber;
		} // while
	} // if
} // viewpipeline_t::issueAreaPaintEvent

/*----------------------------------------------------------------------------*
 * Paint instruction area.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::instructionAreaPaintEvent(QPaintEvent * event)
{
	if(instructions_.size() > 0) {
		QPainter painter(instructionArea_);
		painter.fillRect(event->rect(), QColor(255,255,155));

		QTextBlock block = firstVisibleBlock();
		int blockNumber = block.blockNumber();
		int top =
			(int)blockBoundingGeometry(block).translated(contentOffset()).top();
		int bottom = top + (int)blockBoundingRect(block).height();

		while(blockNumber < instructions_.size() &&
			block.isValid() && top <= event->rect().bottom()) {
			if(block.isVisible() && bottom >= event->rect().top()) {
				painter.setPen(Qt::black);
				painter.drawText(0, top, instructionArea_->width(),
					fontMetrics().height(), Qt::AlignLeft,
					instructions_[blockNumber]);
			}

			block = block.next();
			top = bottom;
			bottom = top + (int) blockBoundingRect(block).height();
			++blockNumber;
		} // while
	} // if
} // viewpipeline_t::instructionAreaPaintEvent

void viewpipeline_t::highlightAreaPaintEvent(QPaintEvent * event)
{
	if(instructions_.size() > 0) {
		QPainter painter(highlightArea_);
		painter.fillRect(event->rect(), QColor(0,0,255, 63));
	} // if
} // viewpipeline_t::paintEvent

/*----------------------------------------------------------------------------*
 * Get width of cycle area.
 *----------------------------------------------------------------------------*/

int viewpipeline_t::cycleAreaWidth()
{
	int digits = 1;

	if(cycles_.size() > 0) {
		digits = cycles_[0].size();
	}

	int space = fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
} // viewpipeline_t::cycleAreaWidth

/*----------------------------------------------------------------------------*
 * Get width of issue area.
 *----------------------------------------------------------------------------*/

int viewpipeline_t::issueAreaWidth()
{
	int digits = 2;

	if(issues_.size() > 0) {
		digits = issues_[0].size();
	}

	int space = fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
} // viewpipeline_t::issueAreaWidth

/*----------------------------------------------------------------------------*
 * Get width of instruction area.
 *----------------------------------------------------------------------------*/

int viewpipeline_t::instructionAreaWidth()
{
	int digits = 1;

	if(instructions_.size() > 0) {
		for(int i=0; i<instructions_.size(); ++i) {
			digits = std::max<int>(digits, instructions_[i].size());
		} // for
	}
	
	digits = std::min<int>(digits, 40);

	int space = 5 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
} // viewpipeline_t::instructionAreaWidth

/*----------------------------------------------------------------------------*
 * Get width of highlight area.
 *----------------------------------------------------------------------------*/

int viewpipeline_t::highlightAreaWidth()
{
	QRect cr = contentsRect();
	return cr.width();
} // viewpipeline_t::highlightAreaWidth

/*----------------------------------------------------------------------------*
 * Override resize event.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::resizeEvent(QResizeEvent * event)
{
	QPlainTextEdit::resizeEvent(event);

	QRect cr = contentsRect();
	cycleArea_->setGeometry(QRect(cr.left(), cr.top(),
		cycleAreaWidth(), cr.height()));
	issueArea_->setGeometry(QRect(cr.left() + cycleAreaWidth(), cr.top(),
		issueAreaWidth(), cr.height()));
	instructionArea_->setGeometry(
		QRect(cr.right() - verticalScrollBar()->sizeHint().width() -
		instructionAreaWidth(), cr.top(), instructionAreaWidth(), cr.height()));


	QTextBlock block = firstVisibleBlock();
	highlight_.setHeight(3 * (int)blockBoundingRect(block).height());
	highlight_.setWidth(cr.width());
	highlightArea_->setGeometry(highlight_);
} // viewpipeline_t::resizeEvent

/*----------------------------------------------------------------------------*
 * Override mouse move event.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::mouseMoveEvent(QMouseEvent * event)
{
	QPlainTextEdit::mouseMoveEvent(event);

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top =
		(int)blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int)blockBoundingRect(block).height();

	blockNumber = blockNumber == 0 ? 1 : blockNumber;
	while(blockNumber < instructions_.size() && block.isValid()) {
		if(blockBoundingGeometry(block).contains(event->x(), event->y())) {
			emit highlightMove(blockNumber);
			highlight_.moveTo(0, top - (int)blockBoundingRect(block).height());
			highlightArea_->setGeometry(highlight_);
			highlightArea_->repaint();
			return;
		} // if

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		++blockNumber;
	} // while
} // viewpipeline_t::mouseMoveEvent

/*----------------------------------------------------------------------------*
 * Update widget width.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::updateWidth(int newBlockCount)
{
	setViewportMargins(cycleAreaWidth() + issueAreaWidth(), 0,
		instructionAreaWidth(), 0);
} // viewpipeline_t::updateWidth

/*----------------------------------------------------------------------------*
 * Update cycle area.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::updateCycleArea(const QRect & rect, int dy)
{
	if(dy) {
		cycleArea_->scroll(0, dy);
	}
	else {
		cycleArea_->update(0, rect.y(), cycleArea_->width(), rect.height());
	} // if

	if(rect.contains(viewport()->rect())) {
		updateWidth(0);
	} // if
} // viewpipeline_t::updateCycleArea

/*----------------------------------------------------------------------------*
 * Update issue area.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::updateIssueArea(const QRect & rect, int dy)
{
	if(dy) {
		issueArea_->scroll(0, dy);
	}
	else {
		issueArea_->update(0, rect.y(), issueArea_->width(), rect.height());
	} // if

	if(rect.contains(viewport()->rect())) {
		updateWidth(0);
	} // if
} // viewpipeline_t::updateIssueArea

/*----------------------------------------------------------------------------*
 * Update instruction area.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::updateInstructionArea(const QRect & rect, int dy)
{
	if(dy) {
		instructionArea_->scroll(0, dy);
	}
	else {
		instructionArea_->update(0, rect.y(), instructionArea_->width(),
			rect.height());
	} // if

	if(rect.contains(viewport()->rect())) {
		updateWidth(0);
	} // if
} // viewpipeline_t::updateInstructionArea

/*----------------------------------------------------------------------------*
 * Update highlight area.
 *----------------------------------------------------------------------------*/

void viewpipeline_t::updateHighlightArea(const QRect & rect, int dy)
{
	if(dy) {
		highlightArea_->scroll(0, dy);
	}
	else {
		highlightArea_->update(0, rect.y(), highlightArea_->width(),
			rect.height());
	} // if

	if(rect.contains(viewport()->rect())) {
		updateWidth(0);
	} // if
} // viewpipeline_t::updateHighlightArea
