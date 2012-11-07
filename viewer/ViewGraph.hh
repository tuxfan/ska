#ifndef ViewGraph_hh
#define ViewGraph_hh

#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtGui/QScrollArea>
#include <QtGui/QToolBar>

class viewgraph_t : public QMainWindow
{
	
	Q_OBJECT

public:

	viewgraph_t(QWidget * parent = nullptr);

	void load(const QString & dataset, const QString & data);

private slots:

	void print();
	void zoom_in();
	void zoom_out();
	void zoom_reset();

private:

	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar * scrollBar, double factor);

#ifndef QT_NO_PRINTER
	QPrinter printer_;
#endif

	QAction * print_;
	QAction * zin_;
	QAction * zout_;
	QAction * reset_;

	QToolBar * fileBar_;

	QLabel * imageLabel_;
	QScrollArea * scrollArea_;

	double scale_;

}; // class viewgraph_t

#endif // ViewGraph_hh
