#include "Logger.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <chrono>

Logger::Logger() : file_path("logfile.txt"), log_level(0) {}

void Logger::set_file_path(std::string path_to_log_file) {
    this->file_path = path_to_log_file;
    this->log_stream << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;
    this->log_stream << "         NUEVA PARTIDA         " << std::endl;
    this->log_stream << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;

    std::string text = this->log_stream.str();
    this->log_stream.str(std::string());
    this->write(text);
}

Logger& Logger::operator<<(Logger::ManipFn manip) {
    manip(this->log_stream);

    if (manip == static_cast<Logger::ManipFn>(std::endl))
        this->make_header();
    return *this;
}

Logger& Logger::operator()(int loglvl) {
    this->log_level = loglvl;
    return *this;
}

void Logger::write(std::string text) {
    if (!this->log_file.is_open())
        this->log_file.open(Logger::file_path, std::fstream::out | std::fstream::app);
    this->log_file << text << std::endl;
    this->log_file.flush();
}

void Logger::make_header() {

    std::string message;


    std::time_t t = std::chrono::system_clock::to_time_t (
                    std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t),"%Y-%m-%d %X");
    message.append(ss.str());
    
    std::string level;
    if (log_level == 0)
        level = " [ERROR]: ";
    else if (log_level == 1)
        level = " [INFO]: ";
    else level = " [DEBUG]: ";
    
    message.append(level);

    message.append(this->log_stream.str());
    this->log_stream.str(std::string());

    this->write(message);

}

Logger::~Logger() {
    this->log_file.close();
}

