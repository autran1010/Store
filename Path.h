#ifndef PATH_H_
#define PATH_H_

#include "Element.h"
#include <vector>
#include <string>
#include <sstream>

class Path : public Element {
private:
    std::string d_attr; // Lưu chuỗi lệnh vẽ
public:
    Path() = default;
    ~Path() override = default;

    void parseAttributes(xml_node<>* node) override;
    void Draw(Gdiplus::Graphics* graphics) override;
};

#endif