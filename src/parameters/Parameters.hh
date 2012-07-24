/*----------------------------------------------------------------------------*
 * Definition of parameters_t class
 *----------------------------------------------------------------------------*/

#ifndef parameters_t_hh
#define parameters_t_hh

#include <fstream>
#include <tuple>
#include <algorithm>

#include <XercesIO.hh>
#include <Utils.hh>
#include <LogUtils.hh>
#include <DataBase.hh>

namespace ska {

#ifndef real_t
#define real_t float
#endif

class parameters_t;

std::ostream & operator << (std::ostream & stream,
	const parameters_t & gp);

/*!
	\class parameters_t parameters_t.hh
	\brief parameters_t provides...
*/
class parameters_t {

public:

	friend std::ostream & operator << (std::ostream & stream,
		const parameters_t & gp);

	/*!
		\return An instance of the parameters_t singleton
	*/
	static parameters_t & instance() {
		static parameters_t gp;
		return gp;
	} // instance

	void init() { default_init(); }

	void init(db_t & db, bool append_to_defaults = true) {
		db_.clear();

		if(append_to_defaults) {
			default_init();
		}

		for(db_t::iterator ita = db.begin(); ita != db.end(); ++ita) {
			db_[(*ita).first] = (*ita).second;
		} // for
	} // init

	void init(const char * filename, bool append_to_defaults = true) {
		db_.clear();

		if(append_to_defaults) {
			default_init();
		}

		file_init(filename);
	} // init

	const std::string & operator [](const std::string & key) {
		return std::get<1>(db_[key]);
	} // operator []

	/*-------------------------------------------------------------------------*
	 * boolean
	 *-------------------------------------------------------------------------*/
	
	/*!
	  Return value for \param key in \param value as a boolean.
	*/
	int32_t getval(bool & value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "boolean",
			"key " << key << " is not a boolean value");
		return string_to_bool(value, std::get<1>(db_[key]));
	} // getval

	/*!
		Set value for \param key to the value in \param value.
	 */
	int32_t setval(const bool value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "boolean",
			"key " << key << " is not a boolean value");

