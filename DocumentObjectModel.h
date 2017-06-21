#ifndef DOCUMENTOBJECTMODEL_H
#define DOCUMENTOBJECTMODEL_H

#include "DOMElement.h"

class DocumentObjectModel
{
public:
	DocumentObjectModel();
	DocumentObjectModel(DocumentObjectModel && rvalue);
	~DocumentObjectModel();

	DocumentObjectModel& operator=(DocumentObjectModel&& rvalue);

	DOMElement* AddRoot(string name);

	friend ostream& operator<<(ostream& out, const DocumentObjectModel& dom);

	vector<DOMElement*> roots;
};

#endif