#include "Element.h"
#include <vector>

#ifndef GROUP_H_
#define GROUP_H_

class Group : public Element {
private:
    std::vector<Element*> children; // Danh sách các đối tượng con
public:
    Group() = default;

    // Destructor cần xóa các con để tránh rò rỉ bộ nhớ
    ~Group() {
        for (auto child : children) delete child;
        children.clear();
    }

    void addChild(Element* element) {
        children.push_back(element);
    }

    void parseAttributes(xml_node<>* node) override;
    void Draw(Gdiplus::Graphics* graphics) override;
};

#endif