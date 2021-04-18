#include "log.hpp"

Log::Log() {
    if (!FS::exists("logs")) {
        FS::create_directory("logs");
    } else {
        if (!FS::is_directory("Logs")) {
            FS::create_directory("logs");
        }
    }
    Path = FS::current_path().string() + "/logs/";
}

bool Log::open(const QString& file) {
    if (!is_open()) {
        File.open(Path.string() + file.toStdString(), std::ios_base::app);
        if (File.is_open()) {
            time_t now = time(0);
            std::string res = ctime(&now); 
            File << "Log started at: " << res << std::endl;
            File.flush();
        } else {
            return false;
        }
        return true;
    }
    return false;
}

bool Log::is_open() {
    return File.is_open();
}

void Log::write(const QString& text, WriteTypes Type) {
    if (PrefixFlag) {
        File << Prefix;
    }
    switch (Type)
    {
    case WriteTypes::Error:
        File << "An error raised while programm was running: " << text.toStdString() << std::endl;
        break;
    case WriteTypes::Normal:
        File << text.toStdString() << std::endl;
        break;
    case WriteTypes::Info:
        File << "Something happened while programm was running: " << text.toStdString() <<std::endl;
        break;
    default:
        File << "Log was called with wrong mode." << std::endl;
        break;
    }
}

void Log::flush() {
    if (File.is_open()) {
        File.flush();
    }
}

Log::~Log() {
    time_t now = time(0);
    std::string res = ctime(&now);
    if (File.is_open()) {
        File << "Log stopped at: " << res << std::endl;
    }
    File.close();
}