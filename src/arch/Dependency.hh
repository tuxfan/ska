#ifndef Dependency_hh
#define Dependency_hh

#include <vector>

#if defined(HAVE_GRAPHVIZ)
#include <Graphviz.hh>
#endif

namespace ska {

class dependency_t
{
public:

	/*-------------------------------------------------------------------------* 
	 * State type.
	 *-------------------------------------------------------------------------*/

	enum state_t {
		pending,
		stalled,
		staging,
		executing,
		retired
	}; // enum state_t

	const char * state_strings[5] = {
		"pending",
		"stalled",
		"staging",
		"executing",
		"retired"
	};

	/*-------------------------------------------------------------------------* 
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	dependency_t(std::string name)
		: name_(name), state_(retired), strahler_(1), depth_(1)
#if defined(HAVE_GRAPHVIZ)
		, agnode_(nullptr)
#endif
		{
#if defined(HAVE_GRAPHVIZ)
		graphviz_t & graph = graphviz_t::instance();
		char _this[1024];
		sprintf(_this, "%p", (void *)(this));
		agnode_ = graph.add_node(_this, name_.c_str());
#endif
	} // dependency_t

	/*-------------------------------------------------------------------------* 
	 * Destructor.
	 *-------------------------------------------------------------------------*/

	virtual ~dependency_t() {}

	/*-------------------------------------------------------------------------* 
	 * Add dependency.
	 *-------------------------------------------------------------------------*/

	void add_dependency(dependency_t * d) {
		depends_.push_back(d);
	} // add_dependency

	/*-------------------------------------------------------------------------* 
	 * Return the current state.
	 *-------------------------------------------------------------------------*/

	state_t state() { return state_; }

	/*-------------------------------------------------------------------------* 
	 * Set current state.
	 *-------------------------------------------------------------------------*/

	void set_state(state_t s) { state_ = s; }

	/*-------------------------------------------------------------------------* 
	 * Return the Strahler number of this node.
	 *-------------------------------------------------------------------------*/

	size_t strahler_number() const { return strahler_; }

	/*-------------------------------------------------------------------------* 
	 * Return the depth number of this node.
	 *-------------------------------------------------------------------------*/

	size_t depth() const { return depth_; }

	/*-------------------------------------------------------------------------* 
	 * These are not meaningful for pure dependencies.
	 *-------------------------------------------------------------------------*/

	virtual size_t cycle_issued() const { return 0; }
	virtual size_t cycle_retired() const { return 0; }

	/*-------------------------------------------------------------------------* 
	 * Return the Graphviz node associated with this instruction.
	 *-------------------------------------------------------------------------*/

#if defined(HAVE_GRAPHVIZ)
	Agnode_t * agnode() {
		return agnode_;
	} // agnode
#endif

#if 0
	virtual std::string info() {
	} // info
#endif

protected:

	std::string name_;
	state_t state_;
	size_t strahler_;
	size_t depth_;

	std::vector<dependency_t *> depends_;

#if defined(HAVE_GRAPHVIZ)
	Agnode_t * agnode_;
#endif

}; // class dependency_t

} // namespace ska

#endif // Dependency_hh
