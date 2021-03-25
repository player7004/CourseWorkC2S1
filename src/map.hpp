#pragma once
#include "parser.hpp"
#include <QString>
#include <cmath>

// Статусы карты
enum class MapStatus {
    // Инициализирована
    Initialized,
    // Успешно пропарсила файл
    Parsed,
    // Карта была создана
    Created,
    // Карта работает
    Working,
    // Карта проработала и все люди кончились
    Done,
    // Дерьмо случилось
    Error
};

// Статусы человека
enum class HumanStatus {
    // Человек просто был инициализирован
    New,
    // Готов идти
    ReadyToWalk,
    // Осматривает стенды вокруг
    Looking,
    // Берёт из стендов предметы
    Taking,
    // Покупает всё из TakenProducts
    Buying,
    // закончил
    Done,
    //
    Undefined
};

class Map {
private:
    // Купил человек или нет
    bool Bought;
    // Индекс того, что вокруг человека
    std::vector<Product>::iterator AroundCurrentHumanIndex;
    // То что вокруг человека
    std::vector<std::pair<ushort, ushort>>  AroundCurrentHuman;
    // Статус текущего человека
    HumanStatus HStatus;
    // Инициалищирует человека
    bool initialize();
    // Осуществляет перемещение человека
    bool move();
    // Осуществялет просмотр стендов
    bool look();
    // Осуществляет выбор предметов из стендов
    bool take();
    // Осуществляет покупку предметов
    bool buy();
    //

    // Лог
    Log MLog;
    // Все люди
    std::vector<Human> AllHumans;
    // Все объекты
    std::vector<Object> AllObjects;
    // Базовая карта
    std::vector<std::vector<QChar>> BaseMap;
    // Объектная карта
    std::vector<std::vector<Object*>> ObjectMap;
    // Очищает карту
    void clear();
    // Очищает человека
    void clearHuman();
public:
    MapStatus MStatus;
    // Текущая карта
    std::vector<std::vector<QChar>> CurrentMap;
    // Текущий человек
    Human* CurrentHuman;
    // Контент текущего стенда
    std::vector<Product>* CurrentStandContent;
    // Конструктор
    Map();
    // Открывает файл
    bool open(const QString& file);
    // Создаёт базовую карту
    bool create();
    // Пересоздаёт карту
    bool rebuild();
    // 
};
//#include <cmath>
//
// enum class MapStatuses {
//     // Карта не инициализированна
//     NotInitialized,
//     // Ошибка при парсинге
//     Error,
//     // Объекты были получены
//     // BaseMap не создан, CurrentMap не создан
//     Parsed,
//     // BaseMap и CurrentMap созданы
//     Created,
//     // Работает
//     // Люди ещё есть
//     Working,
//     // Завершено
//     // Люди кончились
//     Done
// };

// enum class HumanStatuses {
//     // Не ициализирован
//     NotInitialized,
//     // Инициализирован
//     Initialized,
//     // Поставлен на карту
//     Placed,
//     // Ходит
//     Walking,
//     // Лазит по стендам
//     Looking,
//     // Покупает
//     Bying,
//     // Кончился путь
//     WayIsDone,
//     // Всё купил
//     Bought
// };

// class Map {
// private:
//     // Люди
//     std::vector<Human> Humans;
//     // Объекты
//     std::vector<Object> AllObjects;

    
//     // Базовая карта
//     std::vector<std::vector<char>> BaseMap;
//     // Текущая карта
//     std::vector<std::vector<char>> CurrentMap;
//     // Объектная карта
//     std::vector<std::vector<const Object*>> ObjectMap;


//     // Текущий человек
//     Human CurrentHuman;
//     // Статус текущего человека
//     HumanStatuses CurrentHumanStatus;
//     // Указатель на текущего человека
//     std::vector<Human>::const_iterator CurrentHumanIndex;
//     // Указатель на путь текущего человека
//     std::vector<std::pair<unsigned short, unsigned short>>::const_iterator CurrentWayIndex;
//     // Объекты вокруг текущго человека
//     std::vector<const Object*> AroundCurrentHuman;
//     // Указатель на индекс объекта вокруг текущего человека
//     std::vector<const Object*>::const_iterator AroundCurrentHumanIndex;


//     // Имя файла для чтения
//     std::string FileName;
//     // Очищает карту
//     void clearMap();
// public:
//     // Возвращает указатель на объекты
//     const std::vector<Object>* getObjects() const;
//     // Возвращает указатель на людей
//     const std::vector<Human>* getHumans() const;
//     // Возвращает указатель на Базовую карту
//     const std::vector<std::vector<char>>* getBaseMap() const;
//     // Возвращает указатель на Текущую карту
//     const std::vector<std::vector<char>>* getCurrentMap() const;
//     // Возвращает указатель на Объектную карту
//     const std::vector<std::vector<const Object*>>* getObjectMap() const;
//     // Возращает имя файла
//     std::string getFileName() const;
//     // Конструктор
//     explicit Map(const std::string& filename="Shop.json");
//     // Парсит объект и создаёт карту
//     // Парсит файл с помощью класса Parser
//     // Копирует Humans и AllObjects
//     MapStatuses parse();
//     // Создаёт карту
//     // Создаёт BaseMap
//     // Создаёт ObjectMap
//     // Создаёт CurrentMap
//     MapStatuses create();
//     // Оператор вывода
//     friend std::ostream& operator<<(std::ostream& stream, const Map& map);
//     // Возвращает указатель на текущего человека
//     const Human* getCurrentHuman() const;
//     // Пересобирает карту 
//     // Делает следующий шаг
//     MapStatuses rebuildMap(const MapStatuses& status);
// };