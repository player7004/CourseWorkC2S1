#include "map.hpp"

Map::Map() {
	MLog.open("Map.Log");
	MLog.write("Map Initialized");
	Status = MapStatus::Initialized;
    CurrentHumanIter = {};
    CurrentHuman = nullptr;
    AroundCurrentHuman = {};
    AroundCurrentHumanIter = {};
    CurrentHumanWayIter = {};
    HStatus = HumanStatus::None;
}

Map::~Map() {
	MLog.write("Map Closed");
}

bool Map::open(const QString& file) {
	clear();
	MLog.write("Opening file " + file);
	Parser obj;
	if (obj.parse(file)) {
		AllObjects = obj.AllObjects;
		AllHumans = obj.AllHumans;
		MLog.write("File " + file + " sucessfuly parsed");
		Status = MapStatus::Opened;
		return true;
	}
	else {
		MLog.write("File " + file + " can`t be parsed", WriteTypes::Info);
		Status = MapStatus::Error;
		return false;
	}
}

bool Map::create() {
	MLog.write("Starting to create Map");
    std::vector<Object>::iterator pos = AllObjects.begin();
    if (pos->OType != "EmptySpace") {
        MLog.write("First object isn`t EmptySpace", WriteTypes::Info);
        MLog.write("Map creating stopped");
        return false;
    }
    ushort x = pos->Position.first, y = pos->Position.second;
    ushort X = x + pos->Size.first, Y = y + pos->Size.second;
    if (x >= X or y >= Y) {
        MLog.write("Position or size bad", WriteTypes::Info);
        MLog.write("Map creating stopped");
        return false;
    }
    BaseMap.resize(Y);
    for (auto& vect: BaseMap) {
        vect.resize(X);
    }
    ObjectMap.resize(Y);
    for (auto& vect: ObjectMap) {
        vect.resize(X);
    }
    while(pos != AllObjects.end()) {
        x = pos->Position.first, y = pos->Position.second;
        X = x + pos->Size.first, Y = y + pos->Size.second;
        if (x >= X or y >= Y) {
            MLog.write("Position or size bad", WriteTypes::Info);
            MLog.write("Map creating stopped");
            return false;
        }
        for (ushort i = y; i < Y; i++) {
            for (ushort j = x; j < X; j++) {
                BaseMap[j][i] = pos->Symbol;
                ObjectMap[j][i] = pos.base();
            }
        }
        pos++;
    }
    Status = MapStatus::Created;
    MLog.write("Map created");
    updateOutMap();
    return true;
}

void Map::clear() {
	MLog.write("The map was cleared");
	Status = MapStatus::Initialized;
	AllObjects.clear();
	AllHumans.clear();
}

void Map::rebuild() {

}


void Map::updateOutMap() {
	std::vector<QString> result;
	for (const auto& i : BaseMap) {
		QString str;
		for (const auto& j : i) {
			str += j;
		}
		result.push_back(str);
	}
	OutMap = result;
	MLog.write("Out Map Updated");
}

std::vector<QString> Map::generateMapLegend() {
    MLog.write("Creating Map Legend");
    std::map<QChar, QString> Dict;
    // Вносим Объекты
    for (const auto& i : AllObjects) {
        if (Dict.find(i.Symbol) == Dict.end()) {
            Dict.insert({ i.Symbol, i.OType });
        }
    }
    MLog.write("Objects Legend created");
    // Вносим людей
    for (const auto& i : AllHumans) {
        if (Dict.find(i.Symbol) == Dict.end()) {
            Dict.insert({ i.Symbol, i.Type });
        }
    }
    MLog.write("Humans Legend Created");
    // Результат
    std::vector<QString> result;
    // Переносим результат
    for (const auto& i : Dict) {
        result.emplace_back(i.first + QString(" - ") + i.second);
    }
    MLog.write("Map Legend created");
    return result;
}

