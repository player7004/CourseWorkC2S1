#pragma once

#include "entity.hpp"
#include "dist/json/json.h"
#include <QString>
#include <vector>
#include <fstream>
#include <set>

class Parser {
private:
    // Очищает данные парсера
    void clear();
    // Возвращает продукт
    static bool getProduct(const Json::Value& val, Product& res);
    // Возвращает объект
    static bool getObject(const Json::Value& val, Object& res);
    // Возвращает человека
    static bool getHuman(const Json::Value& val, Human& res);
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

class Saver {
private:
    // Сохраняет продукт
    void static saveProduct(Json::Value& val, const Product& res);
    // Сохраняет объект
    void static saveObject(Json::Value& val, const Object& res);
    // Сохраняет человека
    void static saveHuman(Json::Value& val, const Human& res);
public:
    static bool save(const QString &filename, const std::vector<Human>& AllHumans, const std::vector<Object>& AllObjects);
};