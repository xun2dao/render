#include"MouseEvent.h"
#include<sstream>

std::string MouseButtonEvent::ToString() const{
    std::stringstream ss;
    ss << "Mouse Button Event : ["<< m_button<<"]";
    return ss.str();   
}

std::string MouseScrollEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseScroll  [x, y] : "<<m_x << ", "<<m_y;
        return ss.str();
}

 std::string MouseCursorPos::ToString() const {
        std::stringstream ss;
        ss << "CursorPos  [x, y] : "<<m_x << ", "<<m_y;
        return ss.str();
}