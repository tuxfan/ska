/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/

#include <iostream>
#include <QtGui/QApplication>

#include <ViewMain.hh>

int main(int argc, char ** argv) {

	QApplication app(argc, argv);

	viewmain_t main;

	main.show();

	return app.exec();
} // main
