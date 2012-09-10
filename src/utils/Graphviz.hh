#ifndef Graphviz_hh
#define Graphviz_hh

#include <graphviz/gvc.h>
#include <LogUtils.hh>

namespace ska {

class graphviz_t
{
public:

	static graphviz_t & instance() {
		static graphviz_t g;
		return g;
	} // instance

	void clear() {
		if(graph_ != nullptr) {
			gvFreeLayout(gvc_, graph_);
			agclose(graph_);
		} // if

		char name[1024];
		sprintf(name, "%s", "graph");
		graph_ = agopen(name, AGDIGRAPH);
	} // clear

	Agnode_t * add_node(const char * name, const char * label = nullptr) {
		char buffer[1024];
		sprintf(buffer, "%s", name);
		Agnode_t * node = agnode(graph_, buffer);

		if(label != nullptr) {
			char attr[1024];
			sprintf(attr, "%s", "label");	
			sprintf(buffer, "%s", label);	
			agset(node, attr, buffer);
		} // if

		return node;
	} // add_node

	Agedge_t * add_edge(Agnode_t * parent, Agnode_t * child) {
		return agedge(graph_, parent, child);	
	} // add_edge

	void layout(const char * engine) {
		char _engine[1024];
		sprintf(_engine, "%s", engine);
		// FIXME: Error handling
		gvLayout(gvc_, graph_, _engine);
	} // layout

	void write(const char * name) {
		FILE * file = fopen(name, "w");

		if(name == nullptr) {
			ExitOnError("Failed opening " << name, ska::BadFileOperation);
		} // if

		agwrite(graph_, file);
		fclose(file);
	} // write

private:

	graphviz_t()
		: gvc_(nullptr), graph_(nullptr)
		{
			gvc_ = gvContext();
		}

	graphviz_t(const graphviz_t *) {}

	~graphviz_t()
		{
			if(graph_ != nullptr) {
				gvFreeLayout(gvc_, graph_);
				agclose(graph_);
			} // if

			gvFreeContext(gvc_);
		}

	// private data members
	GVC_t * gvc_;
	Agraph_t * graph_;
	
}; // class graphviz_t

} // namespace ska

#endif // Graphviz_hh
