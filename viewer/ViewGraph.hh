#ifndef ViewGraph_hh
#define ViewGraph_hh

#include <graphviz/gvc.h>

class viewgraph_t : public QLabel
{
public:

	viewgraph_t(QWidget * parent = nullptr) : QLabel(parent) {
	} // viewgraph_t

	void load(const QString & dataset, const QString & data) {
		Agraph_t * graph;
		GVC_t * gvc;

		gvc = gvContext();

		// read the graph
		char * _data = strdup(data.toStdString().data());
		graph = agmemread(_data);
		free(_data);

		gvLayout (gvc, graph, "dot");

		// this is lame, for now we have to write to a temporary
		// file for the rendering and then read it back in.
		gvRenderFilename(gvc, graph, "png", "skaview_gv.png");
		QImage image("skaview_gv.png");
		int err = system("rm -rf skaview_gv.png");	
		if(err == -1 || err == 127) {
			// FIXME: error
		} // if

		setPixmap(QPixmap::fromImage(image));
		setScaledContents(true);

	} // load

private:

}; // class viewgraph_t

#endif // ViewGraph_hh
