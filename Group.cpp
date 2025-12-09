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

void Group::Draw(Gdiplus::Graphics* graphics) {
    // 1. Lưu trạng thái
    Gdiplus::GraphicsState state = graphics->Save();

    // 2. Áp dụng Transform của Group (nếu có)
    graphics->MultiplyTransform(&this->transform.getMatrix());

    // 3. Bảo các con tự vẽ
    for (Element* child : children) {
        child->Draw(graphics);
    }

    // 4. Khôi phục trạng thái
    graphics->Restore(state);
}