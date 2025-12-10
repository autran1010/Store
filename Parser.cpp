#include "stdafx.h"
#include "Parser.h"

Parser::Parser() {
    source = "";
}

Parser::~Parser() {
    for (int i = 0; i < children.size(); i++)
        delete children[i];
    children.clear();
}

Element* Parser::createElement(const string& name) {
    if (name == "circle") return new Circle();
    else if (name == "line") return new Line();
    else if (name == "text") return new Text();
    else if (name == "rect") return new rectangle();
    else if (name == "polyline") return new polyline();
    else if (name == "polygon") return new polygon();
    else if (name == "ellipse") return new ellipse();
    else if (name == "path") return new Path(); // Mới
    else if (name == "g") return new Group();   // Mới
    else return nullptr;
}

// Thay thế toàn bộ hàm parseRecursive cũ bằng hàm này
void Parser::parseRecursive(xml_node<>* parentNode, vector<Element*>& container, Element* parentStyle) {
    for (xml_node<>* node = parentNode->first_node(); node; node = node->next_sibling()) {
        string ele_name = node->name();
        Element* ele = createElement(ele_name);

        if (ele) {
            // 1. Parse thuộc tính của chính nó trước
            ele->parseAttributes(node);

            // 2. LOGIC KẾ THỪA (Inheritance)
            if (parentStyle) {
                // Kiểm tra Stroke: Nếu con không có độ dày viền, thử lấy của cha
                if (ele->getStroke().getStrokeWidth() == 0) {
                    Stroke parentStroke = parentStyle->getStroke();
                    // Chỉ copy nếu cha thực sự có viền (độ dày > 0)
                    if (parentStroke.getStrokeWidth() > 0) {
                        ele->setStroke(parentStroke);
                    }
                }

                // Kiểm tra Fill: Tương tự (nếu cần)
                // Lưu ý: Mặc định fill thường là đen nên cần cẩn thận hơn, 
                // nhưng với stroke logic trên là đủ cho bài này.
            }

            // 3. Xử lý Group đệ quy
            if (ele_name == "g") {
                Group* group = dynamic_cast<Group*>(ele);
                if (group) {
                    vector<Element*> groupChildren;

                    // QUAN TRỌNG: Truyền 'group' hiện tại làm 'parentStyle' cho các con
                    parseRecursive(node, groupChildren, group);

                    for (auto child : groupChildren) {
                        group->addChild(child);
                    }
                }
            }

            container.push_back(ele);
        }
    }
}

void Parser::loadFromFile(const string source) {
    // Đọc file vào buffer
    xml_document<> doc;
    ifstream file(source);
    if (!file.is_open()) return;

    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    // Parse XML
    try {
        doc.parse<0>(&buffer[0]);
    }
    catch (...) {
        return;
    }

    xml_node<>* rootNode = doc.first_node("svg");
    if (!rootNode) return;

    // Bắt đầu đệ quy từ root, kết quả lưu vào biến thành viên children
    parseRecursive(rootNode, this->children);
}

const vector<Element*>& Parser::getChildren() {
    return children;
}