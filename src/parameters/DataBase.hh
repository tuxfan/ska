/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Definition of DataBase class
 *----------------------------------------------------------------------------*/

#ifndef DataBase_hh
#define DataBase_hh

#include <map>
#include <tuple>
#include <string>

namespace ska {

enum db_element_t {
	boolean,
	real,
	string,
	integer,
	size,
	boolean_vector,
	real_vector,
	integer_vector,
	size_vector,
	unknown
}; // db_element_t

typedef std::tuple<db_element_t, std::string> db_tuple_t;
typedef std::map<std::string, db_tuple_t> db_t;

db_element_t string_to_db_element_t(const std::string & value) {
	return value == "boolean" ? boolean :
		value == "real" ? real :
		value == "string" ? string :
		value == "integer" ? integer :
		value == "size" ? size :
		value == "boolean_vector" ? boolean_vector :
		value == "real_vector" ? real_vector :
		value == "integer_vector" ? integer_vector :
		value == "size_vector" ? size_vector :
		unknown;
} // string_to_db_element_t

std::string db_element_t_to_string(const db_element_t type) {
	return type == boolean ? "boolean" :
		type == real ? "real" :
		type == string ? "string" :
		type == integer ? "integer" :
		type == size ? "size" :
		type == boolean_vector ? "boolean_vector" :
		type == real_vector ? "real_vector" :
		type == integer_vector ? "integer_vector" :
		type == size_vector ? "size_vector" :
		"unknown";
} // db_element_t_to_string

std::ostream & operator << (std::ostream & stream, const db_t & db) {
	for(db_t::const_iterator ita = db.begin(); ita != db.end(); ++ita) {
		stream << std::endl;
		db_tuple_t tuple = (*ita).second;
		stream << "key: " << (*ita).first << std::endl;
		stream << "type: " << db_element_t_to_string(std::get<0>(tuple))
			<< std::endl;
		stream << "value: " << std::get<1>(tuple) << std::endl;
	} // for

	return stream;
} // operator <<

} // namespace ska

#endif // DataBase_hh

/*----------------------------------------------------------------------------*
 * Local Variables: 
 * mode:c++
 * c-basic-offset:3
 * indent-tabs-mode:t
 * tab-width:3
 * End:
 *
 * vim: set ts=3 :
 *----------------------------------------------------------------------------*/