std::vector<QString> Map::generateStandContent() {
    std::vector<QString> result;
    if (AroundCurrentHumanIter == std::vector<Object>::iterator()) {
        result.push_back("None");
        return result;
    }
    if (AroundCurrentHumanIter.base()->OType == "Stand") {
        for (const auto& i : AroundCurrentHumanIter.base()->Content) {
            result.push_back(QString(std::to_string(i).c_str()));
        }
    }
    else {
        result.push_back("None");
    }
    return result;
}

std::vector<QString> Map::generateToBuyList() {
    std::vector<QString> result;
    if (CurrentHuman == nullptr) {
        result.push_back("None");
    }
    else {
        for (const auto& i : CurrentHuman->ToBuyList) {
            result.push_back(QString(std::to_string(i).c_str()));
        }
    }
    return result;
}

std::vector<QString> Map::generateTakenProducts() {
    std::vector<QString> result;
    if (CurrentHuman == nullptr) {
        result.push_back("None");
    }
    else {
        for (const auto& i : CurrentHuman->TakenProducts) {
            result.push_back(QString(std::to_string(i).c_str()));
        }
    }
    return result;
}
//Map::Map() {
//    MLog.open("Map.log");
//    MLog.write("Map Initialized");
//    CurrentHuman = nullptr;
//    CurrentStandContent = nullptr;
//    MStatus = MapStatus::Initialized;
//}

