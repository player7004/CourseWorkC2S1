#include "parser.hpp"

enum class MapStatuses {
    // Карта не инициализированна
    NotInitialized,
    // Ошибка при парсинге
    Error,
    // Объекты были получены
    // BaseMap не создан, CurrentMap не создан
    Parsed,
    // BaseMap и CurrentMap созданы
    Created,
};

class Map {
private:
    // Люди
    std::vector<Human> Humans;
    // Объекты
    std::vector<Object> AllObjects;
    // Базовая карта
    std::vector<std::vector<char>> BaseMap;
    // Текущая карта
    std::vector<std::vector<char>> CurrentMap;
    // Объектная карта
    std::vector<std::vector<const Object*>> ObjectMap;
    // Текущий человек
    Human CurrentHuman;
    // Указатель на текущего человека
    std::vector<Human>::const_iterator CurrentHumanIndex;
    // Имя файла для чтения
    std::string FileName;
    // Очищает карту
    void clearMap();
public:
    // Возвращает указатель на объекты
    const std::vector<Object>* getObjects() const;
    // Возвращает указатель на людей
    const std::vector<Human>* getHumans() const;
    // Возвращает указатель на Базовую карту
    const std::vector<std::vector<char>>* getBaseMap() const;
    // Возвращает указатель на Текущую карту
    const std::vector<std::vector<char>>* getCurrentMap() const;
    // Возвращает указатель на Объектную карту
    const std::vector<std::vector<const Object*>>* getObjectMap() const;
    // Возращает имя файла
    std::string getFileName() const;
    // Конструктор
    explicit Map(const std::string& filename="Shop.json");
    // Парсит объект и создаёт карту
    // Парсит файл с помощью класса Parser
    // Копирует Humans и AllObjects
    MapStatuses parse();
    // Создаёт карту
    // Создаёт BaseMap
    // Создаёт ObjectMap
    // Создаёт CurrentMap
    MapStatuses create();
    // Оператор вывода
    friend std::ostream& operator<<(std::ostream& stream, const Map& map);
    // Возвращает указатель на текущего человека
    const Human* getCurrentHuman() const;
};