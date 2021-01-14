#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <exception>

enum class Entities
{
    // Объект
    Object,
    // Продукт
    Product,
    // Неопределённый
    Undefined
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
    Door,
    // Непоределённый
    Undefined
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
    Lemon,
    // Неопределённый
    Undefined
};

namespace std {
    std::string to_string(const Entities& type);
    std::string to_string(const Objects& otype);
    std::string to_string(const Products& ptype);
}


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
        const Products& ptype = Products::Undefined,
        unsigned short price = 100,
        float attractiveness = 1);
    Products getPType() const;
    unsigned short getPrice() const;
    float getAttractiveness() const;
    friend std::ostream& operator<<(std::ostream& stream,const Product& prod);
    friend bool operator==(const Product& prod1, const Product& prod2);
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
    std::vector<Product> Content;
    // Кол-во денег
    unsigned int Cash;
public:
    explicit Object(const std::pair<unsigned short, unsigned short>& position=std::pair<unsigned short, unsigned short>{0, 0},
                    const std::pair<unsigned short, unsigned short>& size=std::pair<unsigned short, unsigned short>{1, 1},
                    const Objects& otype=Objects::Undefined,
                    const char& symbol = 'U',
                    std::vector<Product> content = std::vector<Product>());
    std::pair<unsigned short, unsigned short> getPosition() const;
    std::pair<unsigned short, unsigned short> getSize() const;
    Objects getOType() const;
    char getSymbol() const;
    friend std::ostream& operator<<(std::ostream& stream, const Object& obj);
    const std::vector<Product>* getContent() const;
    unsigned int getCash() const;
};

class Human: public Object {
private:
    // Имя человека
    std::string Name;
    // Список взятых продуктов
    std::vector<Product> TakenProducts;
    // Путь человека
    std::vector<std::pair<unsigned short, unsigned short>> Way;
public:
    Human(
        const std::string& name,
        const std::pair<unsigned short, unsigned short>& position,
        const std::pair<unsigned short, unsigned short>& size,
        std::vector<Product> tobuylist = std::vector<Product>(),
        std::vector<std::pair<unsigned short, unsigned short>> way = std::vector<std::pair<unsigned short, unsigned short>>()
    );
    Human();
    // Смотрет есть ли такой объект в Tobuylist
    bool inToBuyList(const Product& product) const;
    bool inTakenProducts(const Product& product) const;
    const std::vector<Product>* getTakenProducts() const;
    void updateTakenProducts(const Product& product);
    // Возможно устаревший
    void deleteFromToBuyList(const Product& product);
    // Удалить элемент из вектора
    void deleteFromBuyList(const std::vector<Product>::const_iterator& product);
    const std::vector<std::pair<unsigned short, unsigned short>>* getWay() const;
    std::pair<unsigned short, unsigned short> getPos(const int& ind) const;
    std::string getName() const;
    friend std::ostream& operator<<(std::ostream& stream, const Human& hum);
};