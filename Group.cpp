#include "stdafx.h"
#include "Group.h"

void Group::parseAttributes(xml_node<>* node) {
    // Parse các thuộc tính, bao gồm cả transform
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        parse(attr->name(), attr->value());
    }
}

void Group::Draw(Gdiplus::Graphics* graphics) {
    // 1. Lưu trạng thái Graphics hiện tại (để không làm hỏng các hình vẽ sau Group này)
    GraphicsState state = graphics->Save();

    // 2. QUAN TRỌNG: Áp dụng ma trận biến đổi của Group này
    // Lệnh này sẽ xoay/scale toàn bộ hệ trục tọa độ trước khi vẽ các con
    graphics->MultiplyTransform(this->transform.getMatrix(), MatrixOrderPrepend);

    // 3. Vẽ tất cả các con
    // Lúc này các con sẽ được vẽ trên hệ trục đã bị xoay/scale bởi bước 2
    for (Element* child : children) {
        // QUAN TRỌNG: Con kế thừa thuộc tính từ Group này
        child->inheritFrom(this);
        child->Draw(graphics);
    }

    // 4. Khôi phục trạng thái cũ (Undo bước 2)
    graphics->Restore(state);
}