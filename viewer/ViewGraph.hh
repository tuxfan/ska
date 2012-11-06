#ifndef ViewGraph_hh
#define ViewGraph_hh

#include <graphviz/gvc.h>

class viewgraph_t : public QLabel
{
public:

	viewgraph_t(QWidget * parent = nullptr) : QLabel(parent) {
	} // viewgraph_t

	void load(const QString & dataset, const QString & data) {
		char * _data = strdup(data.toStdString().data());
		Agraph_t * G;
		GVC_t * gvc;

		gvc = gvContext();
		G = agmemread(_data);
		gvLayout (gvc, G, "dot");

//		gvRenderData(gvc, G, "bmp", &result, &length);
gvRenderFilename (gvc, G, "png", "skaview_gv.png");
QImage image("skaview_gv.png");
setPixmap(QPixmap::fromImage(image));
#if 0
gvRenderData (GVC_t *gvc, Agraph_t* g, char *format, char **result,
      unsigned int *length)
#endif

		free(_data);
	} // load

private:

}; // class viewgraph_t

#endif // ViewGraph_hh
