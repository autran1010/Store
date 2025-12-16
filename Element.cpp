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

    int rcol = 0, gcol = 0, bcol = 0;
    float acol = 0.0f;
    if (value.rfind("rgba", 0) == 0) {

        if (sscanf_s(value.c_str(), "rgba( %d , %d , %d , %f )", &rcol, &gcol, &bcol, &acol) == 4) {
            if (rcol > 255) rcol = 255; else if (rcol < 0) rcol = 0;
            if (gcol > 255) gcol = 255; else if (gcol < 0) gcol = 0;
            if (bcol > 255) bcol = 255; else if (bcol < 0) bcol = 0;
            if (acol > 1.0f) acol = 1.0f; else if (acol < 0.0f) acol = 0.0f;

            return color(rcol / 255.0f, gcol / 255.0f, bcol / 255.0f, acol);
        }
    }


    if (value.rfind("rgb", 0) == 0) {
        if (sscanf_s(value.c_str(), "rgb( %d , %d , %d )", &rcol, &gcol, &bcol) == 3) {
            if (rcol > 255) rcol = 255; else if (rcol < 0) rcol = 0;
            if (gcol > 255) gcol = 255; else if (gcol < 0) gcol = 0;
            if (bcol > 255) bcol = 255; else if (bcol < 0) bcol = 0;

            return color(rcol / 255.0f, gcol / 255.0f, bcol / 255.0f, 1.0f);
        }
    }
    return color(0, 0, 0, 0);
}
void Element::parse(string name, string value) {
    if (name == "fill") {
        if (value == "none") {
            fill = color(0, 0, 0, 0);
        }
        else {
            color newFill = apply(value);
            if (fill.getOpacity() > 0 && newFill.getOpacity() == 1.0f) {
                newFill.setOpacity(fill.getOpacity());
            }
            fill = newFill;
        }
        isFillSet = true;
    }
    else if (name == "stroke") {
        if (value == "none") {
            color transparent(0, 0, 0, 0.0f);
            stroke.setStrokeColor(transparent);
        }
        else {
            color newStrokeColor = apply(value);
            if (isStrokeOpacitySet) {
                newStrokeColor.setOpacity(stroke.getStrokeColor().getOpacity());
            }

            stroke.setStrokeColor(newStrokeColor);
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
    else if (name == "stroke-linecap") {
        stroke.setLineCap(value);
    }
    else if (name == "stroke-linejoin") {
        stroke.setLineJoin(value);
    }
    else if (name == "stroke-miterlimit") {
        try {
            stroke.setMiterLimit(std::stof(value));
        }
        catch (...) {}
    }
    else if (name == "fill-rule") {
        fillRule = value;
        isFillRuleSet = true;
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

Gdiplus::Pen* Element::createPenFromStroke() {
    if (stroke.getStrokeWidth() <= 0.0f || stroke.getStrokeColor().getOpacity() <= 0.001f) {
        return nullptr;
    }
    color c = stroke.getStrokeColor();
    Gdiplus::Color gdiColor(
        (BYTE)(c.getOpacity() * 255),
        (BYTE)(c.getR() * 255),
        (BYTE)(c.getG() * 255),
        (BYTE)(c.getB() * 255)
    );
    Gdiplus::Pen* pen = new Gdiplus::Pen(gdiColor, stroke.getStrokeWidth());
    std::string cap = stroke.getLineCap();
    if (cap == "round") {
        pen->SetStartCap(Gdiplus::LineCapRound);
        pen->SetEndCap(Gdiplus::LineCapRound);
    }
    else if (cap == "square") {
        pen->SetStartCap(Gdiplus::LineCapSquare);
        pen->SetEndCap(Gdiplus::LineCapSquare);
    }
    else {
        pen->SetStartCap(Gdiplus::LineCapFlat);
        pen->SetEndCap(Gdiplus::LineCapFlat);
    }
    std::string join = stroke.getLineJoin();
    if (join == "round") {
        pen->SetLineJoin(Gdiplus::LineJoinRound);
    }
    else if (join == "bevel") {
        pen->SetLineJoin(Gdiplus::LineJoinBevel);
    }
    else {
        pen->SetLineJoin(Gdiplus::LineJoinMiter);
        pen->SetMiterLimit(stroke.getMiterLimit());
    }

    std::vector<float> dashes = stroke.getDashArray();
    if (!dashes.empty()) {
        if (dashes.size() % 2 != 0) {
            dashes.insert(dashes.end(), dashes.begin(), dashes.end());
        }

        int count = dashes.size();
        Gdiplus::REAL* gdiDashes = new Gdiplus::REAL[count];
        float w = stroke.getStrokeWidth();

        for (int i = 0; i < count; i++) {
            gdiDashes[i] = (w > 0) ? (Gdiplus::REAL)(dashes[i] / w) : 0;
        }

        pen->SetDashStyle(Gdiplus::DashStyleCustom);
        pen->SetDashPattern(gdiDashes, count);

        if (w > 0) pen->SetDashOffset(stroke.getDashOffset() / w);

        delete[] gdiDashes;
    }

    return pen;
}
void Element::inheritFrom(Element* parent) {
    if (!parent) return;
    bool inheritedFillColor = false;
    bool inheritedStrokeColor = false;
    if (!isFillSet && parent->isFillSet) {
        fill = parent->fill;
        isFillSet = true;
        inheritedFillColor = true;
    }

    if (!isFillOpacitySet && parent->isFillOpacitySet) {
        if (!inheritedFillColor) {
            fill.setOpacity(fill.getOpacity() * parent->fill.getOpacity());
        }
        isFillOpacitySet = true;
    }

    if (!isStrokeSet && parent->isStrokeSet) {
        stroke.setStrokeColor(parent->stroke.getStrokeColor());
        isStrokeSet = true;
        inheritedStrokeColor = true;
    }

    if (!isStrokeOpacitySet && parent->isStrokeOpacitySet) {
        if (!inheritedStrokeColor) {
            color c = stroke.getStrokeColor();
            float parentOpacity = parent->stroke.getStrokeColor().getOpacity();
            c.setOpacity(c.getOpacity() * parentOpacity);
            stroke.setStrokeColor(c);
        }
        isStrokeOpacitySet = true;
    }

    // Sửa lại: Kế thừa nếu cha có set cờ HOẶC cha có độ dày viền thực tế > 0
    if (!isStrokeWidthSet && (parent->isStrokeWidthSet || parent->stroke.getStrokeWidth() > 0.0f)) {
        stroke.setStrokeWidth(parent->stroke.getStrokeWidth());
        // Ta không nhất thiết phải set isStrokeWidthSet = true nếu đó là giá trị ngầm định,
        // nhưng để an toàn cho các đời con cháu tiếp theo, ta có thể set nó.
        isStrokeWidthSet = true;
    }

    if (!isFillRuleSet && parent->isFillRuleSet) {
        fillRule = parent->fillRule;
        isFillRuleSet = true;
    }
}