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

        // LOGIC MỚI: Chỉ kế thừa nếu con CHƯA HỀ SET fill (isFillSet == false)
        if (!child->getIsFillSet() && this->fill.getOpacity() > 0) {
            child->setFill(this->fill);
            // Lưu ý: Không cần set isFillSet = true cho con, để nó không ảnh hưởng logic khác
        }

        // Tương tự với Stroke
        if (!child->getIsStrokeSet() && this->stroke.getStrokeWidth() > 0) {
            child->setStroke(this->stroke);
        }

        child->Draw(graphics);
    }
    graphics->Restore(state);
}