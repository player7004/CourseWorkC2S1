#pragma once

#include <iostream>
#include <QString>
#include <vector>
#include <set>
#include <exception>


const std::set<QString> Types {
    // Объект
    "Object",
    // Продукт
    "Product",
    // Человек
    "Human"
};

const std::set<QString> OTypes{
    // Стена
    "Wall",
    // Пустое место
    "EmptySpace",
    // Стенд
    "Stand",
    // Касса
    "CashBox",
    // Дверь
    "Door",
    // Неопределённый
    "Undefinded"
};

const std::set<QString> PTypes {
    // Молоко
    "Milk",
    // Хлеб
    "Bread",
    // Пиво
    "Beer",
    // Сыр
    "Cheese",
    // Яйца
    "Eggs",
    // Гречка
    "BuckWheat",
    // Туалетка
    "ToiletPaper",
    // Мясо
    "Meet",
    // Рыба
    "Fish",
    // Масло
    "Butter",
    // Помидоры
    "Tomato",
    // Яблоки
    "Apples",
    // Лимоны
    "Lemon",
    // Неопределённый
    "Undefined"
};

class Product {
public:
    // Тип объекта
    QString Type{"Product"};
    // Цена
    ushort Price;
    // Привлекательность
    float Attractiveness;
    // Тип продукта
    QString PType;
    // Название
    QString Name;
    // Конструктор
    Product();
    // Операторы тождественного равенства
    friend bool operator==(const Product& first, const Product& second);
    friend bool operator==(Product& first, Product& second);
};

class Object {
public:
    // Тип объекта
    QString Type{"Object"};
    // Позиция объекта
    std::pair<ushort, ushort> Position;
    // Размер объекта
    std::pair<ushort, ushort> Size;
    // Имя объекта
    QString Name;
    // Тип объекта
    QString OType;
    // Символ объекта
    QChar Symbol;
    // Содержимое
    std::vector<Product> Content;
    // Конструктор
    Object();
};

class Human {
public:
    // Тип объекта
    QString Type{"Human"};
    // Имя человека
    QString Name;
    // Символ человека
    QChar Symbol;
    // Путь человека
    std::vector<std::pair<ushort, ushort>> Way;
    // Список покупок
    std::vector<Product> ToBuyList;
    // Взятые продукты
    std::vector<Product> TakenProducts;
    // Потарченые деньги
    ushort Money;
    // Конструктор
    Human();
};

namespace std {
    std::string to_string(Object val);
    std::string to_string(Product val);
    std::string to_string(Human val);
}