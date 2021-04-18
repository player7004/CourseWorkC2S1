#pragma once
#include "parser.hpp"
#include <QString>
#include <cmath>

enum class MapStatus {
    // Карта была инициализирована
    Initialized,
    // Файл для карты был прочитан
    Opened,
    // Карта была создана
    Created,
    // Карта в процессе работы
    Working,
    // Работа закончена
    Done,
    // Ошибка выполнения
    Error
};

enum class HumanStatus {
    // Не инициализирован
    None,
    // Инициализирован
    Initialized,
    // Походил
    Walked,
    // Осматривает стенды
    Looking,
    // Берёт продукты
    Taking,
    // Покупает продукты
    Buying,
    // Закончил
    Done,
    // Люди кончились
    End,
    // Ошибка
    // Человек не купил всё или не всё взял
    Error
};

namespace std {
    string to_string(const HumanStatus& status);
}


// Карта
class Map {
private:
    // Здесь лежат все имеющиеся объекты
    std::vector<Object> AllObjects;
    // Здесь лежат все имеющиеся люди
    std::vector<Human> AllHumans;

    // То что связано с картой в смысле её графического представления
    // Базовая карта
    std::vector<std::vector<QChar>> BaseMap;
    // Объектная карта
    std::vector<std::vector<Object*>> ObjectMap;
    // Обновляет карту на выход
    void updateOutMap();

    // Все что связано с человеком и его объектами
    // Итератор указывающий на местоположение текущиего человека
    std::vector<Human>::iterator CurrentHumanIter;
    // Указатель на текущего человека
    Human* CurrentHuman;
    // Указатель на местоположение текущего человека
    std::vector<std::pair<ushort, ushort>>::iterator CurrentHumanWayIter;
    // То, что находится вокруг человека
    std::vector<Object> AroundCurrentHuman;
    // Итератор по AroundCurrentHuman
    std::vector<Object>::iterator AroundCurrentHumanIter;
    // Статус текущего человека
    HumanStatus HStatus;
    // Инициализирует человека
    void initializeHuman();
    // Деинициализирует человека
    void clearHuman();
    // Просто сбрасывает человека
    void resetHuman();
    // Перемещает человека
    void move();
    // Cтавит человека
    void placeHuman();
    // Осматривает всё вокруг человека
    void look();
    // Берёт предметы из стенда
    void take();
    // Покупает предметы
    void buy();

    // Свойства и атрибуты карты
    // Статус карты
    MapStatus Status;
    // Собственный лог карты
    Log MLog;
public:
    // Карта на выход
    std::vector<QString> OutMap;

    // Базовые функции
    // Конструктор
    Map();
    // Деструктор
    ~Map();

    // Функции контроля карты
    // Отрывает файл
    bool open(const QString& file);
    // Создаёт из файла карту
    bool create();
    // Очищает карту
    void clear();
    // Создаёт легенду карты
    std::vector<QString> generateMapLegend();
    // Создаёт содежимое текущего стенды
    std::vector<QString> generateStandContent();
    // Создаёт содержимое текущего списка покупок
    std::vector<QString> generateToBuyList();
    // Создает список взятых продуктов
    std::vector<QString> generateTakenProducts();
    // Создаёт информацию о текущем человеке
    std::vector<QString> generateCurrentHumanInfo();
    // Создаёт информацию о всех людях
    std::vector<QString> generateAllHumans();
    // Перестраивает карту
    void rebuild();
    // Меняет статус текущего человека
    void inline changeHStatus(const HumanStatus& status);
};