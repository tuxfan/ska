/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#ifndef ViewData_h
#define ViewData_h

#include <QtCore/QtCore>

#include <ViewUtils.hh>

struct module_data_t : public QMap<QString, QString> {
	void clear() {
		QMap::clear();
		cycles.clear();
		issues.clear();
		pipelines = "";
		instructions.clear();
		x_points.clear();
		y_points.clear();
	} // clear

	QStringList cycles;
	QStringList issues;
	QString pipelines;
	QStringList instructions;
	QString metrics;
	QVector<double> x_points;
	QVector<double> y_points;
}; // struct module_data_t

struct program_version_t {
	program_version_t() : set_(false) {}

	int32_t set(QString & version) {
		set_ = false;
		QStringList subs = version.split('.');

		if(subs.size() != 3) {
			return -1;
		} // if

		major_ = subs[0].toUInt();
		minor_ = subs[1].toUInt();
		build_ = subs[2].toUInt();

		set_ = true;
		return 0;
	} // set

#define VERSION_ACCESSOR(v)					\
	unsigned v() const {							\
		ASSERT(set_, "Version not set");		\
		return v##_;								\
	}

	VERSION_ACCESSOR(major);
	VERSION_ACCESSOR(minor);
	VERSION_ACCESSOR(build);
#undef VERSION_ACCESSOR

	bool set_;
	unsigned major_;
	unsigned minor_;
	unsigned build_;
}; // struct program_version_t

#endif // ViewData_h