//bool Map::open(const QString& file) {
//    clear();
//    MLog.write("Openinig file " + file);
//    Parser parser;
//    if (parser.parse(file)) {
//        AllObjects = parser.AllObjects;
//        AllHumans = parser.AllHumans;
//        MLog.write("File " + file + " parsed");
//        MStatus = MapStatus::Parsed;
//        return true;
//    } else {
//        MLog.write("Can`t parse file " + file, WriteTypes::Info);
//        MLog.write("File " + file + " isn`t opened");
//        return false;
//    }
//}
//
//void Map::clear() {
//    MStatus = MapStatus::Initialized;
//    AllObjects.clear();
//    AllHumans.clear();
//    BaseMap.clear();
//    CurrentMap.clear();
//    CurrentHuman = nullptr;
//    CurrentStandContent = nullptr;
//    MLog.write("Map is cleared");
//}
//
//bool Map::create() {
//    MLog.write("Creating map");
//    std::vector<Object>::iterator pos = AllObjects.begin();
//    if (pos->OType != "EmptySpace") {
//        MLog.write("First object isn`t EmptySpace", WriteTypes::Info);
//        MLog.write("Map creating stopped");
//        return false;
//    }
//    ushort x = pos->Position.first, y = pos->Position.second;
//    ushort X = x + pos->Size.first, Y = y + pos->Size.second;
//    if (x >= X or y >= Y) {
//        MLog.write("Position or size bad", WriteTypes::Info);
//        MLog.write("Map creating stopped");
//        return false;
//    }
//    BaseMap.resize(Y);
//    for (auto& vect: BaseMap) {
//        vect.resize(X);
//    }
//    ObjectMap.resize(Y);
//    for (auto& vect: ObjectMap) {
//        vect.resize(X);
//    }
//    while(pos != AllObjects.end()) {
//        x = pos->Position.first, y = pos->Position.second;
//        X = x + pos->Size.first, Y = y + pos->Size.second;
//        std::cout << std::to_string(*pos) << std::endl;
//        if (x >= X or y >= Y) {
//            MLog.write("Position or size bad", WriteTypes::Info);
//            MLog.write("Map creating stopped");
//            return false;
//        }
//        for (ushort i = y; i < Y; i++) {
//            for (ushort j = x; j < X; j++) {
//                BaseMap[i][j] = pos->Symbol;
//                ObjectMap[i][j] = pos.base();
//            }
//        }
//        pos++;
//    }
//    MLog.write("Map created");
//    CurrentMap = BaseMap;
//    MStatus = MapStatus::Created;
//    return true;
//}
//
//bool Map::initialize() {
//    try {
//        clearHuman();
//        CurrentHuman = CurrentHumanIndex.base();
//        CurrentHumanWay = &CurrentHuman->Way;
//        CurrentHumanWayIndex = CurrentHumanWay->begin();
//        CurrentHumanToBuyList = &CurrentHuman->ToBuyList;
//        HStatus = HumanStatus::New;
//        MLog.write("Human Initialized");
//        return true;
//    }
//    catch (...) {
//        MLog.write("Can`t initialize Human", WriteTypes::Info);
//        return false;
//    }
//}
//
//bool Map::move() {
//    try {
//        if (CurrentHumanWayIndex == CurrentHumanWay->end()) {
//            HStatus == HumanStatus::Walked;
//            return true;
//        }
//        else {
//            MLog.write("Human moved");
//            CurrentMap = BaseMap;
//            CurrentMap[CurrentHumanWayIndex->second][CurrentHumanWayIndex->first] = CurrentHuman->Symbol;
//            CurrentHumanWayIndex++;
//            HStatus == HumanStatus::Walked;
//            return true;
//        }
//    }
//    catch (...) {
//        MLog.write("Can`t set human position to next", WriteTypes::Info);
//        return false;
//    }
//
//}
//
//bool Map::look() {
//    // Если вектор пуст, то создаём новый
//    if (AroundCurrentHuman == nullptr) {
//        AroundCurrentHuman = new std::vector<Object>{};
//        std::pair<ushort, ushort> position = *(CurrentHumanWayIndex - 1).base();
//        if (position.first - 1 > 0) {
//            AroundCurrentHuman->emplace_back(*ObjectMap[position.second][position.first - 1]);
//        }
//        if (position.first + 1 < ObjectMap[0].size()) {
//            AroundCurrentHuman->emplace_back(*ObjectMap[position.second][position.first + 1]);
//        }
//        if (position.second - 1 > 0) {
//            AroundCurrentHuman->emplace_back(*ObjectMap[position.second - 1][position.first]);
//        }
//        if (position.second + 1 < ObjectMap.size()) {
//            AroundCurrentHuman->emplace_back(*ObjectMap[position.second + 1][position.first]);
//        }
//        AroundCurrentHumanIndex = AroundCurrentHuman->begin();
//    }
//}
//
//bool Map::take() {
//
//}
//
//bool Map::buy() {
//    
//}
//
//bool Map::rebuild() {
//    if (MStatus == MapStatus::Created) {
//        MLog.write("Current human index set to start");
//        CurrentHumanIndex = AllHumans.begin();
//        return initialize();
//    }
//    else if (MStatus == MapStatus::Working) {
//        if (HStatus == HumanStatus::New) {
//            if (move()) {
//                return true;
//            }
//            else {
//                MStatus = MapStatus::Error;
//                MLog.write("Current human isn`t movable. Aborting!", WriteTypes::Error);
//                return false;
//            }
//        }
//        else if (HStatus == HumanStatus::Walked) {
//
//        }
//        else if (HStatus == HumanStatus::Looking) {
//
//        }
//        else if (HStatus == HumanStatus::Taking) {
//
//        }
//        else if (HStatus == HumanStatus::Buying) {
//
//        }
//        else if (HStatus == HumanStatus::Done) {
//
//        }
//        else {
//
//        }
//    }
//    else {
//        MLog.write("Trying to call rebuild while map isn`t created", WriteTypes::Info);
//        return false;
//    }
//}
//
//void Map::clearHuman() {
//    HStatus = HumanStatus::Undefined;
//    AroundCurrentHuman = nullptr;
//    AroundCurrentHumanIndex = std::vector<Object>::iterator();
//    CurrentHumanWay = nullptr;
//    CurrentHumanWayIndex = std::vector<std::pair<ushort, ushort>>::iterator();
//    CurrentHuman = nullptr;
//    CurrentStandContent = nullptr;
//    CurrentHumanToBuyList = nullptr;
//}

