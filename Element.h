#include "Color.h"
#include "Stroke.h"
#include "Transform.h"
#include <vector>

#ifndef ELEMENT_H_
#define ELEMENT_H_

class Element {
protected:
    Stroke stroke;
    color fill;
    Transform transform;

    // Các cờ đánh dấu xem thuộc tính này có được set trực tiếp từ XML không
    bool isFillSet = false;
    bool isStrokeSet = false;
    bool isStrokeWidthSet = false;
    bool isFillOpacitySet = false;
    bool isStrokeOpacitySet = false;

    color apply(string value);

public:
    Element() {
        // Mặc định stroke width = 0 (không vẽ)
        stroke.setStrokeWidth(0.0f);
    }
    virtual ~Element() = default;

    void parse(string, string);
    virtual void parseAttributes(xml_node<>*) = 0;
    virtual void Draw(Gdiplus::Graphics* graphics) = 0;

    // --- HÀM MỚI: Kế thừa thuộc tính từ cha ---
    // Nếu con chưa set thuộc tính nào thì lấy thuộc tính đó từ cha
    void inheritFrom(Element* parent);

    Transform& getTransform() { return transform; }
    Stroke& getStroke() { return stroke; }
    void setStroke(const Stroke& s) { stroke = s; }
    color& getFill() { return fill; }
    void setFill(const color& f) { fill = f; }
};

#endif