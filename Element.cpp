#include "stdafx.h"
#include "Element.h"
std::unordered_map<std::string, color> loadColorMapInternal() {
    std::unordered_map<std::string, color> colorMap;
    std::ifstream color_file("Color.txt", std::ios::in);

    if (!color_file.is_open()) {
        
        colorMap["none"] = color(0, 0, 0, 0);
        return colorMap;
    }

    
    std::string color_line;
    while (getline(color_file, color_line)) {
        std::stringstream ss(color_line);
        std::vector<std::string> vct;
        std::string token;
        while (ss >> token) vct.push_back(token);

        if (vct.size() < 2) continue;

        std::string hexa_code = vct.back();
        std::string color_name = "";
        for (size_t i = 0; i < vct.size() - 1; ++i) {
            if (i > 0) color_name += " ";
            color_name += vct[i];
        }

        std::transform(color_name.begin(), color_name.end(), color_name.begin(), ::tolower);

        if (hexa_code.length() == 7 && hexa_code[0] == '#') {
            try {
               
                unsigned long hexValue = std::stoul(hexa_code.substr(1), nullptr, 16);
                float r_f = (float)((hexValue >> 16) & 0xFF) / 255.0f;
                float g_f = (float)((hexValue >> 8) & 0xFF) / 255.0f;
                float b_f = (float)(hexValue & 0xFF) / 255.0f;

                colorMap[color_name] = color(r_f, g_f, b_f, 1.0f);
            }
            catch (const std::exception&) {
                
            }
        }
    }
    colorMap["none"] = color(0, 0, 0, 0);
    return colorMap;
}

color Element::apply(string value) {
    static const std::unordered_map<std::string, color> colorMap = loadColorMapInternal();

    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    if (colorMap.count(value)) {
        return colorMap.at(value);
    }

    if (value.rfind("#", 0) == 0) {
        if (value.length() == 7 || value.length() == 4) {
            try {
                unsigned long hexValue = std::stoul(value.substr(1), nullptr, 16);

                float r_f, g_f, b_f;

                if (value.length() == 4) {
                    int r = (int)((hexValue >> 8) & 0xF);
                    int g = (int)((hexValue >> 4) & 0xF);
                    int b = (int)((hexValue >> 0) & 0xF);

                    r_f = (float)((r << 4) | r) / 255.0f;
                    g_f = (float)((g << 4) | g) / 255.0f;
                    b_f = (float)((b << 4) | b) / 255.0f;
                }
                else {
                    r_f = (float)((hexValue >> 16) & 0xFF) / 255.0f;
                    g_f = (float)((hexValue >> 8) & 0xFF) / 255.0f;
                    b_f = (float)(hexValue & 0xFF) / 255.0f;
                }

                return color(r_f, g_f, b_f, 1.0f);
            }
            catch (const std::exception&) {
            }
        }
    }

    if (value.rfind("rgb", 0) == 0) {
        int rcol = 0, gcol = 0, bcol = 0;
        if (sscanf_s(value.c_str(), "rgb(%d,%d,%d)", &rcol, &gcol, &bcol) == 3) {
            return color(rcol / 255.0f, gcol / 255.0f, bcol / 255.0f, 1.0f);
        }
    }
    return color(0, 0, 0, 0);
}

void Element::parse(string name, string value) {
    if (name == "fill") {
        if (value == "none") {
            fill = color(0, 0, 0, 0); // Trong suốt
        }
        else {
            color newFill = apply(value);
            // Giữ lại opacity cũ nếu đã có (vì fill="..." chỉ set màu)
            if (fill.getOpacity() > 0 && newFill.getOpacity() == 1.0f) {
                newFill.setOpacity(fill.getOpacity());
            }
            fill = newFill;
        }
        isFillSet = true;
    }
    else if (name == "stroke") {
        if (value == "none") {
            // Trường hợp stroke="none"
            color transparent(0, 0, 0, 0.0f);
            stroke.setStrokeColor(transparent);
            // Không set stroke width = 1 ở đây
        }
        else {
            // Trường hợp có màu
            color newStrokeColor = apply(value);

            // Giữ opacity cũ nếu đã set riêng (stroke-opacity)
            if (isStrokeOpacitySet) {
                newStrokeColor.setOpacity(stroke.getStrokeColor().getOpacity());
            }

            stroke.setStrokeColor(newStrokeColor);

            // QUAN TRỌNG: Chỉ set width mặc định = 1 nếu nó chưa được set và màu KHÔNG phải none
            if (stroke.getStrokeWidth() == 0 && !isStrokeWidthSet) {
                stroke.setStrokeWidth(1.0f);
            }
        }
        isStrokeSet = true;
    }
    else if (name == "stroke-width") {
        try {
            stroke.setStrokeWidth(stof(value));
            isStrokeWidthSet = true;
        }
        catch (...) {}
    }
    else if (name == "stroke-opacity") {
        try {
            float opacity = stof(value);
            color c = stroke.getStrokeColor();
            c.setOpacity(opacity);
            stroke.setStrokeColor(c);
            isStrokeOpacitySet = true;
        }
        catch (...) {}
    }
    else if (name == "fill-opacity") {
        try {
            float opacity = stof(value);
            color c = fill;
            c.setOpacity(opacity);
            fill = c;
            isFillOpacitySet = true;
        }
        catch (...) {}
    }
    else if (name == "transform") {
        this->transform.parse(value);
    }
}

// Logic kế thừa quan trọng
void Element::inheritFrom(Element* parent) {
    if (!parent) return;

    if (!isFillSet && parent->isFillSet) {
        this->fill = parent->fill;
        // Nếu con chưa set opacity, lấy luôn opacity của cha nằm trong biến fill
        if (!isFillOpacitySet) {
            // fill của cha đã chứa opacity rồi
        }
        isFillSet = true;
    }
    // Nếu con có set fill nhưng chưa set opacity, và cha có set opacity
    if (isFillSet && !isFillOpacitySet && parent->isFillOpacitySet) {
        this->fill.setOpacity(this->fill.getOpacity() * parent->fill.getOpacity());
    }

    if (!isStrokeSet && parent->isStrokeSet) {
        this->stroke.setStrokeColor(parent->stroke.getStrokeColor());
        isStrokeSet = true;
    }

    if (!isStrokeWidthSet && parent->isStrokeWidthSet) {
        this->stroke.setStrokeWidth(parent->stroke.getStrokeWidth());
        isStrokeWidthSet = true;
    }

    // Kế thừa opacity cho stroke
    if (isStrokeSet && !isStrokeOpacitySet && parent->isStrokeOpacitySet) {
        color c = this->stroke.getStrokeColor();
        c.setOpacity(c.getOpacity() * parent->stroke.getStrokeColor().getOpacity());
        this->stroke.setStrokeColor(c);
    }
}