// std::ostream& operator<<(std::ostream& stream, const Map& map) {
//     stream << "Map: " << map.getFileName() << std::endl;
//     // Печатаем простую карту
//     stream << "Base Map: " << std::endl;
//     for (auto i = map.getBaseMap()->begin(); i != map.getBaseMap()->end(); i++) {
//         for (auto j = i->begin(); j != i->end(); j++) {
//             stream << *j;
//         }
//         stream << std::endl;
//     }
//     // Печатаем текущую карту
//     stream << std::endl << "Current Map: " << std::endl;
//         for (auto i = map.getCurrentMap()->begin(); i != map.getCurrentMap()->end(); i++) {
//             for (auto j = i->begin(); j != i->end(); j++) {
//                 stream << *j;
//         }
//         stream << std::endl;
//     }
//     // Выводим объекты
//     stream << std::endl << "Objects: " << std::endl;
//     for (auto i = map.getObjects()->begin(); i != map.getObjects()->end(); i++) {
//         stream << *i;
//     }
//     stream << std::endl;
//     // Выводим людей
//     stream << std::endl << "Humans " << std::endl; //TODO
//     for (auto i = map.getHumans()->begin(); i != map.getHumans()->end(); i++) {
//         stream << *i;
//     }
//     return stream;
// }

// Map::Map(const std::string& filename) {
//     FileName = filename;
// };

// void Map::clearMap() {
//     AllObjects.clear();
//     Humans.clear();
//     BaseMap.clear();
//     CurrentMap.clear();
//     ObjectMap.clear();
//     CurrentHuman = Human();
//     CurrentHumanIndex = std::vector<Human>::const_iterator();
//     CurrentHumanStatus = HumanStatuses::NotInitialized;
//     AroundCurrentHuman = std::vector<const Object*>{};
//     AroundCurrentHumanIndex = AroundCurrentHuman.cbegin();
//     CurrentWayIndex = std::vector<std::pair<unsigned short, unsigned short>>::const_iterator();
// }

// MapStatuses Map::create() {
//     // Создаём BaseMap и ObjectMap
//     if (AllObjects[0].getOType() != Objects::EmptySpace) {
//         return MapStatuses::Error;
//     }
//     // Создаём размер базовой карты
//     BaseMap.resize(AllObjects[0].getPosition().second + AllObjects[0].getSize().second);
//     for (auto& i: BaseMap) {
//         i.resize(AllObjects[0].getPosition().first + AllObjects[0].getSize().first);
//     }
//     // Создаём размер объектной карты
//     ObjectMap.resize(AllObjects[0].getPosition().second + AllObjects[0].getSize().second);
//     for (auto& i: ObjectMap) {
//         i.resize(AllObjects[0].getPosition().first + AllObjects[0].getSize().first);
//     }
//     // Заполняем
//     try {
//         for (const auto& i: AllObjects) {
//             char symbol = i.getSymbol();
//             const Object* obj = &i;
//             unsigned short x = i.getPosition().first;
//             unsigned short y = i.getPosition().second;
//             unsigned short maxX = x + i.getSize().first;
//             unsigned short maxY = y + i.getSize().second;
//             if (x > maxX and y > maxY) {
//                 clearMap();
//                 return MapStatuses::Error;
//             }
//             for (int Y = y; Y < maxY; Y++) {
//                 for (int X = x; X < maxX; X++) {
//                     BaseMap[Y][X] = symbol;
//                     ObjectMap[Y][X] = obj;
//                 }
//             }
//         }
//     } catch (...) {
//         clearMap();
//         return MapStatuses::Error;
//     }
//     CurrentMap = BaseMap;
//     CurrentHumanStatus = HumanStatuses::NotInitialized;
//     CurrentHumanIndex = Humans.cbegin();
//     AroundCurrentHumanIndex = AroundCurrentHuman.cbegin();
//     // Инициализируем человека
//     if (Humans.empty()) {
//         return MapStatuses::Done;
//     }
//     return MapStatuses::Created;
// }