		std::string str;
		int32_t retval = bool_to_string(value, str);
		db_[key] = db_tuple_t(db_element_t::boolean, str);
		return retval;
	} // setval

	/*-------------------------------------------------------------------------*
	 * real
	 *-------------------------------------------------------------------------*/

	/*!
	  Return value for \param key in \param value as a real_t.
	*/
	int32_t getval(real_t & value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "real",
			"key " << key << " is not a real value");
		return string_to_type(value, std::get<1>(db_[key]));
	} // getval

	/*!
		Set value for \param key to the value in \param value.
	 */
	int32_t setval(const real_t value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "real",
			"key " << key << " is not a real value");

		std::string str;
		int32_t retval = type_to_string(value, str);
		db_[key] = db_tuple_t(db_element_t::real, str);
		return retval;
	} // setval

	/*-------------------------------------------------------------------------*
	 * string
	 *-------------------------------------------------------------------------*/

	/*!
	  Return value for \param key in \param value as a string.
	*/
	int32_t getval(std::string & value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "string",
			"key " << key << " is not a string value");
		value = std::get<1>(db_[key]);
		return !(value.size() == 0);
	} // getval

	/*!
		Set value for \param key to the value in \param value.
	 */
	int32_t setval(const std::string & value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "string",
			"key " << key << " is not a string value");
		db_[key] = db_tuple_t(db_element_t::string, value);
		return 0;
	} // setval

	/*!
		Set value for \param key to the value in \param value.
	 */
	int32_t setval(const char * value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "string",
			"key " << key << " is not a string value");
		Assert(value != NULL, "NULL const char * value passed to setval");
		db_[key] = db_tuple_t(db_element_t::string, value);
		return 0;
	} // setval

	/*-------------------------------------------------------------------------*
	 * integer
	 *-------------------------------------------------------------------------*/

	/*!
	  Return value for \param key in \param value as a int32_t.
	*/
	int32_t getval(int32_t & value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "integer",
			"key " << key << " is not an integer value");
		return string_to_type(value, std::get<1>(db_[key]));
	} // getval

	/*!
		Set value for \param key to the value in \param value.
	 */
	int32_t setval(const int32_t value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "integer",
			"key " << key << " is not an integer value");

		std::string str;
		int32_t retval = type_to_string(value, str);
		db_[key] = db_tuple_t(db_element_t::real, str);
		return retval;
	} // setval

	/*-------------------------------------------------------------------------*
	 * size
	 *-------------------------------------------------------------------------*/

	/*!
	  Return value for \param key in \param value as a size_t.
	*/
	int32_t getval(size_t & value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "size",
			"key " << key << " is not a size value");
		return string_to_type(value, std::get<1>(db_[key]));
	} // getval

	/*!
		Set value for \param key to the value in \param value.
	 */
	int32_t setval(const size_t value, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) == "size",
			"key " << key << " is not a size value");

		std::string str;
		int32_t retval = type_to_string(value, str);
		db_[key] = db_tuple_t(db_element_t::real, str);
		return retval;
	} // setval

	/*-------------------------------------------------------------------------*
	 * boolean_vector
	 *-------------------------------------------------------------------------*/

	/*!
	  Return value for \param key in \param values as a std::vector<bool>.
	*/
	int32_t getval(std::vector<bool> & values, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) ==
			"boolean_vector", "key " << key << " is not a boolean_vector value");
		return string_to_bool_vector(values, std::get<1>(db_[key]));
	} // getval

	/*!
		Set value for \param key to the values in \param values.
	 */
	int32_t setval(const std::vector<bool> & values, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) ==
			"boolean_vector", "key " << key << " is not a boolean_vector value");

		std::string str;
		int32_t retval = bool_vector_to_string(values, str);
		db_[key] = db_tuple_t(db_element_t::boolean_vector, str);
		return retval;
	} // setval

	/*-------------------------------------------------------------------------*
	 * real_vector
	 *-------------------------------------------------------------------------*/

	/*!
	  Return value for \param key in \param values as a std::vector<real_t>.
	*/
	int32_t getval(std::vector<real_t> & values, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) ==
			"real_vector", "key " << key << " is not a real_vector value");
		return string_to_vector(values, std::get<1>(db_[key]));
	} // getval

	/*!
		Set value for \param key to the values in \param values.
	 */
	int32_t setval(const std::vector<real_t> & values, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) ==
			"real_vector", "key " << key << " is not a real_vector value");

		std::string str;
		int32_t retval = vector_to_string(values, str);
		db_[key] = db_tuple_t(db_element_t::real_vector, str);
		return retval;
	} // setval

	/*-------------------------------------------------------------------------*
	 * integer_vector
	 *-------------------------------------------------------------------------*/

	/*!
	  Return value for \param key in \param values as a std::vector<int32_t>.
	*/
	int32_t getval(std::vector<int32_t> & values, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) ==
			"integer_vector", "key " << key << " is not an integer_vector value");
		return string_to_vector(values, std::get<1>(db_[key]));
	} // getval

	/*!
		Set value for \param key to the values in \param values.
	 */
	int32_t setval(const std::vector<int32_t> & values,
		const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) ==
			"integer_vector", "key " << key << " is not an integer_vector value");

		std::string str;
		int32_t retval = vector_to_string(values, str);
		db_[key] = db_tuple_t(db_element_t::integer_vector, str);
		return retval;
	} // setval

	/*-------------------------------------------------------------------------*
	 * size_vector
	 *-------------------------------------------------------------------------*/

	/*!
	  Return value for \param key in \param values as a std::vector<size_t>.
	*/
	int32_t getval(std::vector<size_t> & values, const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) ==
			"size_vector", "key " << key << " is not a size_vector value");
		return string_to_vector(values, std::get<1>(db_[key]));
	} // getval

	/*!
		Set value for \param key to the values in \param values.
	 */
	int32_t setval(const std::vector<size_t> & values,
		const std::string & key) {
		Assert(stdMapKeyExists(db_, key), "key " << key << " does not exist");
		Assert(db_element_t_to_string(std::get<0>(db_[key])) ==
			"size_vector", "key " << key << " is not a size_vector value");

		std::string str;
		int32_t retval = vector_to_string(values, str);
		db_[key] = db_tuple_t(db_element_t::size_vector, str);
		return retval;
	} // setval

private:

	parameters_t() {}
	parameters_t(const parameters_t & p) {}
	~parameters_t() {}

	void file_init(const char * filename) {
		XercesIO xio(filename);
		xio.populateDataBase(db_);
	} // file_init

	void default_init() {
	} // default_init

	db_t db_;

}; // class parameters_t

std::ostream & operator << (std::ostream & stream,
	const parameters_t & gp) {
	stream << gp.db_ << std::endl;
	return stream;
} // operator <<

} // namespace ska

#endif // parameters_t_hh

/*----------------------------------------------------------------------------*
 * Local Variables: 
 * mode:c++
 * c-basic-offset:3
 * c-file-offsets: ((arglist-intro . +))
 * coding: unix
 * indent-tabs-mode:t
 * tab-width:3
 * End:
 *
 * vim: set ts=3 :
 *----------------------------------------------------------------------------*/
