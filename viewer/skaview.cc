/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#include <iostream>
#include <QtGui/QApplication>

#include "ViewMain.h"

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