// MapStatuses Map::parse() {
//     auto parser = Parser(FileName);
//     if (!parser.parse()) {
//         return MapStatuses::Error;
//     }
//     AllObjects = parser.getObjects();
//     Humans = parser.getHumans();
//     return MapStatuses::Parsed;
// }

// const std::vector<std::vector<char>>* Map::getBaseMap() const {
//     return &BaseMap;
// }

// const std::vector<std::vector<char>>* Map::getCurrentMap() const {
//     return &CurrentMap;
// }

// const std::vector<std::vector<const Object*>>* Map::getObjectMap() const {
//     return &ObjectMap;
// }

// std::string Map::getFileName() const {
//     return FileName;
// }

// const std::vector<Object>* Map::getObjects() const {
//     return &AllObjects;
// }

// const std::vector<Human>* Map::getHumans() const {
//     return &Humans;
// }

// const Human* Map::getCurrentHuman() const {
//     return  (CurrentHumanStatus == HumanStatuses::NotInitialized) ? nullptr: &CurrentHuman;
// }

// MapStatuses Map::rebuildMap(const MapStatuses& status) {
//     if (status == MapStatuses::NotInitialized) {
//         //  Инициализируем
//         return parse();
//     } else if (status == MapStatuses::Parsed) {
//         // Создаём карту
//         return create();
//     } else if (status == MapStatuses::Created) {
//         return MapStatuses::Working;
//     } else if (status == MapStatuses::Working) {
//         if (CurrentHumanStatus == HumanStatuses::NotInitialized) {
//             // Если не инициализирован - пытаемся инициализировать
//             // Если люди кончились - вырубаем
//             if (Humans.empty()) {
//                 return MapStatuses::Done;
//             } else {
//                 // Если прошлись уже по всем - готово
//                 if (CurrentHumanIndex == Humans.cend()) {
//                     return MapStatuses::Done;
//                 }
//                 // Иначе инициализируем
//                 CurrentHuman = Human(*CurrentHumanIndex);
//                 CurrentHumanIndex++;
//                 CurrentWayIndex = CurrentHuman.getWay()->cbegin();
//                 CurrentHumanStatus = HumanStatuses::Initialized;
//                 return MapStatuses::Working;
//             }
//         } else if (CurrentHumanStatus == HumanStatuses::Initialized) {
//             // Если человек инициализирован - ходим
//             CurrentMap = BaseMap;
//             CurrentMap[CurrentWayIndex->second][CurrentWayIndex->first] = CurrentHuman.getSymbol();
//             CurrentHumanStatus = HumanStatuses::Walking;
//             return MapStatuses::Working;
//         } else if (CurrentHumanStatus == HumanStatuses::Placed) {
//             // Смещаемся на шаг вперёд
//             CurrentWayIndex++;
//             // Если это конец - то путь кончен
//             if (CurrentWayIndex == CurrentHuman.getWay()->cend()) {
//                 CurrentHumanStatus = HumanStatuses::WayIsDone;
//             } else {
//                 // Иначе Ставим на эту позицию
//                 CurrentMap = BaseMap;
//                 CurrentMap[CurrentWayIndex->second][CurrentWayIndex->first] = CurrentHuman.getSymbol();
//                 CurrentHumanStatus = HumanStatuses::Walking;
//             }
//             return MapStatuses::Working;
//         } else if (CurrentHumanStatus == HumanStatuses::Walking) {
//             // Добавить проверку на пустой tobuylist
//             // Смотрим объекты вокруг
//             Objects otype;
//             // Проверяем есть ли ещё объекты в TobuyList
//             // Если их нет - переходим в Bying
//             // Если есть - переходим в Looking
//             if (CurrentHuman.getContent()->empty()) {
//                 otype = Objects::CashBox;
//                 CurrentHumanStatus = HumanStatuses::Bying;
//             } else {
//                 otype = Objects::Stand;
//             }
//             std::pair<unsigned short, unsigned short> pos;
//             if (CurrentWayIndex == CurrentHuman.getWay()->cbegin()) {
//                 pos = *CurrentWayIndex;
//             } else {
//                 pos = *(CurrentWayIndex - 1);
//             }
// 			// Проверка tobuylist  и takenproducts  у currenthuman
//             // Добавляем
//             if (pos.first - 1 >= 0) {
//                 AroundCurrentHuman.push_back(ObjectMap[pos.second][pos.first - 1]);
//             }
//             if (pos.first + 1 < ObjectMap[CurrentWayIndex->second].size()) {
//                 AroundCurrentHuman.push_back(ObjectMap[pos.second][pos.first + 1]);
//             }
//             if (pos.second - 1 >= 0) {
//                 AroundCurrentHuman.push_back(ObjectMap[pos.second - 1][pos.first]);
//             }
//             if (pos.second + 1 < ObjectMap.size()) {
//                 AroundCurrentHuman.push_back(ObjectMap[pos.second + 1][pos.first]);
//             }
//             std::vector<std::vector<const Object*>::const_iterator> toDelete;
//             // Проверяем стенды они или нет и если нет - сохраняем итераторы для удаления
//             for (auto i = AroundCurrentHuman.cbegin(); i != AroundCurrentHuman.cend(); i++) {
//                 auto obj = *i;
//                 if (obj->getOType() != otype) {
//                     toDelete.push_back(i);
//                 }
//             }
//             // Удаляем все НЕ стенды
//             for (const auto& i: toDelete) {
//                 AroundCurrentHuman.erase(i);
//             }
//             //
//             if (AroundCurrentHuman.empty()) {
//             // Если стендов нет - идём дальше
//             // Иначе осматриваем их
//                 CurrentHumanStatus = HumanStatuses::Placed;
//             } else {
//                 AroundCurrentHumanIndex = AroundCurrentHuman.cbegin();
//                 if (CurrentHuman.getContent()->empty()) {
//                     CurrentHumanStatus = HumanStatuses::Bying;
//                 } else {
//                     CurrentHumanStatus = HumanStatuses::Looking;
//                 }
//             }
//             return MapStatuses::Working;
//         } else if (CurrentHumanStatus == HumanStatuses::Looking) {
// 			// Если список покупок пуст - возвращаемся искать стенды
// 			if (CurrentHuman.getContent()->empty()) {
// 				CurrentHumanStatus = HumanStatuses::Walking;
// 				return MapStatuses::Working;
// 			}
// 			// Если Стенды рядом кончились - Двигаемся дальше
//             if (AroundCurrentHumanIndex == AroundCurrentHuman.cend()) {
// 				CurrentHumanStatus = HumanStatuses::Placed;
// 				return MapStatuses::Working;
// 			}
// 			auto obj = *AroundCurrentHumanIndex;
//             // Если случилось какое-то дерьмо
//             // То это дерьмо случилось
//             // АУФ
//             // Программу стоит выключить
//             if (obj->getOType() != Objects::Stand) {
//                 clearMap();
//                 return MapStatuses::Error;
//             }
//             // Продукты для удаленния
//             std::vector<std::vector<Product>::const_iterator> toDelete;
//             // Проходимся по списку покупок
// 			for (auto tb = CurrentHuman.getContent()->cbegin(); tb != CurrentHuman.getContent()->cend(); tb++) {
//                 // Берём все продукты нужного нам типа
//                 std::vector<const Product*> available;
//                 for (auto sc = obj->getContent()->cbegin(); sc != obj->getContent()->cend(); sc++) {
//                     if (sc->getPType() == tb->getPType()) {
//                         available.push_back(sc.base());
//                     }
//                 }
//                 // Если таких нет - продолжаем
//                 if (available.empty()) {
//                     // Если не нашли нужного - идём дальше
//                     continue;
//                 }
//                 // Выделяем из этого списка подходящий нам
//                 // Коеффициент нужного нам продукта
//                 short ProductCoeff = tb->getPrice() * tb->getAttractiveness();
//                 // Выбранный продукт
//                 std::vector<const Product*>::const_iterator choosen = available.cbegin();
//                 // Выбираем самый подходящий
//                 // Самый близкий по модулю к исходному 
//                 for (auto obj = available.cbegin() + 1; obj != available.cend(); obj++) {
//                     auto i = *obj.base();
//                     auto j = *choosen.base();
//                     short coeff = i->getPrice() * i->getAttractiveness();
//                     short currCoeff = j->getPrice() * i->getAttractiveness();
//                     if (fabs(ProductCoeff - coeff) < fabs(ProductCoeff - currCoeff)) {
//                         choosen = obj;
//                     }
//                 }
//                 CurrentHuman.updateTakenProducts(**choosen);
//                 toDelete.push_back(tb);
// 			}
//             // Нашли всё понравившееся
//             // Теперь смотрим объекты вокруг и с некоторым шансом берём их
//             for (const auto& i: toDelete) {
//                 // Коеффициент. Чем больше - тем больше шанс взять продукт
//                 // Если у продукта привлекательность меньше, то он даже не будет рассмотрен
//                 float coeff = 0.5;
//                 // В левую сторону
//                 for (auto j = i; j != CurrentHuman.getContent()->cbegin() and coeff < 0.9; j--, coeff += 0.17) {
//                     if (!(CurrentHuman.inTakenProducts(*j)) and !(CurrentHuman.inToBuyList(*j))) {
//                         // Смотрим подходит продукт или нет
//                         if (j->getAttractiveness() < coeff) {
//                             continue;
//                         } else {
//                             if (j->getPrice() * j->getAttractiveness() > int(coeff*100) - rand()%100*(1-coeff)) {
//                                 CurrentHuman.updateTakenProducts(*j);
//                             }
//                         }
//                     }
//                 }
//                 coeff = 0.5;
//                 // В правую сторону
//                 for (auto j = i; j != CurrentHuman.getContent()->cend() and coeff < 0.8; j++, coeff += 0.17) {
//                     if (!(CurrentHuman.inTakenProducts(*j)) and !(CurrentHuman.inToBuyList(*j))) {
//                         // Смотрим подходит продукт или нет
//                         if (j->getAttractiveness() < coeff) {
//                             continue;
//                         } else {
//                             if (j->getPrice() * j->getAttractiveness() > int(coeff*100) - rand()%100*(1-coeff)) {
//                                 CurrentHuman.updateTakenProducts(*j);
//                             }
//                         }
//                     }
//                 }
//                 // Удаляем из списка покупок
//                 CurrentHuman.deleteFromBuyList(i);
//             }
//             // Смещаемся на следующий 
//             AroundCurrentHumanIndex++;
//             return MapStatuses::Working;
//         } else if(CurrentHumanStatus == HumanStatuses::Bying) {
//             // Если Объекты вокруг кончились - двигаемся дальше
//             if (AroundCurrentHumanIndex == AroundCurrentHuman.cend()) {
//                 CurrentHumanStatus = HumanStatuses::Placed;
// 				return MapStatuses::Working;
//             }
//             auto cashbox = *AroundCurrentHumanIndex;
//             if (cashbox->getOType() != Objects::CashBox) {
//                 clearMap();
//                 return MapStatuses::Error;
//             }
//             std::vector<std::vector<Product>::const_iterator> toDelete;
//             for (auto i = CurrentHuman.getTakenProducts()->cbegin(); i != CurrentHuman.getTakenProducts()->cend(); i++) {
//                 short price = i->getPrice();
//             }
//             // Доделать
//             // Смащаемся дальше
//             AroundCurrentHumanIndex++;
//         }
//     } else if (status == MapStatuses::Done) {
//         return MapStatuses::Done;
//     } else if (status == MapStatuses::Error) {
//         return MapStatuses::Error;
//     } else {
//         return MapStatuses::Error;
//     }
// }