#pragma once

#include <QString>
#include <fstream>
#include <ctime>
#include <boost/filesystem.hpp>
#include <iostream>

#define FS boost::filesystem

enum class WriteTypes {
    // Ошибка
    // Останавливает работу программы
    Error,
    // Обычный лог
    // Простое сообщение с текстом
    Normal,
    // Информация
    // Ошибка не останавливающая ошибку программы
    Info,
};

class Log {
private:
    // Файл с которым сейчас работаем
    std::ofstream File;
    // Путь к логам
    FS::path Path = FS::current_path();
    // Префикс для красивого вывода
    char Prefix = '\t';
    // Добавлять префикс или нет
    bool PrefixFlag = false;
public:
    // Конструктор
    Log();
    // Работает ли лог или нет
    bool is_open();
    // Записывает текст в лог
    void write(const QString& text, WriteTypes Type = WriteTypes::Normal);
    // записывает данные в файл, но не закрывает его
    void flush();
    // Открыть файл
    bool open(const QString& file);
    // Функция, которая меняет значение постановки префикса
    void inline switchPrefix() {
        PrefixFlag = !PrefixFlag;
    };
    // Деструктор
    ~Log();
};