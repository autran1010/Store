#include "stdafx.h"
#include "Group.h"

void Group::parseAttributes(xml_node<>* node) {
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        parse(attr->name(), attr->value());
    }
}

void Group::Draw(Gdiplus::Graphics* graphics) {

    GraphicsState state = graphics->Save();

    graphics->MultiplyTransform(this->transform.getMatrix(), MatrixOrderPrepend);


    for (Element* child : children) {

        child->inheritFrom(this);
        child->Draw(graphics);
    }

    graphics->Restore(state);
}