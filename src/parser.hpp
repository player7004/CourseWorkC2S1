#pragma once

#include "entity.hpp"
#include "dist/json/json.h"
#include <QString>
#include <vector>
#include <fstream>
#include <set>

class Parser {
private:
    // Лог
    Log log;
    // Очищает данные парсера
    void clear();
    // Возвращает продукт
    bool getProduct(const Json::Value& val, Product& res);
    // Возвращает объект
    bool getObject(const Json::Value& val, Object& res);
    // Возвращает человека
    bool getHuman(const Json::Value& val, Human& res);
public:
    // Все имющиеся объекты
    std::vector<Object> AllObjects;
    // Все имеющиеся люди
    std::vector<Human> AllHumans;
    // Парсит файл
    bool parse(const QString& file);
    // Конструктор
    Parser();
};