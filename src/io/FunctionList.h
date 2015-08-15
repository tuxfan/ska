#ifndef FunctionList_h
#define FunctionList_h

#include <iostream>
#include <fstream>
#include <set>

namespace ska {

class function_list_t
{
public:

	static function_list_t & instance() {
		static function_list_t f;
		return f;
	} // instance

	bool contains(const char * function) {
		std::string func(function);
		return contains(func);
	} // contains

	bool contains(const std::string & function) {
		return list_.empty() ? true : list_.find(function) != list_.end();
	} // contains

	void read(const std::string & name) {
		read(name.c_str());
	} // read

	void read(const char * name) {
		std::ifstream f(name);
		std::string line;

		if(f.is_open()) {
			while(f >> line) {
				list_.emplace(line);
			} // while
		} // if
	} // read

private:

	function_list_t() {}
	function_list_t(const function_list_t &) {}

	~function_list_t() {}

	std::set<std::string> list_;

}; // function_list_t

} // namespace ska

#endif // FunctionList_h
