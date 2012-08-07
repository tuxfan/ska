#ifndef ViewMain_h
#define ViewMain_h

#include <QtGui/QtGui>

#include <ViewPipeline.hh>
#include <ViewData.hh>

class viewmain_t : public QMainWindow
{
	Q_OBJECT

public:

	viewmain_t();
	~viewmain_t();

private slots:

	void open();
	void load(int m);

private:

	// GUI elements
	QAction * openAction_;
	QAction * quitAction_;

	viewpipeline_t * pipeline_;

	QMenu * fileMenu_;

	QToolBar * fileBar_;

	QComboBox * selector_;
	QLabel * status_;
	QLineEdit * searchBox_;

	QWidget * fill0_;
	QWidget * fill1_;
	QWidget * fill2_;
	QWidget * fill3_;
	QWidget * fill4_;

	// data
	QVector<module_data_t> modules_;

}; // class viewmain_t

#endif // ViewMain_h
