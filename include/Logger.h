#ifndef BALLESTRINQUE_LOGGER_H
#define BALLESTRINQUE_LOGGER_H

#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>


class Logger {

    private:

        std::string file_path;

        int log_level;

        std::stringstream log_stream;

        std::fstream log_file;

    public:
    
        void set_file_path(std::string path_to_log_file);

        //basicamente el tipo correspondiente a std::endl
        typedef std::ostream&  (*ManipFn)(std::ostream&);

        Logger();

        template<class T>  // int, strings, floats
        Logger& operator<<(const T& output);

        Logger& operator<<(ManipFn manip);

        Logger& operator()(int loglvl);

        ~Logger();

    private:

        void write(std::string text);

        void make_header();


};

template<class T>
Logger& Logger::operator<<(const T& output) {
    this->log_stream << output;
    return *this;
}

#endif