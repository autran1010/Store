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
    else if (name == "path") return new Path();
    else if (name == "g") return new Group();
    else return nullptr;
}

void Parser::parseRecursive(xml_node<>* parentNode, vector<Element*>& container, Element* parentStyle) {
    for (xml_node<>* node = parentNode->first_node(); node; node = node->next_sibling()) {
        string ele_name = node->name();
        Element* ele = createElement(ele_name);

        if (ele) {

            ele->parseAttributes(node);
            if (ele_name == "g") {
                Group* group = dynamic_cast<Group*>(ele);
                if (group) {
                    vector<Element*> groupChildren;


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
    xml_document<> doc;
    ifstream file(source);
    if (!file.is_open()) return;

    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    try {
        doc.parse<0>(&buffer[0]);
    }
    catch (...) {
        return;
    }

    xml_node<>* rootNode = doc.first_node("svg");
    if (!rootNode) return;

    parseRecursive(rootNode, this->children);
}

const vector<Element*>& Parser::getChildren() {
    return children;
}