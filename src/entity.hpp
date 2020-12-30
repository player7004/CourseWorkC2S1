#pragma once

#include <iostream>
#include <string>
#include <vector>

enum class Entities
{
    // Объект
    Object,
    // Продукт
    Product,
};

// Типы объектов
enum class Objects
{
    // Стена
    Wall,
    // Проход
    EmptySpace,
    // Стенд с продуктами
    Stand,
    // Касса
    CashBox,
    // Человек
    Human,
    // Дверь
    Door
};

// Типы Продуктов
enum class Products
{
    // Молоко
    Milk,
    // Хлеб
    Bread,
    // Пиво
    Beer,
    // Сыр
    Cheese,
    // Яйца
    Eggs,
    // Гречка
    BuckWheat,
    // Туалетка
    ToiletPaper,
    // Мясо
    Meet,
    // Рыба
    Fish,
    // Масло
    Butter,
    // Помидоры
    Tomato,
    // Яблоки
    Apples,
    // Лимоны
    Lemon
};

class Product {
protected:
    // Тип сущности
    Entities Type;
    // Тип продукта
    Products PType;
    // Цена
    unsigned short Price;
    // Привлекательность
    float Attractiveness;
public:
    explicit Product(
        const Products& ptype = Products::Lemon,
        unsigned short price = 100,
        float attractiveness = 1);
    Products getPType() const;
    unsigned short getPrice() const;
    float getAttractiveness() const;
};

class Object {
    protected:
    // Тип сущности
    Entities Type;
    // Положение
    std::pair<unsigned short, unsigned short> Position;
    // Размер
    std::pair<unsigned short, unsigned short> Size;
    // Тип объекта
    Objects OType;
    // Символ
    char Symbol;
    // Внутренности
    std::vector<Product>* Content;
    // Кол-во денег
    unsigned int Cash;
    public:
    explicit Object(const std::pair<unsigned short, unsigned short>& position=std::pair<unsigned short, unsigned short>{0, 0},
                    const std::pair<unsigned short, unsigned short>& size=std::pair<unsigned short, unsigned short>{1, 1},
                    const Objects& otype=Objects::Wall,
                    const char& symbol = 'W',
                    std::vector<Product>* content = nullptr);
    std::pair<unsigned short, unsigned short> getPosition() const;
    std::pair<unsigned short, unsigned short> getSize() const;
    Objects getOType() const;
    char getSymbol() const;
};

class Human: public Object {
private:
    std::string Name;
    std::vector<Product>* ToBuyList;
    std::vector<Product>* TakenProducts;
    std::vector<std::pair<unsigned short, unsigned short>>* Way;
public:
    Human(
        const std::string name,
        const std::pair<unsigned short, unsigned short>& position,
        const std::pair<unsigned short, unsigned short>& size,
        std::vector<Product>* tobuylist = nullptr,
        std::vector<Product>* takenproducts = nullptr,
        std::vector<std::pair<unsigned short, unsigned short>>* way = nullptr
    );
    const std::vector<Product>* getToBuyList() const;
    const std::vector<Product>* getTakenProducts() const;
    void updateTakenProducts(const Product& product);
    void deleteFromToBuyList(const Product& product);
    const std::vector<std::pair<unsigned short, unsigned short>>* getWay() const;
    std::pair<unsigned short, unsigned short> getPos(const int& ind) const;
    std::string getName() const;
};