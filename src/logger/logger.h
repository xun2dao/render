#pragma once
#include<spdlog/sinks/stdout_color_sinks.h>
class Logger{
public:
Logger(){
    m_logger = spdlog::stdout_color_mt("console");
}
private:
std::shared_ptr<spdlog::logger> m_logger;
};