#include"MouseEvent.h"
#include<sstream>

std::string MouseButtonEvent::ToString() const{
    std::stringstream ss;
    ss << "Mouse Button Event : ["<< m_button<<"]";
    return ss.str();   
}
