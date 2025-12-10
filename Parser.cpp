#include "stdafx.h"
#include "Parser.h"

Parser::Parser() {
    source = "";
}

Parser::~Parser() {
    for (size_t i = 0; i < children.size(); i++)
        delete children[i];
    children.clear();
}

Element* Parser::createElement(const string& name) {
    if (name == "rect") return new rectangle();
    else if (name == "circle") return new Circle();
    else if (name == "ellipse") return new ellipse();
    else if (name == "line") return new Line();
    else if (name == "polyline") return new polyline();
    else if (name == "polygon") return new polygon();
    else if (name == "text") return new Text();
    else if (name == "path") return new Path(); // YC 2
    else if (name == "g") return new Group();   // YC 3
    else return nullptr;
}

// --- HÀM MỚI: Xử lý đệ quy ---
Element* Parser::parseElement(xml_node<>* node) {
    string name = node->name();
    Element* elem = createElement(name);

    if (elem) {
        // 1. Parse thuộc tính của chính đối tượng (bao gồm transform, fill, stroke...)
        elem->parseAttributes(node);

        // 2. Nếu là Group, cần đệ quy để lấy các con bên trong
        if (name == "g") {
            Group* group = dynamic_cast<Group*>(elem);
            if (group) {
                // Duyệt qua tất cả các node con nằm trong thẻ <g>...</g>
                for (xml_node<>* childNode = node->first_node(); childNode; childNode = childNode->next_sibling()) {

                    // GỌI ĐỆ QUY: Parser tự gọi lại chính mình để xử lý con
                    Element* childElem = parseElement(childNode);

                    if (childElem) {
                        group->addChild(childElem);
                    }
                }
            }
        }
    }
    return elem;
}

void Parser::loadFromFile(const string source) {
    // Xóa dữ liệu cũ
    for (auto c : children) delete c;
    children.clear();

    xml_document<> doc;
    ifstream file(source);
    if (!file.is_open()) return;

    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    xml_node<>* rootNode = doc.first_node("svg");
    if (!rootNode) return;
    \
    xml_attribute<>* attrViewBox = rootNode->first_attribute("viewBox");
    if (attrViewBox) {
        string vb = attrViewBox->value();
        // Bạn cần parse chuỗi "x y w h" này và lưu vào một biến nào đó
        // Để đơn giản, ta in ra hoặc gán tạm vào Renderer nếu có thể truy cập
        // (Gợi ý: Cần thêm hàm setViewBox cho Parser hoặc Renderer)
    }
    xml_attribute<>* attrWidth = rootNode->first_attribute("width");
    xml_attribute<>* attrHeight = rootNode->first_attribute("height");

    // Duyệt qua các node ở tầng ngoài cùng của SVG
    for (xml_node<>* node = rootNode->first_node(); node; node = node->next_sibling()) {
        // Thay vì gọi createElement trực tiếp, ta gọi qua hàm đệ quy parseElement
        Element* ele = parseElement(node);
        if (ele) {
            children.push_back(ele);
        }
    }
}

const vector<Element*>& Parser::getChildren() {
    return children;
}