#ifndef Dependency_hh
#define Dependency_hh

#include <vector>

#include <OpCodes.hh>
#include <OpTypes.hh>

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

	dependency_t(std::string name, state_t state = retired)
		: name_(name), state_(state), strahler_(1), depth_(0)
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

	size_t num_dependencies() const {
		return depends_.size();
	} // num_dependencies

	dependency_t * dependency(size_t index) {
		return depends_[index];
	} // dependency

	/*-------------------------------------------------------------------------* 
	 * Return dependency vector.
	 *-------------------------------------------------------------------------*/

	const std::vector<dependency_t *> & dependencies() const {
		return depends_;
	} // dependencies

	/*-------------------------------------------------------------------------* 
	 *-------------------------------------------------------------------------*/

	virtual bool in_tree() {
//		return depends_.size() > 0;
		return true;
	} //

	/*-------------------------------------------------------------------------* 
	 *-------------------------------------------------------------------------*/

	void prune(bool recursive = false) {
		if(recursive && depends_.size() > 0) {
			for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
				(*ita)->prune();	
			} // for
		} // if

#if defined(HAVE_GRAPHVIZ)
		graphviz_t & graph = graphviz_t::instance();

		char _this[1024];
		sprintf(_this, "%p", (void *)(this));
		graph.remove_node(_this);
#endif
	} // prune

	/*-------------------------------------------------------------------------*
	 * This is not meaningful for pure dependencies.
	 *-------------------------------------------------------------------------*/

	virtual void update_tree_properties() {}

	/*-------------------------------------------------------------------------* 
	 * Return string name.
	 *-------------------------------------------------------------------------*/

	const std::string & name() const {
		return name_;
	} // name

	/*-------------------------------------------------------------------------* 
	 * Return the current state.
	 *-------------------------------------------------------------------------*/

	state_t state() const { return state_; }

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
	virtual unsigned opcode() const { return dummy_op(); }
	virtual unsigned optype() const { return any_type(); }

	/*-------------------------------------------------------------------------* 
	 * Return the Graphviz node associated with this instruction.
	 *-------------------------------------------------------------------------*/

#if defined(HAVE_GRAPHVIZ)
	Agnode_t * agnode() {
		return agnode_;
	} // agnode

	void set_label(const char * label) {
		graphviz_t & graph = graphviz_t::instance();
		char _this[1024];
		sprintf(_this, "%p", (void *)(this));
		graph.set_label(_this, label);
	} // set_label

	void set_color(const char * color) {
		graphviz_t & graph = graphviz_t::instance();
		char _this[1024];
		sprintf(_this, "%p", (void *)(this));
		graph.set_node_attribute(_this, GV_COLOR, color);
	} // set_color

	void set_fill_color(const char * color) {
		graphviz_t & graph = graphviz_t::instance();
		char _this[1024];
		sprintf(_this, "%p", (void *)(this));
		graph.set_node_attribute(_this, GV_FILL_COLOR, color);
	} // set_fill_color

	void set_style(const char * style) {
		graphviz_t & graph = graphviz_t::instance();
		char _this[1024];
		sprintf(_this, "%p", (void *)(this));
		graph.set_node_attribute(_this, GV_STYLE, style);
	} // set_style

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
