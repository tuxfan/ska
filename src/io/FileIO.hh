#ifndef FileIO_hh
#define FileIO_hh

#include <iostream>
#include <fstream>
#include <fstream>

namespace ska {

class file_io_t
{
public:

	static file_io_t & instance() {
		static file_io_t f;
		return f;
	} // instance

	void set_verbose() {
		log_stream_->rdbuf(std::cerr.rdbuf());
	} // set_verbose

	void set_log_stream(const char * name) {
		if(log_file_ != nullptr) {
			log_file_->close();
			delete log_file_;
		} // if

		log_file_ = new std::ofstream(name);
		
		if(!log_file_->good()) {
			std::cerr << "Failed opening " << name << "! Using std::cerr";
			return;
		} // if

		log_stream_->rdbuf(log_file_->rdbuf());
	} // set_log_stream

	void set_out_stream(const char * name) {
		if(out_file_ != nullptr) {
			out_file_->close();
			delete out_file_;
		} // if

		out_file_ = new std::ofstream(name);
		
		if(!out_file_->good()) {
			std::cerr << "Failed opening " << name << "! Using std::cout";
			return;
		} // if

		out_stream_->rdbuf(out_file_->rdbuf());
	} // set_out_stream

	std::ostream & log_stream() { return *log_stream_; }
	std::ostream & out_stream() { return *out_stream_; }

private:

	file_io_t()
		: log_file_(nullptr), out_file_(nullptr),
		log_stream_(nullptr), out_stream_(nullptr) {
		log_stream_ = new std::ostream(std::cerr.rdbuf());
		out_stream_ = new std::ostream(std::cout.rdbuf());

		// default log output is /dev/null
		set_log_stream("/dev/null");
	}

	file_io_t(const file_io_t &) {}

	~file_io_t() {
		if(log_file_ != nullptr) {
			log_file_->close();
		} // if

		if(out_file_ != nullptr) {
			out_file_->close();
		} // if

		delete log_file_;
		delete out_file_;
		delete log_stream_;
		delete out_stream_;
	} // file_io_t

	std::ofstream * log_file_;
	std::ofstream * out_file_;
	std::ostream * log_stream_;
	std::ostream * out_stream_;
}; // file_io_t

} // namespace ska

#endif // FileIO_hh
