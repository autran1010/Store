#include "stdafx.h"
#include "Text.h"
#include <algorithm> // Thêm thư viện này để dùng std::replace

wstring s2ws(const std::string& str) {
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    return converter.from_bytes(str);
}

// Hàm chuẩn hóa chuỗi theo chuẩn SVG
std::string normalizeText(std::string text) {
    // 1. Thay thế các ký tự điều khiển (xuống dòng, tab) thành khoảng trắng
    std::replace(text.begin(), text.end(), '\n', ' ');
    std::replace(text.begin(), text.end(), '\r', ' ');
    std::replace(text.begin(), text.end(), '\t', ' ');

    // 2. Xóa khoảng trắng thừa (nhiều khoảng trắng liên tiếp -> 1 khoảng trắng)
    std::string cleanText;
    bool lastWasSpace = false;

    // Bỏ qua khoảng trắng đầu chuỗi
    size_t start = 0;
    while (start < text.length() && text[start] == ' ') start++;

    for (size_t i = start; i < text.length(); ++i) {
        if (text[i] == ' ') {
            if (!lastWasSpace) {
                cleanText += ' ';
                lastWasSpace = true;
            }
        }
        else {
            cleanText += text[i];
            lastWasSpace = false;
        }
    }

    // Bỏ khoảng trắng cuối chuỗi nếu có
    if (!cleanText.empty() && cleanText.back() == ' ') {
        cleanText.pop_back();
    }

    return cleanText;
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
            font = value.substr(0, value.find(',')); // lấy font đầu tiên
            font.erase(remove(font.begin(), font.end(), '"'), font.end());
            font.erase(remove(font.begin(), font.end(), '\''), font.end());
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

    // --- SỬA LỖI XUỐNG DÒNG ---
    xml_node<>* dataNode = node->first_node();
    if (dataNode && dataNode->type() == rapidxml::node_data) {
        std::string rawContent = dataNode->value();
        content = normalizeText(rawContent); // Gọi hàm chuẩn hóa
    }
}

void Text::Draw(Graphics* graphics) {
    if (content.empty()) return;

    GraphicsState state = graphics->Save();
    graphics->MultiplyTransform(this->transform.getMatrix(), MatrixOrderPrepend);

    graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

    // XỬ LÝ FONT
    std::wstring wFontFamily = font.empty() ? L"Times New Roman" : s2ws(font);
    FontFamily* pFontFamily = new FontFamily(wFontFamily.c_str());

    if (pFontFamily->GetLastStatus() != Ok) {
        delete pFontFamily;
        pFontFamily = new FontFamily(L"Times New Roman");
    }

    // XỬ LÝ STYLE
    FontStyle style = FontStyleRegular;
    std::string lowerStyle = fontStyle;
    std::transform(lowerStyle.begin(), lowerStyle.end(), lowerStyle.begin(), ::tolower);
    std::string lowerWeight = fontWeight;
    std::transform(lowerWeight.begin(), lowerWeight.end(), lowerWeight.begin(), ::tolower);

    if (lowerWeight.find("bold") != std::string::npos) style = (FontStyle)(style | FontStyleBold);
    if (lowerStyle.find("italic") != std::string::npos) style = (FontStyle)(style | FontStyleItalic);

    if (!pFontFamily->IsStyleAvailable(style)) {
        style = FontStyleRegular;
    }

    // TÍNH TOÁN BASELINE
    float finalFontSize = (fontsize > 0.0f) ? fontsize : 12.0f;

    REAL cellAscent = (REAL)pFontFamily->GetCellAscent(style);
    REAL emHeight = (REAL)pFontFamily->GetEmHeight(style);
    REAL baselineOffset = finalFontSize * cellAscent / emHeight;
    PointF origin(position.getX() + dx, position.getY() + dy - baselineOffset);

    // XỬ LÝ ALIGNMENT
    StringFormat format;
    format.SetLineAlignment(StringAlignmentNear);
    std::string lowerAnchor = textAnchor;
    std::transform(lowerAnchor.begin(), lowerAnchor.end(), lowerAnchor.begin(), ::tolower);
    if (lowerAnchor == "middle") format.SetAlignment(StringAlignmentCenter);
    else if (lowerAnchor == "end") format.SetAlignment(StringAlignmentFar);
    else format.SetAlignment(StringAlignmentNear);

    std::wstring wcontent = s2ws(content);
    GraphicsPath path(FillModeWinding);
    path.AddString(wcontent.c_str(), -1, pFontFamily, style, finalFontSize, origin, &format);

    // VẼ FILL (Màu bên trong)
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

    // VẼ STROKE (Viền ngoài)
    // Lưu ý: Text SVG kế thừa stroke từ cha. Nếu cha set stroke="navy", text sẽ có viền navy.
    // Nếu bạn không muốn text có viền, bạn phải set stroke="none" cho thẻ text trong SVG
    // hoặc kiểm tra Color.txt đã có màu "navy" chưa.
    Gdiplus::Pen* pen = this->createPenFromStroke();

    if (pen != nullptr) {
        graphics->DrawPath(pen, &path);
        delete pen;
    }

    delete pFontFamily;
    graphics->Restore(state);
}