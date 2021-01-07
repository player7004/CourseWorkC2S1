#include "map.hpp"

std::ostream& operator<<(std::ostream& stream, const Map& map) {
    stream << "Map: " << map.getFileName() << std::endl;
    // Печатаем простую карту
    stream << "Base Map: " << std::endl;
    for (auto i = map.getBaseMap()->begin(); i != map.getBaseMap()->end(); i++) {
        for (auto j = i->begin(); j != i->end(); j++) {
            stream << *j;
        }
        stream << std::endl;
    }
    // Печатаем текущую карту
    stream << std::endl << "Current Map: " << std::endl;
        for (auto i = map.getCurrentMap()->begin(); i != map.getCurrentMap()->end(); i++) {
            for (auto j = i->begin(); j != i->end(); j++) {
                stream << *j;
        }
        stream << std::endl;
    }
    // Выводим объекты
    stream << std::endl << "Objects: " << std::endl;
    for (auto i = map.getObjects()->begin(); i != map.getObjects()->end(); i++) {
        stream << *i;
    }
    stream << std::endl;
    // Выводим людей
    stream << std::endl << "Humans " << std::endl; //TODO
    for (auto i = map.getHumans()->begin(); i != map.getHumans()->end(); i++) {
        stream << *i;
    }
    return stream;
}

Map::Map(const std::string& filename) {
    FileName = filename;
};

void Map::clearMap() {
    AllObjects.clear();
    Humans.clear();
    BaseMap.clear();
    CurrentMap.clear();
    ObjectMap.clear();
    CurrentHuman = Human();
    CurrentHumanIndex = std::vector<Human>::const_iterator();
}

MapStatuses Map::create() {
    // Создаём BaseMap и ObjectMap
    if (AllObjects[0].getOType() != Objects::EmptySpace) {
        return MapStatuses::Error;
    }
    // Создаём размер базовой карты
    BaseMap.resize(AllObjects[0].getPosition().second + AllObjects[0].getSize().second);
    for (auto& i: BaseMap) {
        i.resize(AllObjects[0].getPosition().first + AllObjects[0].getSize().first);
    }
    // Создаём размер объектной карты
    ObjectMap.resize(AllObjects[0].getPosition().second + AllObjects[0].getSize().second);
    for (auto& i: ObjectMap) {
        i.resize(AllObjects[0].getPosition().first + AllObjects[0].getSize().first);
    }
    // Заполняем
    try {
        for (const auto& i: AllObjects) {
            char symbol = i.getSymbol();
            const Object* obj = &i;
            unsigned short x = i.getPosition().first;
            unsigned short y = i.getPosition().second;
            unsigned short maxX = x + i.getSize().first;
            unsigned short maxY = y + i.getSize().second;
            if (x > maxX and y > maxY) {
                clearMap();
                return MapStatuses::NotInitialized;
            }
            for (int Y = y; Y < maxY; Y++) {
                for (int X = x; X < maxX; X++) {
                    BaseMap[Y][X] = symbol;
                    ObjectMap[Y][X] = obj;
                }
            }
        }
    } catch (...) {
        clearMap();
        return MapStatuses::NotInitialized;
    }
    CurrentMap = BaseMap;
    CurrentHuman = Humans[0];
    CurrentHumanIndex = Humans.begin();
    CurrentHumanIndex++;
    return MapStatuses::Created;
}

MapStatuses Map::parse() {
    auto parser = Parser(FileName);
    if (!parser.parse()) {
        return MapStatuses::Error;
    }
    AllObjects = parser.getObjects();
    Humans = parser.getHumans();
    return MapStatuses::Parsed;
}

const std::vector<std::vector<char>>* Map::getBaseMap() const {
    return &BaseMap;
}

const std::vector<std::vector<char>>* Map::getCurrentMap() const {
    return &CurrentMap;
}

const std::vector<std::vector<const Object*>>* Map::getObjectMap() const {
    return &ObjectMap;
}

std::string Map::getFileName() const {
    return FileName;
}

const std::vector<Object>* Map::getObjects() const {
    return &AllObjects;
}

const std::vector<Human>* Map::getHumans() const {
    return &Humans;
}

const Human* Map::getCurrentHuman() const {
    return &CurrentHuman;
}