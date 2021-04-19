#include "map.hpp"

Map::Map() {
	Status = MapStatus::Initialized;
    CurrentHumanIter = {};
    CurrentHuman = nullptr;
    AroundCurrentHuman = {};
    AroundCurrentHumanIter = {};
    CurrentHumanWayIter = {};
    changeHStatus(HumanStatus::None);
}

Map::~Map() {
}

bool Map::open(const QString& file) {
	clear();
	Parser obj;
	if (obj.parse(file)) {
		AllObjects = obj.AllObjects;
		AllHumans = obj.AllHumans;
		Status = MapStatus::Opened;
		return true;
	}
	else {
		Status = MapStatus::Error;
		return false;
	}
}

bool Map::create() {
    if (Status != MapStatus::Opened) {
        return false;
    }
    auto pos = AllObjects.begin();
    if (pos->OType != "EmptySpace") {
        return false;
    }
    ushort x = pos->Position.first, y = pos->Position.second;
    ushort X = x + pos->Size.first, Y = y + pos->Size.second;
    if (x >= X or y >= Y) {
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
    updateOutMap();
    if (AllHumans.empty()) {
        Status = MapStatus::Done;
    } else {
        CurrentHumanIter = AllHumans.begin();
        initializeHuman();
    }
    return true;
}

void Map::clear() {
	Status = MapStatus::Initialized;
	AllObjects.clear();
	AllHumans.clear();
	clearHuman();
}

void Map::rebuild() {
    if (Status == MapStatus::Initialized) {
        return;
    }
    if (Status == MapStatus::Opened) {
        return;
    }
    if (Status == MapStatus::Created) {
        Status = MapStatus::Working;
    }
    if (Status == MapStatus::Error) {
        return;
    }
    if (Status == MapStatus::Done) {
        updateOutMap();
    }
    if (Status == MapStatus::Working) {
        if (HStatus == HumanStatus::None) {
            initializeHuman();
        } else if (HStatus == HumanStatus::Initialized or HStatus == HumanStatus::Walked) {
            move();
        } else if (HStatus == HumanStatus::Looking) {
            look();
        } else if (HStatus == HumanStatus::Taking) {
            take();
        } else if (HStatus == HumanStatus::Buying) {
            buy();
        } else if (HStatus == HumanStatus::Done or HStatus == HumanStatus::Error) {
            resetHuman();
        } else if(HStatus == HumanStatus::End) {
            return;
        }
    }
}

void Map::initializeHuman() {
    if (CurrentHumanIter == std::vector<Human>::iterator()) {
        Status = MapStatus::Error;
        return;
    }
    if (CurrentHumanIter == AllHumans.end()) {
        Status = MapStatus::Done;
        changeHStatus(HumanStatus::End);
        clearHuman();
    } else {
        CurrentHuman = CurrentHumanIter.base();
        CurrentHumanIter++;
        CurrentHumanWayIter = CurrentHuman->Way.begin();
        updateOutMap();
        changeHStatus(HumanStatus::Initialized);
    }
}

void Map::clearHuman() {
    CurrentHuman = nullptr;
    changeHStatus(HumanStatus::End);
    CurrentHumanIter = {};
    CurrentHumanWayIter = {};
    AroundCurrentHumanIter = {};
    AroundCurrentHuman = {};
}

void Map::resetHuman() {
    CurrentHuman = nullptr;
    changeHStatus(HumanStatus::None);
    AroundCurrentHuman.clear();
    AroundCurrentHumanIter = {};
    CurrentHumanWayIter = {};
}

void Map::move() {
    if (CurrentHumanWayIter == CurrentHuman->Way.end()) {
        if (CurrentHuman->TakenProducts.empty() and CurrentHuman->ToBuyList.empty()) {
            changeHStatus(HumanStatus::Done);
        } else {
            changeHStatus(HumanStatus::Error);
        }
    }
    if (HStatus == HumanStatus::Initialized) {
        placeHuman();
        CurrentHumanWayIter++;
        changeHStatus(HumanStatus::Walked);
        return;
    }
    if (HStatus == HumanStatus::Walked) {
        if (CurrentHuman->ToBuyList.empty() and CurrentHuman->TakenProducts.empty()) {
            placeHuman();
            CurrentHumanWayIter++;
        } else {
            placeHuman();
            CurrentHumanWayIter++;
            changeHStatus(HumanStatus::Looking);
        }
    }
}

void Map::placeHuman() {
    auto pos = CurrentHumanWayIter.base();
    updateOutMap();
    OutMap[pos->first][pos->second] = CurrentHuman->Symbol;
}

void Map::look() {
    if (AroundCurrentHuman.empty() and AroundCurrentHumanIter == std::vector<Object>::iterator()) {
        QString Type;
        if (CurrentHuman->ToBuyList.empty()) {
            Type = "CashBox";
        } else {
            Type = "Stand";
        }
        auto pos = (CurrentHumanWayIter - 1).base();
        // Слева
        if (pos->first - 1 >= 0) {
            if (ObjectMap[pos->first -1][pos->second]->Type == Type) {
                AroundCurrentHuman.push_back(*ObjectMap[pos->first - 1][pos->second]);
            }
        }
        // Справа
        if (pos->first + 1 < ObjectMap.size()) {
            if (ObjectMap[pos->first + 1][pos->second]->Type == Type) {
                AroundCurrentHuman.push_back(*ObjectMap[pos->first + 1][pos->second]);
            }
        }
        // Сверху
        if (pos->second - 1 >= 0) {
            if (ObjectMap[pos->first][pos->second - 1]->OType == Type) {
                AroundCurrentHuman.push_back(*ObjectMap[pos->first][pos->second - 1]);
            }
        }
        // Снизу
        if (pos->first + 1 < ObjectMap.back().size()) {
            if (ObjectMap[pos->first][pos->second + 1]->Type == Type) {
                AroundCurrentHuman.push_back(*ObjectMap[pos->first][pos->second + 1]);
            }
        }
        if (AroundCurrentHuman.empty()) {
            changeHStatus(HumanStatus::Walked);
            return;
        }
        AroundCurrentHumanIter = AroundCurrentHuman.begin();
        if (Type == "Stand") {
            changeHStatus(HumanStatus::Taking);
        } else {
            changeHStatus(HumanStatus::Buying);
        }
    } else {
        AroundCurrentHumanIter++;
        if (AroundCurrentHumanIter == AroundCurrentHuman.end()) {
            changeHStatus(HumanStatus::Walked);
            AroundCurrentHumanIter = {};
            AroundCurrentHuman.clear();
        } else {
            if (AroundCurrentHumanIter.base()->Type == "CashBox") {
                changeHStatus(HumanStatus::Buying);
            } else {
                changeHStatus(HumanStatus::Taking);
            }
        }
    }
}

void Map::take() {
    std::vector<std::vector<Product>::iterator> toDelete;
    for (auto i = CurrentHuman->ToBuyList.begin(); i != CurrentHuman->ToBuyList.end(); i++) {
        std::vector<Product> found;
        // Ищем все подходящие по типу продукты
        for (const auto& j: AroundCurrentHumanIter.base()->Content) {
            if (j.PType == i.base()->PType) {
                found.push_back(j);
            }
        }
        // Если таких нет, то прост идём дальше
        if (found.empty()) {
            continue;
        }
        // Ищем наименьшее значение
        // Т.Е. наилучший для нас вариант
        ushort bestOne = 1 + found.back().Price - (found.back().Price * found.back().Attractiveness);
        auto chosen = found.end() - 1;
        for (auto j = found.begin(); j != found.end(); j++) {
            if (1 + j->Price - j->Price * j->Attractiveness < bestOne) {
                bestOne = 1 + j->Price - j->Price * j->Attractiveness;
                chosen = j;
            }
        }
        toDelete.push_back(i);
        CurrentHuman->TakenProducts.push_back(*chosen.base());
    }
    for (const auto& i: toDelete) {
        CurrentHuman->ToBuyList.erase(i);
    }
    changeHStatus(HumanStatus::Looking);
}

void Map::buy() {
    for (const auto& i: CurrentHuman->TakenProducts) {
        CurrentHuman->Money += i.Price;
    }
    CurrentHuman->TakenProducts.clear();
    changeHStatus(HumanStatus::Looking);
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
}

std::vector<QString> Map::generateMapLegend() {
    std::map<QChar, QString> Dict;
    // Вносим Объекты
    for (const auto& i : AllObjects) {
        if (Dict.find(i.Symbol) == Dict.end()) {
            Dict.insert({ i.Symbol, i.OType });
        }
    }
    // Вносим людей
    for (const auto& i : AllHumans) {
        if (Dict.find(i.Symbol) == Dict.end()) {
            Dict.insert({ i.Symbol, i.Type });
        }
    }
    // Результат
    std::vector<QString> result;
    // Переносим результат
    if (Dict.empty()) {
        result.emplace_back("None");
        return result;
    }
    result.reserve(Dict.size());
    for (const auto& i : Dict) {
        result.emplace_back(i.first + QString(" - ") + i.second);
    }
    return result;
}

std::vector<QString> Map::generateStandContent() {
    std::vector<QString> result;
    if (AroundCurrentHumanIter == std::vector<Object>::iterator()) {
        result.emplace_back("None");
        return result;
    }
    if (AroundCurrentHumanIter.base()->OType == "Stand" and !AroundCurrentHumanIter->Content.empty()) {
        for (const auto& i : AroundCurrentHumanIter.base()->Content) {
            result.emplace_back(std::to_string(i).c_str());
        }
    }
    else {
        result.emplace_back("Empty");
    }
    return result;
}

std::vector<QString> Map::generateToBuyList() {
    std::vector<QString> result;
    if (CurrentHuman == nullptr) {
        result.emplace_back("None");
    }
    else {
        if (CurrentHuman->ToBuyList.empty()) {
            result.emplace_back("Empty");
            return result;
        }
        for (const auto& i : CurrentHuman->ToBuyList) {
            result.emplace_back(std::to_string(i).c_str());
        }
    }
    return result;
}

std::vector<QString> Map::generateTakenProducts() {
    std::vector<QString> result;
    if (CurrentHuman == nullptr) {
        result.emplace_back("None");
    } else if (CurrentHuman->TakenProducts.empty()) {
        result.emplace_back("Empty");
    } else {
        for (const auto& i : CurrentHuman->TakenProducts) {
            result.emplace_back(std::to_string(i).c_str());
        }
    }
    return result;
}

std::vector<QString> Map::generateCurrentHumanInfo() {
    std::vector<QString> result;
    if (CurrentHuman == nullptr) {
        result.emplace_back("None");
    } else {
        result.emplace_back(std::to_string(*CurrentHuman).c_str());
    }
    return result;
}

std::vector<QString> Map::generateAllHumans() {
    std::vector<QString> result;
    if (AllHumans.empty()) {
        result.emplace_back("Empty");
    } else {
        for (const auto& i: AllHumans) {
            result.emplace_back(std::to_string(i).c_str());
        }
    }
    return result;
}

void inline Map::changeHStatus(const HumanStatus& status) {
    HStatus = status;
}

std::string std::to_string(const HumanStatus& status) {
    switch (status) {
        case HumanStatus::None:
            return "None";
        case HumanStatus::Initialized:
            return "Initialized";
        case HumanStatus::Walked:
            return "Walked";
        case HumanStatus::Looking:
            return "Looking";
        case HumanStatus::Taking:
            return "Taking";
        case HumanStatus::Buying:
            return "Buying";
        case HumanStatus::Done:
            return "Done";
        case HumanStatus::End:
            return "End";
        default:
            return "Error";
    }
}

bool Map::save(const QString &filename) {
    return Saver::save(filename, AllHumans, AllObjects);
}