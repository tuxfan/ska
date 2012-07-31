#include <ViewPipeline.h>
#include <ViewCycle.h>

viewpipeline_t::viewpipeline_t(QWidget * parent)
{
	/*-------------------------------------------------------------------------*
	 * set properties
	 *-------------------------------------------------------------------------*/
	setReadOnly(true);
	setLineWrapMode(QPlainTextEdit::NoWrap);
	setFont(QFont("Courier", 12));

	/*-------------------------------------------------------------------------*
	 * create cycle count area
	 *-------------------------------------------------------------------------*/

	cycleArea_ = new viewcycle_t(this);

	connect(this, SIGNAL(blockCountChanged(int)),
		this, SLOT(updateCycleAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)),
		this, SLOT(updateCycleArea(QRect,int)));

	updateCycleAreaWidth(0);

	/*-------------------------------------------------------------------------*
	 * create instruction area
	 *-------------------------------------------------------------------------*/

	//instructionArea_ = new view

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
				QString number = QString::number(blockNumber + 1);
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

} // viewpipeline_t::resizeEvent

void viewpipeline_t::updateCycleAreaWidth(int newBlockCount)
{
	setViewportMargins(cycleAreaWidth(), 0, 0, 0);
} // viewpipeline_t::updateCycleAreaWidth

void viewpipeline_t::updateInstructionAreaWidth(int newBlockCount)
{
} // viewpipeline_t::updateInstructionAreaWidth

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
		updateCycleAreaWidth(0);
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
		updateInstructionAreaWidth(0);
	} // if
} // viewpipeline_t::updateInstructionArea
