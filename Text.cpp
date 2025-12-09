#include "stdafx.h"
#include "Text.h"
wstring s2ws(const std::string& str) {
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    return converter.from_bytes(str);
}
void Text::parseAttributes(xml_node<>* node) {
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        std::string name = attr->name();
        std::string value = attr->value();

        if (name == "x") {
            try { position.setX(stof(value)); }
            catch (const std::invalid_argument&) {}
        }
        else if (name == "y") {
            try { position.setY(stof(value)); }
            catch (const std::invalid_argument&) {}
        }
        else if (name == "font-size") {
            try { fontsize = stof(value); }
            catch (const std::invalid_argument&) {}
        }
        else if (name == "font-family") {
            font = value;
        }
        else if (name == "dx") {
            try { this->dx = stof(value); }
            catch (const std::invalid_argument&) {}
        }
        else if (name == "dy") {
            try { this->dy = stof(value); }
            catch (const std::invalid_argument&) {}
        }
        else if (name == "text-anchor") {
            textAnchor = value;
        }
        else if (name == "font-style") {
            fontStyle = value;
        }
        else if (name == "font-weight") {
            fontWeight = value;
        }
        else {
            parse(name, value);
        }
    }
    xml_node<>* dataNode = node->first_node();
    if (dataNode && dataNode->type() == rapidxml::node_data) {
        content = dataNode->value();
    }
}

void Text::Draw(Graphics* graphics) {
    if (content.empty()) return;

  
    std::wstring wFontFamily = font.empty() ? L"Times New Roman" : s2ws(font);
    FontFamily gdiFontFamily(wFontFamily.c_str());

    FontStyle style = FontStyleRegular;
    std::string lowerStyle = fontStyle;
    std::transform(lowerStyle.begin(), lowerStyle.end(), lowerStyle.begin(), ::tolower);
    std::string lowerWeight = fontWeight;
    std::transform(lowerWeight.begin(), lowerWeight.end(), lowerWeight.begin(), ::tolower);

    if (lowerWeight.find("bold") != std::string::npos) style = (FontStyle)(style | FontStyleBold);
    if (lowerStyle.find("italic") != std::string::npos) style = (FontStyle)(style | FontStyleItalic);

   
    float finalFontSize = (fontsize > 0.0f) ? fontsize : 12.0f;
    Font gdiFont(&gdiFontFamily, finalFontSize, style, UnitPixel);

    
    REAL cellAscent = (REAL)gdiFontFamily.GetCellAscent(style);
    REAL emHeight = (REAL)gdiFontFamily.GetEmHeight(style); 
    REAL baselineOffset = finalFontSize * cellAscent / emHeight;

    
    PointF origin(position.getX() + dx, position.getY() + dy - baselineOffset); 

 
    StringFormat format;
    format.SetLineAlignment(StringAlignmentNear);

    std::string lowerAnchor = textAnchor;
    std::transform(lowerAnchor.begin(), lowerAnchor.end(), lowerAnchor.begin(), ::tolower);
    if (lowerAnchor == "middle") format.SetAlignment(StringAlignmentCenter);
    else if (lowerAnchor == "end") format.SetAlignment(StringAlignmentFar);
    else format.SetAlignment(StringAlignmentNear);

   
    std::wstring wcontent = s2ws(content);

    GraphicsPath path(FillModeWinding);
    path.AddString(
        wcontent.c_str(),
        -1,
        &gdiFontFamily,
        style,
        finalFontSize,
        origin,
        &format
    );

    if (stroke.getStrokeColor().getOpacity() > 0.0f && stroke.getStrokeWidth() > 0.0f) {
        Color strokeColor(
            (BYTE)(stroke.getStrokeColor().getOpacity() * 255.0f),
            (BYTE)(stroke.getStrokeColor().getR() * 255.0f),
            (BYTE)(stroke.getStrokeColor().getG() * 255.0f),
            (BYTE)(stroke.getStrokeColor().getB() * 255.0f)
        );
        Pen pen(strokeColor, stroke.getStrokeWidth());
        graphics->DrawPath(&pen, &path);
    }

    if (fill.getOpacity() > 0.0f) {
        Color fillColor(
            (BYTE)(fill.getOpacity() * 255.0f),
            (BYTE)(fill.getR() * 255.0f),
            (BYTE)(fill.getG() * 255.0f),
            (BYTE)(fill.getB() * 255.0f)
        );
        SolidBrush brush(fillColor);
        graphics->FillPath(&brush, &path);
    }
}