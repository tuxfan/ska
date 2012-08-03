/*----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*/

#include <iostream>

#include <ViewMain.h>

viewmain_t::viewmain_t()
{
	openAction_ = new QAction(QIcon(":/icons/open.png"), tr("&Open"), this);
	openAction_->setShortcut(tr("Ctrl+O"));

	quitAction_ = new QAction(tr("&Quit"), this);

	filler_ = new QLabel(this);

	searchBox_ = new QLineEdit("", this);
	searchBox_->setPlaceholderText("search");
	searchBox_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

	connect(openAction_, SIGNAL(triggered()), this, SLOT(open()));
	connect(quitAction_, SIGNAL(triggered()), qApp, SLOT(quit()));

	// add file menu
	fileMenu_ = menuBar()->addMenu(tr("&File"));
	fileMenu_->addAction(openAction_);
	fileMenu_->addSeparator();
	fileMenu_->addAction(quitAction_);

	// add file toolbar
	fileBar_ = addToolBar(tr("File"));
	fileBar_->addAction(openAction_);
	fileBar_->addWidget(filler_);
	fileBar_->addWidget(searchBox_);

	pipeline_ = new viewpipeline_t;
	setCentralWidget(pipeline_);

	setWindowTitle(tr("SKAView"));
	resize(800, 600);
} // viewmain_t::viewmain_t

viewmain_t::~viewmain_t()
{
} // viewmain_t::~viewmain_t

void viewmain_t::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"", tr("SKA Files (*.ska);;Text Files (*.txt)"));

	if(fileName.isEmpty()) {
		return;
	} // if

	QFile file(fileName);

	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox msgBox;
		msgBox.setText(QString("Failed opening ") + fileName);
		msgBox.exec();
		return;
	} // if

	QTextStream stream(&file);
	QString line;
	QString pipelines;
	QStringList cycles;
	QStringList issues;
	QStringList instructions;

	while(!stream.atEnd()) {
		line = stream.readLine();
		QStringList sections = line.split('|');

		cycles.append(sections[0]);
		issues.append(sections[1]);
		pipelines += sections[2] + "\n";
		instructions.append(sections[3]);
	} // while

	pipeline_->load(cycles, issues, pipelines, instructions);
} // viewmain_t::open
