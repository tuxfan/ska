/*----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*/

#include <iostream>

#include <ViewMain.hh>

viewmain_t::viewmain_t()
{
	/*-------------------------------------------------------------------------*
	 * Open file action
	 *------------------------------------------------------------------------*/
	openAction_ = new QAction(QIcon(":/icons/open.png"), tr("&Open"), this);
	openAction_->setShortcut(tr("Ctrl+O"));
	connect(openAction_, SIGNAL(triggered()), this, SLOT(open()));

	/*-------------------------------------------------------------------------*
	 * Quit action
	 *------------------------------------------------------------------------*/
	quitAction_ = new QAction(tr("&Quit"), this);
	connect(quitAction_, SIGNAL(triggered()), qApp, SLOT(quit()));

	/*-------------------------------------------------------------------------*
	 * Loaded modules display
	 *------------------------------------------------------------------------*/
	loaded_ = new QLabel(this);
	loaded_->setText("None");

	selector_ = new QComboBox(this);
	connect(selector_, SIGNAL(currentIndexChanged(int)),
		this, SLOT(load(int)));
	selector_->setFixedWidth(200);
	selector_->setEnabled(false);

	/*-------------------------------------------------------------------------*
	 * Status information
	 *------------------------------------------------------------------------*/
	status_ = new QLabel(this);
	status_->setText("No Data Loaded");

	/*-------------------------------------------------------------------------*
	 * Search
	 *------------------------------------------------------------------------*/
	searchBox_ = new QLineEdit("", this);
	searchBox_->setPlaceholderText("search");
	searchBox_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	searchBox_->setEnabled(false);

	/*-------------------------------------------------------------------------*
	 * File menu
	 *------------------------------------------------------------------------*/
	fileMenu_ = menuBar()->addMenu(tr("&File"));
	fileMenu_->addAction(openAction_);
	fileMenu_->addSeparator();
	fileMenu_->addAction(quitAction_);

	/*-------------------------------------------------------------------------*
	 * Toolbar
	 *------------------------------------------------------------------------*/
	int fill = 0;
	fileBar_ = addToolBar(tr("File"));

	fill_[fill] = new QWidget(this);
	fileBar_->addWidget(fill_[fill]);
	fill_[fill]->setFixedWidth(10);

	fileBar_->addAction(openAction_);

	++fill;
	fill_[fill] = new QWidget(this);
	fileBar_->addWidget(fill_[fill]);
	fill_[fill]->setFixedWidth(10);

	fileBar_->addWidget(loaded_);

	++fill;
	fill_[fill] = new QWidget(this);
	fileBar_->addWidget(fill_[fill]);
	fill_[fill]->setFixedWidth(10);

	fileBar_->addWidget(selector_);

	++fill;
	fill_[fill] = new QWidget(this);
	fileBar_->addWidget(fill_[fill]);
	fill_[fill]->setFixedWidth(10);

	fileBar_->addWidget(status_);

	++fill;
	fill_[fill] = new QWidget(this);
	fileBar_->addWidget(fill_[fill]);
	fill_[fill]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	fileBar_->addWidget(searchBox_);

	++fill;
	fill_[fill] = new QWidget(this);
	fileBar_->addWidget(fill_[fill]);
	fill_[fill]->setFixedWidth(10);

	/*-------------------------------------------------------------------------*
	 * Pipeline (main widget)
	 *------------------------------------------------------------------------*/
	pipeline_ = new viewpipeline_t;
	setCentralWidget(pipeline_);

	/*-------------------------------------------------------------------------*
	 * Misc.
	 *------------------------------------------------------------------------*/
	setWindowTitle(tr("SKAView"));
	resize(800, 600);
} // viewmain_t::viewmain_t

/*----------------------------------------------------------------------------*
 * Destructor
 *----------------------------------------------------------------------------*/

viewmain_t::~viewmain_t()
{
} // viewmain_t::~viewmain_t

/*----------------------------------------------------------------------------*
 * Open
 *----------------------------------------------------------------------------*/

void viewmain_t::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"", tr("SKA Files (*.ska);;Text Files (*.txt)"));

	if(fileName.isEmpty()) {
		return;
	} // if

	open(fileName);
} // viewmain_t::open

void viewmain_t::open(QString & fileName)
{
	QFile file(fileName);

	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox msgBox;
		msgBox.setText(QString("Failed opening ") + fileName);
		msgBox.exec();
		return;
	} // if

	QTextStream stream(&file);
	QString line;
	module_data_t module;

	while(!stream.atEnd()) {
		line = stream.readLine();

		// skip empty lines and comments
		if(line.isEmpty() || line.startsWith('#')) {
			continue;
		} // if

		if(line.contains("KEYWORD_SKA_VERSION")) {
			version_.set(line.split(" ")[1]);
		} // if

		if(line.contains("KEYWORD_ARCHITECTURE")) {
			architecture_ = line.split(" ")[1];
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

			modules_.append(module);
		} // if
	} // while

	selector_->clear();

	for(auto ita = modules_.begin(); ita != modules_.end(); ++ita) {
		selector_->addItem((*ita)["name"]);
	} // for

	selector_->setEnabled(true);
	//searchBox_->setEnabled(true);
	loaded_->setText("Architecture: " + architecture_);

	load(0);
} // viewmain_t::open

/*----------------------------------------------------------------------------*
 * Load
 *----------------------------------------------------------------------------*/

void viewmain_t::load(int m)
{
	QString balance;
	double ratio = modules_[m]["flops"].toDouble() /
		modules_[m]["load bytes"].toDouble();
	balance.setNum(ratio);

	// update status
	QString state =
		"Cycles: " + modules_[m]["cycles"] +
		", Balance: " + balance;
	status_->setText(state);

	// load data
	pipeline_->load(modules_[m].cycles, modules_[m].issues,
		modules_[m].pipelines, modules_[m].instructions);
} // viewmain_t::load
