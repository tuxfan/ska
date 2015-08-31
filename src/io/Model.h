#ifndef Model_h
#define Model_h

#include <string>
#include <yaml-cpp/yaml.h>

namespace ska {

class model_t
{
public:

	using node_t = YAML::Node;

	model_t() {};
	model_t(const model_t &) = delete;
	model_t & operator = (const model_t &) = delete;
	~model_t() {}

	static model_t & instance() {
		static model_t f;
		return f;
	} // instance

	void init(const std::string & filename) {
		init(filename.c_str());
	} // init

	void init(const char * filename) {
		node_ = YAML::LoadFile(filename);
	} // init

	auto operator ()(const char * key) const {
		return node_[key];
	} // operator ()

	auto operator [](const char * key) const {
		return node_[key];
	} // operator []

private:

	node_t node_;

}; // model_t

} // namespace ska

#endif // Model_h
