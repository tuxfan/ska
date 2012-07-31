#include <ViewMain.h>

viewmain_t::viewmain_t()
{
	openAction_ = new QAction(tr("&Open"), this);
	quitAction_ = new QAction(tr("&Quit"), this);

	connect(openAction_, SIGNAL(triggered()), this, SLOT(open()));
	connect(quitAction_, SIGNAL(triggered()), qApp, SLOT(quit()));

	fileMenu_ = menuBar()->addMenu(tr("&File"));
	fileMenu_->addAction(openAction_);
	fileMenu_->addSeparator();
	fileMenu_->addAction(quitAction_);

	textEdit_ = new QPlainTextEdit;
	setCentralWidget(textEdit_);

	//setWindowTitle(tr("Notepad"));
} // viewmain_t::viewmain_t

viewmain_t::~viewmain_t()
{
} // viewmain_t::~viewmain_t

void viewmain_t::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"", tr("SKA Files (*.ska);;Text Files (*.txt)"));
} // viewmain_t::open
