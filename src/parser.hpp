#pragma once

#include "entity.hpp"
#include <json/json.h>
#include <string>
#include <vector>
#include <ostream>
#include <fstream>

class Parser {
private:
    // Имя файла для чтения
    std::string FileName;
    // Люди
    std::vector<Human> Humans;
    // Объекты
    std::vector<Object> AllObjects;
    // Возвращает тип сущности
    static Entities getEntityType(const std::string& type);
    // Возращает тип объекта
    static Objects getObjectType(const std::string& otype);
    // Возращает символ объекта
    static char getObjectSymbol(const Objects& otype);
    // Возвращает список продуктов
    static std::vector<Product> getProducts(const Json::Value& val);
    // Возвращает тип продукта
    static Products getProductType(const std::string& ptype);
    // Возвращает путь
    std::vector<std::pair<unsigned short, unsigned short>> getWay(const Json::Value& way);
public:
    // Базовый конструктор
    // Конструктор с именем файла
    explicit Parser(const std::string& filename="Shop.json");
    // Возвращает людей
    std::vector<Human> getHumans() const;
    // Возвращает
    std::vector<Object> getObjects() const;
    // Читает иформацию с файла
    // Создает Humans
    // Создает Objects
    bool parse();
    // Печатает все имеющиеся объекты
    // Печатает всех людей
    friend std::ostream& operator<<(std::ostream& stream, const Parser& parser);
};