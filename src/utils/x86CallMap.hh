#ifndef x86CallMap_hh
#define x86CallMap_hh

namespace ska {

bool x86_match(const std::string & call) {
	return call.find("llvm.x86") != std::string::npos;
} // match

std::map<std::string, std::string> x86_call_map = {
	{ "llvm.x86.sse.rsqrt.ps", "rsqrt"},
	{ "llvm.x86.sse.rsqrt.pd", "rsqrt"},
	{ "llvm.x86.sse3.hadd.ps", "hadd"},
	{ "llvm.x86.sse3.hadd.pd", "hadd"}
};

} // namespace ska

#endif // x86CallMap_hh
