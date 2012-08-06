/*----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*/

#include <iostream>

#include <ViewMain.hh>
#include <ViewData.hh>

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
	program_version_t version;
	QVector<module_data_t> modules;
	module_data_t module;

	while(!stream.atEnd()) {
		line = stream.readLine();

		// skip empty lines and comments
		if(line.isEmpty() || line.startsWith('#')) {
			continue;
		} // if

		if(line.contains("KEYWORD_SKA_VERSION")) {
			version.set(line.split(" ")[1]);
			std::cerr << "Major: " << version.major() << std::endl;
			std::cerr << "Minor: " << version.minor() << std::endl;
			std::cerr << "Build: " << version.build() << std::endl;
		} // if

		if(line.contains("BEGIN_MODULE")) {
			module.clear();

			line = stream.readLine();
			while(!stream.atEnd() && !line.contains("END_MODULE")) {

				if(line.contains("KEYWORD")) {
					QStringList _pair = line.split(' ');
					QString pretty = _pair[0].replace("KEYWORD_", "").
						replace('_', ' ').toLower();
					module[pretty] = _pair[1];
				} // if

				if(line.contains("BEGIN_INSTRUCTION_STREAM")) {

					line = stream.readLine();
					while(!stream.atEnd() &&
						!line.contains("END_INSTRUCTION_STREAM")) {

						QStringList sections = line.split('|');
						module.cycles.append(sections[0]);
						module.issues.append(sections[1]);
						module.pipelines += sections[2] + "\n";
						module.instructions.append(sections[3]);
						line = stream.readLine();
					} // while
				} // if

				line = stream.readLine();
			} // while

			modules.append(module);
		} // if
	} // while

	pipeline_->load(modules[0].cycles, modules[0].issues,
		modules[0].pipelines, modules[0].instructions);
} // viewmain_t::open
