#include <ViewPipeline.h>
#include <ViewCycle.h>
#include <ViewInstruction.h>

viewpipeline_t::viewpipeline_t(QWidget * parent)
{
	/*-------------------------------------------------------------------------*
	 * set properties
	 *-------------------------------------------------------------------------*/
	setReadOnly(true);
	setLineWrapMode(QPlainTextEdit::NoWrap);
	setFont(QFont("Courier", 12));

	/*-------------------------------------------------------------------------*
	 * create cycle count and instruction areas
	 *-------------------------------------------------------------------------*/

	cycleArea_ = new viewcycle_t(this);
	instructionArea_ = new viewinstruction_t(this);

	connect(this, SIGNAL(blockCountChanged(int)),
		this, SLOT(updateWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)),
		this, SLOT(updateCycleArea(QRect,int)));
	connect(this, SIGNAL(updateRequest(QRect,int)),
		this, SLOT(updateInstructionArea(QRect,int)));

	updateWidth(0);
} // viewpipeline_t::viewpipeline_t

void viewpipeline_t::load(const QStringList & cycles,
	const QString & pipelines, const QStringList & instructions)
{
	cycles_ = cycles;
	instructions_ = instructions;
	setPlainText(pipelines);
} // viewpipeline_t::setText

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
				painter.drawText(0, top, cycleArea_->width(),
					fontMetrics().height(), Qt::AlignRight, cycles_[blockNumber]);
			}

			block = block.next();
			top = bottom;
			bottom = top + (int) blockBoundingRect(block).height();
			++blockNumber;
		} // while
	} // if
} // viewpipeline_t::cycleAreaPaintEvent

void viewpipeline_t::instructionAreaPaintEvent(QPaintEvent * event)
{
	if(instructions_.size() > 0) {
		QPainter painter(instructionArea_);
		painter.fillRect(event->rect(), Qt::yellow);

		QTextBlock block = firstVisibleBlock();
		int blockNumber = block.blockNumber();
		int top =
			(int)blockBoundingGeometry(block).translated(contentOffset()).top();
		int bottom = top + (int)blockBoundingRect(block).height();
		int right =
			(int)blockBoundingGeometry(block).translated(contentOffset()).right();
		
		right++;

		while(blockNumber < instructions_.size() &&
			block.isValid() && top <= event->rect().bottom()) {
			if(block.isVisible() && bottom >= event->rect().top()) {
				painter.setPen(Qt::black);
				painter.drawText(0, top, instructionArea_->width(),
					fontMetrics().height(), Qt::AlignRight,
					instructions_[blockNumber]);
			}

			block = block.next();
			top = bottom;
			bottom = top + (int) blockBoundingRect(block).height();
			++blockNumber;
		} // while
	} // if
} // viewpipeline_t::instructionAreaPaintEvent

int viewpipeline_t::cycleAreaWidth()
{
	int digits = 1;

	if(cycles_.size() > 0) {
		digits = cycles_[0].size();
	}

	int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
} // viewpipeline_t::cycleAreaWidth

int viewpipeline_t::instructionAreaWidth()
{
	int digits = 1;

	if(instructions_.size() > 0) {
		digits = instructions_[0].size();
	}

	int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
} // viewpipeline_t::instructionAreaWidth

void viewpipeline_t::resizeEvent(QResizeEvent * event)
{
	QPlainTextEdit::resizeEvent(event);

	QRect cr = contentsRect();
	cycleArea_->setGeometry(QRect(cr.left(), cr.top(),
		cycleAreaWidth(), cr.height()));
	instructionArea_->setGeometry(QRect(cr.left(), cr.top(),
		instructionAreaWidth(), cr.height()));

} // viewpipeline_t::resizeEvent

void viewpipeline_t::updateWidth(int newBlockCount)
{
	setViewportMargins(cycleAreaWidth(), 0, instructionAreaWidth(), 0);
} // viewpipeline_t::updateWidth

void viewpipeline_t::highlightCurrentLine()
{
} // viewpipeline_t::highlightCurrentLine

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
