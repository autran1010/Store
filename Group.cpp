#include "stdafx.h"
#include "Group.h"

Group::Group() {}

Group::~Group() {
    for (Element* child : children) delete child;
    children.clear();
}

void Group::addChild(Element* child) {
    children.push_back(child);
}

void Group::parseAttributes(xml_node<>* node) {
    // Parse các thuộc tính của chính thẻ <g> (vd: transform, fill...)
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        parse(attr->name(), attr->value());
    }
}

// Group.cpp
void Group::Draw(Gdiplus::Graphics* graphics) {
    Gdiplus::GraphicsState state = graphics->Save();
    graphics->MultiplyTransform(&this->transform.getMatrix());

    for (Element* child : children) {
        // --- LOGIC KẾ THỪA MÀU SẮC ---

        // 1. Xử lý FILL: Nếu con chưa có màu (opacity == 0) và Cha có màu -> Con lấy màu Cha
        if (child->getFill().getOpacity() == 0 && this->fill.getOpacity() > 0) {
            child->setFill(this->fill);
        }

        // 2. Xử lý STROKE: Tương tự
        if (child->getStroke().getStrokeWidth() == 0 && this->stroke.getStrokeWidth() > 0) {
            child->setStroke(this->stroke);
        }

        // Nếu stroke width cha có, nhưng con chưa có màu stroke -> lấy màu stroke của cha
        if (child->getStroke().getStrokeColor().getOpacity() == 0 && this->stroke.getStrokeColor().getOpacity() > 0) {
            Stroke tempStroke = child->getStroke();
            tempStroke.setStrokeColor(this->stroke.getStrokeColor());
            child->setStroke(tempStroke);
        }

        child->Draw(graphics);
    }

    graphics->Restore(state);
}