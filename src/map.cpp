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
    CurrentHumanStatus = HumanStatuses::NotInitialized;
    AroundCurrentHuman = std::vector<const Object*>{};
    AroundCurrentHumanIndex = AroundCurrentHuman.cbegin();
    CurrentWayIndex = std::vector<std::pair<unsigned short, unsigned short>>::const_iterator();
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
                return MapStatuses::Error;
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
        return MapStatuses::Error;
    }
    CurrentMap = BaseMap;
    CurrentHumanStatus = HumanStatuses::NotInitialized;
    CurrentHumanIndex = Humans.cbegin();
    AroundCurrentHumanIndex = AroundCurrentHuman.cbegin();
    // Инициализируем человека
    if (Humans.empty()) {
        return MapStatuses::Done;
    }
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
    return  (CurrentHumanStatus == HumanStatuses::NotInitialized) ? nullptr: &CurrentHuman;
}

MapStatuses Map::rebuildMap(const MapStatuses& status) {
    if (status == MapStatuses::NotInitialized) {
        //  Инициализируем
        return parse();
    } else if (status == MapStatuses::Parsed) {
        // Создаём карту
        return create();
    } else if (status == MapStatuses::Created) {
        return MapStatuses::Working;
    } else if (status == MapStatuses::Working) {
        if (CurrentHumanStatus == HumanStatuses::NotInitialized) {
            // Если не инициализирован - пытаемся инициализировать
            // Если люди кончились - вырубаем
            if (Humans.empty()) {
                return MapStatuses::Done;
            } else {
                // Если прошлись уже по всем - готово
                if (CurrentHumanIndex == Humans.cend()) {
                    return MapStatuses::Done;
                }
                // Иначе инициализируем
                CurrentHuman = Human(*CurrentHumanIndex);
                CurrentHumanIndex++;
                CurrentWayIndex = CurrentHuman.getWay()->cbegin();
                CurrentHumanStatus = HumanStatuses::Initialized;
                return MapStatuses::Working;
            }
        } else if (CurrentHumanStatus == HumanStatuses::Initialized) {
            // Если человек инициализирован - ходим
            CurrentMap = BaseMap;
            CurrentMap[CurrentWayIndex->second][CurrentWayIndex->first] = CurrentHuman.getSymbol();
            CurrentHumanStatus = HumanStatuses::Walking;
            return MapStatuses::Working;
        } else if (CurrentHumanStatus == HumanStatuses::Placed) {
            // Смещаемся на шаг вперёд
            CurrentWayIndex++;
            // Если это конец - то путь кончен
            if (CurrentWayIndex == CurrentHuman.getWay()->cend()) {
                CurrentHumanStatus = HumanStatuses::WayIsDone;
            } else {
                // Иначе Ставим на эту позицию
                CurrentMap = BaseMap;
                CurrentMap[CurrentWayIndex->second][CurrentWayIndex->first] = CurrentHuman.getSymbol();
                CurrentHumanStatus = HumanStatuses::Walking;
            }
            return MapStatuses::Working;
        } else if (CurrentHumanStatus == HumanStatuses::Walking) {
            // Добавить проверку на пустой tobuylist
            // Смотрим объекты вокруг
            Objects otype;
            // Проверяем есть ли ещё объекты в TobuyList
            // Если их нет - переходим в Bying
            // Если есть - переходим в Looking
            if (CurrentHuman.getContent()->empty()) {
                otype = Objects::CashBox;
                CurrentHumanStatus = HumanStatuses::Bying;
            } else {
                otype = Objects::Stand;
            }
            std::pair<unsigned short, unsigned short> pos;
            if (CurrentWayIndex == CurrentHuman.getWay()->cbegin()) {
                pos = *CurrentWayIndex;
            } else {
                pos = *(CurrentWayIndex - 1);
            }
			// Проверка tobuylist  и takenproducts  у currenthuman
            // Добавляем
            if (pos.first - 1 >= 0) {
                AroundCurrentHuman.push_back(ObjectMap[pos.second][pos.first - 1]);
            }
            if (pos.first + 1 < ObjectMap[CurrentWayIndex->second].size()) {
                AroundCurrentHuman.push_back(ObjectMap[pos.second][pos.first + 1]);
            }
            if (pos.second - 1 >= 0) {
                AroundCurrentHuman.push_back(ObjectMap[pos.second - 1][pos.first]);
            }
            if (pos.second + 1 < ObjectMap.size()) {
                AroundCurrentHuman.push_back(ObjectMap[pos.second + 1][pos.first]);
            }
            std::vector<std::vector<const Object*>::const_iterator> toDelete;
            // Проверяем стенды они или нет и если нет - сохраняем итераторы для удаления
            for (auto i = AroundCurrentHuman.cbegin(); i != AroundCurrentHuman.cend(); i++) {
                auto obj = *i;
                if (obj->getOType() != otype) {
                    toDelete.push_back(i);
                }
            }
            // Удаляем все НЕ стенды
            for (const auto& i: toDelete) {
                AroundCurrentHuman.erase(i);
            }
            //
            if (AroundCurrentHuman.empty()) {
            // Если стендов нет - идём дальше
            // Иначе осматриваем их
                CurrentHumanStatus = HumanStatuses::Placed;
            } else {
                AroundCurrentHumanIndex = AroundCurrentHuman.cbegin();
                if (CurrentHuman.getContent()->empty()) {
                    CurrentHumanStatus = HumanStatuses::Bying;
                } else {
                    CurrentHumanStatus = HumanStatuses::Looking;
                }
            }
            return MapStatuses::Working;
        } else if (CurrentHumanStatus == HumanStatuses::Looking) {
			// Если список покупок пуст - возвращаемся искать стенды
			if (CurrentHuman.getContent()->empty()) {
				CurrentHumanStatus = HumanStatuses::Walking;
				return MapStatuses::Working;
			}
			// Если Стенды рядом кончились - Двигаемся дальше
            if (AroundCurrentHumanIndex == AroundCurrentHuman.cend()) {
				CurrentHumanStatus = HumanStatuses::Placed;
				return MapStatuses::Working;
			}
			auto obj = *AroundCurrentHumanIndex;
            // Если случилось какое-то дерьмо
            // То это дерьмо случилось
            // АУФ
            // Программу стоит выключить
            if (obj->getOType() != Objects::Stand) {
                clearMap();
                return MapStatuses::Error;
            }
            // Продукты для удаленния
            std::vector<std::vector<Product>::const_iterator> toDelete;
            // Проходимся по списку покупок
			for (auto tb = CurrentHuman.getContent()->cbegin(); tb != CurrentHuman.getContent()->cend(); tb++) {
                // Берём все продукты нужного нам типа
                std::vector<const Product*> available;
                for (auto sc = obj->getContent()->cbegin(); sc != obj->getContent()->cend(); sc++) {
                    if (sc->getPType() == tb->getPType()) {
                        available.push_back(sc.base());
                    }
                }
                // Если таких нет - продолжаем
                if (available.empty()) {
                    // Если не нашли нужного - идём дальше
                    continue;
                }
                // Выделяем из этого списка подходящий нам
                // Коеффициент нужного нам продукта
                short ProductCoeff = tb->getPrice() * tb->getAttractiveness();
                // Выбранный продукт
                std::vector<const Product*>::const_iterator choosen = available.cbegin();
                // Выбираем самый подходящий
                // Самый близкий по модулю к исходному 
                for (auto obj = available.cbegin() + 1; obj != available.cend(); obj++) {
                    auto i = *obj.base();
                    auto j = *choosen.base();
                    short coeff = i->getPrice() * i->getAttractiveness();
                    short currCoeff = j->getPrice() * i->getAttractiveness();
                    if (fabs(ProductCoeff - coeff) < fabs(ProductCoeff - currCoeff)) {
                        choosen = obj;
                    }
                }
                CurrentHuman.updateTakenProducts(**choosen);
                toDelete.push_back(tb);
			}
            // Нашли всё понравившееся
            // Теперь смотрим объекты вокруг и с некоторым шансом берём их
            for (const auto& i: toDelete) {
                // Коеффициент. Чем больше - тем больше шанс взять продукт
                // Если у продукта привлекательность меньше, то он даже не будет рассмотрен
                float coeff = 0.5;
                // В левую сторону
                for (auto j = i; j != CurrentHuman.getContent()->cbegin() and coeff < 0.9; j--, coeff += 0.17) {
                    if (!(CurrentHuman.inTakenProducts(*j)) and !(CurrentHuman.inToBuyList(*j))) {
                        // Смотрим подходит продукт или нет
                        if (j->getAttractiveness() < coeff) {
                            continue;
                        } else {
                            if (j->getPrice() * j->getAttractiveness() > int(coeff*100) - rand()%100*(1-coeff)) {
                                CurrentHuman.updateTakenProducts(*j);
                            }
                        }
                    }
                }
                coeff = 0.5;
                // В правую сторону
                for (auto j = i; j != CurrentHuman.getContent()->cend() and coeff < 0.8; j++, coeff += 0.17) {
                    if (!(CurrentHuman.inTakenProducts(*j)) and !(CurrentHuman.inToBuyList(*j))) {
                        // Смотрим подходит продукт или нет
                        if (j->getAttractiveness() < coeff) {
                            continue;
                        } else {
                            if (j->getPrice() * j->getAttractiveness() > int(coeff*100) - rand()%100*(1-coeff)) {
                                CurrentHuman.updateTakenProducts(*j);
                            }
                        }
                    }
                }
                // Удаляем из списка покупок
                CurrentHuman.deleteFromBuyList(i);
            }
            // Смещаемся на следующий 
            AroundCurrentHumanIndex++;
            return MapStatuses::Working;
        } else if(CurrentHumanStatus == HumanStatuses::Bying) {
            // Если Объекты вокруг кончились - двигаемся дальше
            if (AroundCurrentHumanIndex == AroundCurrentHuman.cend()) {
                CurrentHumanStatus = HumanStatuses::Placed;
				return MapStatuses::Working;
            }
            auto cashbox = *AroundCurrentHumanIndex;
            if (cashbox->getOType() != Objects::CashBox) {
                clearMap();
                return MapStatuses::Error;
            }
            std::vector<std::vector<Product>::const_iterator> toDelete;
            for (auto i = CurrentHuman.getTakenProducts()->cbegin(); i != CurrentHuman.getTakenProducts()->cend(); i++) {
                short price = i->getPrice();
            }
            // Доделать
            // Смащаемся дальше
            AroundCurrentHumanIndex++;
        }
    } else if (status == MapStatuses::Done) {
        return MapStatuses::Done;
    } else if (status == MapStatuses::Error) {
        return MapStatuses::Error;
    } else {
        return MapStatuses::Error;
    }
}