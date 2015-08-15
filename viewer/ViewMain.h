/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#ifndef ViewMain_h
#define ViewMain_h

#include <QtGui/QtGui>

#include "ViewPipeline.h"
#include "ViewSlope.h"
#include "ViewMetrics.h"
#include "ViewGraph.h"
#include "ViewData.h"

class viewmain_t : public QMainWindow
{
	Q_OBJECT

public:

	viewmain_t();
	~viewmain_t();

	void open(QString & file);

private slots:

	void open();
	void openSlope();
	void openMetrics();
	void openGraph();
	void load(int m);

private:

	// GUI elements
	QAction * openAction_;
	QAction * slopeAction_;
	QAction * metricsAction_;
	QAction * graphAction_;
	QAction * quitAction_;

	viewpipeline_t * pipeline_;
	viewslope_t * slope_;
	viewmetrics_t * metrics_;
	viewgraph_t * graph_;

	QMenu * fileMenu_;

	QToolBar * fileBar_;

	QLabel * loaded_;
	QComboBox * selector_;
	QLineEdit * searchBox_;

	QWidget * fill_[10];

	// data
	QVector<module_data_t> modules_;
	program_version_t version_;
	QString architecture_;

}; // class viewmain_t

#endif // ViewMain_h
