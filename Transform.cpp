#include "stdafx.h"
#include "Transform.h"

using namespace Gdiplus;

Transform::Transform() {
    matrix.Reset(); // Khởi tạo ma trận đơn vị
}

void Transform::concatenate(const Gdiplus::Matrix& other) {
    // Nhân ma trận khác (ma trận cha) vào ma trận hiện tại.
    // Dùng MatrixOrderPrepend để đảm bảo các phép biến hình của cha
    // được áp dụng trước các phép biến hình của con.
    matrix.Multiply(&other, MatrixOrderPrepend);
}

std::vector<float> Transform::parseNumbers(const std::string& argsStr) {
    std::vector<float> values;
    std::stringstream ss(argsStr);
    std::string sanitized = argsStr;

    // Thay thế dấu phẩy bằng khoảng trắng
    std::replace(sanitized.begin(), sanitized.end(), ',', ' ');

    std::stringstream clean_ss(sanitized);
    float value;

    // Đọc các giá trị float
    while (clean_ss >> value) {
        values.push_back(value);
    }
    return values;
}

void Transform::parseTransform(const std::string& transformValue) {
    matrix.Reset();

    std::stringstream ss(transformValue);
    std::string token;

    // Chúng ta sẽ tách chuỗi dựa trên dấu ')'
    while (std::getline(ss, token, ')')) {
        if (token.empty() || token.find('(') == std::string::npos) continue;

        size_t open_paren = token.find('(');

        // Lấy tên lệnh (ví dụ: "translate" hoặc "rotate")
        std::string command = token.substr(0, open_paren);
        command.erase(std::remove_if(command.begin(), command.end(), ::isspace), command.end());
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);

        // Lấy các đối số (ví dụ: "-100, 200")
        std::string argsStr = token.substr(open_paren + 1);
        std::vector<float> values = parseNumbers(argsStr);

        Matrix tempMatrix;

        // Thao tác với ma trận tạm thời
        if (command == "translate" && values.size() >= 1) {
            float tx = values[0];
            float ty = (values.size() > 1) ? values[1] : 0.0f;
            tempMatrix.Translate(tx, ty);
        }
        else if (command == "rotate" && values.size() >= 1) {
            float angle = values[0];
            tempMatrix.Rotate(angle);
        }
        else if (command == "scale" && values.size() >= 1) {
            float sx = values[0];
            float sy = (values.size() > 1) ? values[1] : sx;
            tempMatrix.Scale(sx, sy);
        }

        // Áp dụng phép biến hình vào ma trận chính theo thứ tự APPEAND (thứ tự khai báo)
        matrix.Multiply(&tempMatrix, MatrixOrderAppend);
    }
}