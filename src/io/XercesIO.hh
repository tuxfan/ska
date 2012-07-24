/*----------------------------------------------------------------------------*
 * Definition of XercesIO class
 *----------------------------------------------------------------------------*/

#ifndef XercesIO_hh
#define XercesIO_hh

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/dom/DOM.hpp>

#include <DataBase.hh>
#include <LogUtils.hh>

namespace ska {

std::string transcoder(const XMLCh * const code) {
	char * a = XERCES_CPP_NAMESPACE::XMLString::transcode(code);
	std::string b(a);
	XERCES_CPP_NAMESPACE::XMLString::release(&a);
	return b;
} // transcoder

struct XercesIOErrorHandler : public XERCES_CPP_NAMESPACE::DOMErrorHandler {
	bool handleError(const XERCES_CPP_NAMESPACE::DOMError & error) {
		ExitOnError("XercesIOErrorHandler: " <<
			transcoder(error.getMessage()) << " " <<
			error.getLocation(), ErrCode::XercesError);
	} // handleError
}; // XercesIOErrorHandler

/*!
 * \class XercesIO XercesIO.h
 *	\brief XercesIO provides...
 */
class XercesIO
{
public:

	XercesIO(const char * filename)
		: handler_(NULL) {
		using namespace XERCES_CPP_NAMESPACE;

		// setup crappy Xerces stuff
		XMLPlatformUtils::Initialize();
		
		static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
		implementation_ = DOMImplementationRegistry::getDOMImplementation(gLS);
		parser_ = static_cast<DOMImplementationLS *>(implementation_)->
			createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
		parser_->getDomConfig()->setParameter(
			XMLUni::fgDOMValidateIfSchema, true);

		XercesIOErrorHandler * handler_ = new XercesIOErrorHandler;
		parser_->getDomConfig()->setParameter(XMLUni::fgDOMErrorHandler,
			handler_);

		parser_->resetDocumentPool();
		doc_ = parser_->parseURI(filename);
	} // XercesIO	

	~XercesIO() {
		XERCES_CPP_NAMESPACE::XMLPlatformUtils::Terminate();
		delete handler_;
	} // XercesIO

	int32_t populateDataBase(db_t & db) {
		using namespace XERCES_CPP_NAMESPACE;
		DOMNode * node = doc_->getDocumentElement();
		//Assert(node, "Bad DOMNode in populateDataBase");

		if(node->getNodeType() != DOMNode::ELEMENT_NODE) {
			ExitOnError("Wrong XML Node Type", ErrCode::AssertionFailed);
		} // if

		// go through the children of the root node
		for(DOMNode * child = node->getFirstChild(); child != 0;
			child = child->getNextSibling()) {

			if(transcoder(child->getNodeName()) == "value") {
				// get the key
				std::string key(get_key(child));

				// get the type
				DOMNode * element_node = get_child(child, DOMNode::ELEMENT_NODE);
				std::string type = transcoder(element_node->getNodeName());

				// get the value
				DOMNode * text_node =
					get_child(element_node, DOMNode::TEXT_NODE);
				std::string value(transcoder(text_node->getNodeValue()));

				// set value in database
				db[key] = db_tuple_t(string_to_db_element_t(type),
					value);
			} // if
		} // for

		return 0;
	} // populateDataBase

private:

	XercesIO(const XercesIO & io);
	XercesIO & operator =(const XercesIO & io);

	XERCES_CPP_NAMESPACE::DOMNode * get_child(
		XERCES_CPP_NAMESPACE::DOMNode * node,
		XERCES_CPP_NAMESPACE::DOMNode::NodeType type) {
		using namespace XERCES_CPP_NAMESPACE;
		//Assert(node, "Bad DOMNode in get_child");

		for(DOMNode * child = node->getFirstChild(); child != 0;
			child = child->getNextSibling()) {

			if(child->getNodeType() == type) {
				return child;
			} // if
		} // for

		ExitOnError("Failed to find child node of type " << type,
			ErrCode::XercesError);
		return NULL;
	} // get_child

	std::string get_key(XERCES_CPP_NAMESPACE::DOMNode * node) {
		//Assert(node, "Bad DOMNode in get_key");

		// get the node map
		XERCES_CPP_NAMESPACE::DOMNamedNodeMap * nnm = node->getAttributes();

		// make sure that this node has the correct number of attributes
		if(nnm->getLength() != 1) {
			ExitOnError("In tag" << transcoder(node->getNodeName()) <<
				" has incorrect number of attributes", ErrCode::XercesError);
		} // if

		// get the key attribute
		XERCES_CPP_NAMESPACE::DOMAttr * attr =
			static_cast<XERCES_CPP_NAMESPACE::DOMAttr *>(nnm->item(0));

		if(transcoder(attr->getName()) != "key") {
			ExitOnError("In tag" << transcoder(node->getNodeName()) <<
				" unrecognized attribute", ErrCode::XercesError);
		} // if

		return transcoder(attr->getValue());
	} // get_key

	// private data members
	XercesIOErrorHandler * handler_;

	XERCES_CPP_NAMESPACE::DOMImplementation * implementation_;
	XERCES_CPP_NAMESPACE::DOMLSParser * parser_;
	XERCES_CPP_NAMESPACE::DOMDocument * doc_;

}; // class XercesIO

} // namespace ska

#endif // XercesIO_h
