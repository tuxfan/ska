#ifndef ViewMain_h
#define ViewMain_h

#include <QtGui/QtGui>

#include <ViewPipeline.h>

class viewmain_t : public QMainWindow
{
	Q_OBJECT

public:

	viewmain_t();
	~viewmain_t();

private slots:

	void open();

private:

	QAction * openAction_;
	QAction * quitAction_;

	QPlainTextEdit * textEdit_;

	QMenu * fileMenu_;

}; // class viewmain_t

#endif // ViewMain_h
