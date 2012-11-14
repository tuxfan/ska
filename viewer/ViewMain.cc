/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#include <thread>
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
	 * Show slope action
	 *------------------------------------------------------------------------*/
	slopeAction_ = new QAction(QIcon(":/icons/slope.png"), tr("&Slope"), this);
	slopeAction_->setShortcut(tr("Ctrl+1"));
	connect(slopeAction_, SIGNAL(triggered()), this, SLOT(openSlope()));

	/*-------------------------------------------------------------------------*
	 * Show metrics action
	 *------------------------------------------------------------------------*/
	metricsAction_ = new QAction(QIcon(":/icons/metrics.png"),
		tr("&Metrics"), this);
	metricsAction_->setShortcut(tr("Ctrl+2"));
	connect(metricsAction_, SIGNAL(triggered()), this, SLOT(openMetrics()));

	/*-------------------------------------------------------------------------*
	 * Show metrics action
	 *------------------------------------------------------------------------*/
	graphAction_ = new QAction(QIcon(":/icons/graph.png"),
		tr("&Graph"), this);
	graphAction_->setShortcut(tr("Ctrl+3"));
	connect(graphAction_, SIGNAL(triggered()), this, SLOT(openGraph()));

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
	fileBar_->addAction(slopeAction_);
	fileBar_->addAction(metricsAction_);
	fileBar_->addAction(graphAction_);

	++fill;
	fill_[fill] = new QWidget(this);
	fileBar_->addWidget(fill_[fill]);
	fill_[fill]->setFixedWidth(10);

	fileBar_->addWidget(loaded_);

	++fill;
	fill_[fill] = new QWidget(this);
	fileBar_->addWidget(fill_[fill]);
	//fill_[fill]->setFixedWidth(10);
	fill_[fill]->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

	fileBar_->addWidget(selector_);

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
	 * Slope view
	 *------------------------------------------------------------------------*/
	slope_ = new viewslope_t;

	/*-------------------------------------------------------------------------*
	 * Metrics view
	 *------------------------------------------------------------------------*/
	metrics_ = new viewmetrics_t;

	/*-------------------------------------------------------------------------*
	 * Graph view
	 *------------------------------------------------------------------------*/
	graph_ = new viewgraph_t;

	connect(pipeline_, SIGNAL(updateRequest(QRect,int)),
		slope_, SLOT(updateHighlightArea(QRect,int)));
	connect(pipeline_, SIGNAL(highlightMove(int)),
		slope_, SLOT(moveHighlight(int)));

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
			QStringList words = line.split(" ");
			words.removeFirst();
			architecture_ = words.join(" ");
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

				if(line.contains("BEGIN_SLOPE_DATA")) {
					line = stream.readLine();
					while(!stream.atEnd() &&
						!line.contains("END_SLOPE_DATA")) {
						QStringList coordinates = line.split(' ');
						module.x_points.append(coordinates[0].toDouble());
						module.y_points.append(coordinates[1].toDouble());
						line = stream.readLine();
					} // while
				} // if

				if(line.contains("BEGIN_GRAPH_DATA")) {
					line = stream.readLine();
					while(!stream.atEnd() &&
						!line.contains("END_GRAPH_DATA")) {
						module.graph += line;	
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

		// this is a convenient place to update metrics string
		(*ita).metrics = "";
		(*ita).metrics += "Instructions: " + (*ita)["instructions"] + "\n";
		(*ita).metrics += "Cycles: " + (*ita)["cycles"] + "\n";
		(*ita).metrics += "Stalls: " + (*ita)["stalls"] + "\n";
		(*ita).metrics += "Flops: " + (*ita)["flops"] + "\n";
		(*ita).metrics += "Iops: " + (*ita)["iops"] + "\n";
		(*ita).metrics += "Branches: " + (*ita)["branches"] + "\n";
		(*ita).metrics +=
			"Stack Allocations: " + (*ita)["stack allocations"] + "\n";
		(*ita).metrics +=
			"Stack Allocation Bytes: " + (*ita)["stack allocation bytes"] + "\n";
		(*ita).metrics += "Loads: " + (*ita)["loads"] + "\n";
		(*ita).metrics += "Load Bytes: " + (*ita)["load bytes"] + "\n";
		(*ita).metrics += "Stores: " + (*ita)["stores"] + "\n";
		(*ita).metrics += "Store Bytes: " + (*ita)["store bytes"] + "\n";
		(*ita).metrics +=
			"Cycles per Instruction: " + (*ita)["cycles per instruction"] + "\n";
		(*ita).metrics += "Algorithmic Balance: " + (*ita)["balance"] + "\n";
		(*ita).metrics += "Lewin Number: " + (*ita)["lewin"] + "\n";
		(*ita).metrics += "Depth: " + (*ita)["depth"] + "\n";
	} // for

	selector_->setEnabled(true);
	//searchBox_->setEnabled(true);
	loaded_->setText("Architecture: " + architecture_);

	load(0);
} // viewmain_t::open

/*----------------------------------------------------------------------------*
 * OpenSlope
 *----------------------------------------------------------------------------*/

void viewmain_t::openSlope()
{
	slope_->show();
} // viewmain_t::openSlope

/*----------------------------------------------------------------------------*
 * OpenMetrics
 *----------------------------------------------------------------------------*/

void viewmain_t::openMetrics()
{
	metrics_->show();
} // viewmain_t::openMetrics

/*----------------------------------------------------------------------------*
 * OpenGraph
 *----------------------------------------------------------------------------*/

void viewmain_t::openGraph()
{
	graph_->show();
} // viewmain_t::openGraph

/*----------------------------------------------------------------------------*
 * Load
 *----------------------------------------------------------------------------*/

void viewmain_t::load(int m)
{
	QString balance;
	balance.setNum(modules_[m]["balance"].toDouble());

#if 0
	std::thread t(graph_load, std::ref(*graph_),
		std::ref(modules_[m]["name"]), std::ref(modules_[m].graph));
#endif

	// load data
	pipeline_->load(modules_[m].cycles, modules_[m].issues,
		modules_[m].pipelines, modules_[m].instructions);

	// load slope data
	slope_->load(modules_[m]["name"], modules_[m].x_points,
		modules_[m].y_points);
	
	// load metrics data
	metrics_->load(modules_[m]["name"], modules_[m].metrics);

#if 0
	t.join();

	// load graph data
	gthread_ = new std::thread(graph_load, std::ref(*graph_),
		std::ref(modules_[m]["name"]), std::ref(modules_[m].graph));
#endif
	graph_->load(modules_[m]["name"], modules_[m].graph);
} // viewmain_t::load
