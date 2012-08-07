/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/

#include <iostream>
#include <QtGui/QApplication>

#include <ViewMain.hh>

int main(int argc, char ** argv) {

	QApplication app(argc, argv);

	viewmain_t main;

	main.show();

	if(argc == 2) {
		QString fileName = argv[1];
		main.open(fileName);
	} // if

	return app.exec();
} // main
