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
    changeHStatus(HumanStatus::None);
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
    if (Status != MapStatus::Opened) {
        MLog.write("An attempt to create map without opening file", WriteTypes::Info);
        return false;
    }
	MLog.write("Starting to create Map");
    auto pos = AllObjects.begin();
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
    if (AllHumans.empty()) {
        Status = MapStatus::Done;
        MLog.write("No more humans, the work is done");
    } else {
        CurrentHumanIter = AllHumans.begin();
        initializeHuman();
    }
    return true;
}

void Map::clear() {
	MLog.write("The map was cleared");
	Status = MapStatus::Initialized;
	AllObjects.clear();
	AllHumans.clear();
	clearHuman();
}

void Map::rebuild() {
    MLog.write("Rebuilding map");
    MLog.switchPrefix();
    if (Status == MapStatus::Initialized) {
        MLog.write("An attempt to rebuild map, without opening and creating it", WriteTypes::Info);
        return;
    }
    if (Status == MapStatus::Opened) {
        MLog.write("An attempt to rebuild map, without creating it", WriteTypes::Info);
        return;
    }
    if (Status == MapStatus::Created) {
        Status = MapStatus::Working;
        MLog.write("Map is starting to work");
    }
    if (Status == MapStatus::Error) {
        MLog.write("An attempt to rebuild map while an error has occurred", WriteTypes::Info);
        return;
    }
    if (Status == MapStatus::Done) {
        updateOutMap();
        MLog.write("It`s done");
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
    MLog.switchPrefix();
    MLog.write("Rebuilding is done");
}

void Map::initializeHuman() {
    MLog.write("Initializing human");
    if (CurrentHumanIter == std::vector<Human>::iterator()) {
        Status = MapStatus::Error;
        return;
    }
    if (CurrentHumanIter == AllHumans.end()) {
        Status = MapStatus::Done;
        changeHStatus(HumanStatus::End);
        clearHuman();
        MLog.write("No more humans, the work is done");
    } else {
        CurrentHuman = CurrentHumanIter.base();
        CurrentHumanIter++;
        CurrentHumanWayIter = CurrentHuman->Way.begin();
        updateOutMap();
        changeHStatus(HumanStatus::Initialized);
    }
    if (CurrentHuman != nullptr) {
        MLog.write("New human " + CurrentHuman->Name + " is initialized");
    }
}

void Map::clearHuman() {
    MLog.write("Clearing Human");
    CurrentHuman = nullptr;
    changeHStatus(HumanStatus::End);
    CurrentHumanIter = {};
    CurrentHumanWayIter = {};
    AroundCurrentHumanIter = {};
    AroundCurrentHuman = {};
    MLog.write("Human cleared");
}

void Map::resetHuman() {
    MLog.write("Resetting human");
    CurrentHuman = nullptr;
    changeHStatus(HumanStatus::None);
    AroundCurrentHuman.clear();
    AroundCurrentHumanIter = {};
    CurrentHumanWayIter = {};
    MLog.write("Human reset");
}

void Map::move() {
    MLog.write("Moving human");
    if (CurrentHumanWayIter == CurrentHuman->Way.end()) {
        if (CurrentHuman->TakenProducts.empty() and CurrentHuman->ToBuyList.empty()) {
            changeHStatus(HumanStatus::Done);
            MLog.write("All humans passed");
        } else {
            changeHStatus(HumanStatus::Error);
            MLog.write("An error occurred");
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
    MLog.write("Human Moved");
}

void Map::placeHuman() {
    MLog.write("Placing human");
    auto pos = CurrentHumanWayIter.base();
    updateOutMap();
    OutMap[pos->first][pos->second] = CurrentHuman->Symbol;
    MLog.write("Human placed");
}

void Map::look() {
    MLog.write("Human is looking");
    if (AroundCurrentHuman.empty() and AroundCurrentHumanIter == std::vector<Object>::iterator()) {
        QString Type;
        if (CurrentHuman->ToBuyList.empty()) {
            Type = "CashBox";
        } else {
            Type = "Stand";
        }
        MLog.write("Human is looking for " + Type);
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
        MLog.write("Found " + QString(std::to_string(AroundCurrentHuman.size()).c_str()) + " objects");
        AroundCurrentHumanIter = AroundCurrentHuman.begin();
        if (Type == "Stand") {
            changeHStatus(HumanStatus::Taking);
        } else {
            changeHStatus(HumanStatus::Buying);
        }
    } else {
        AroundCurrentHumanIter++;
        if (AroundCurrentHumanIter == AroundCurrentHuman.end()) {
            MLog.write("Everything around is looked");
            changeHStatus(HumanStatus::Walked);
            AroundCurrentHumanIter = {};
            AroundCurrentHuman.clear();
        } else {
            MLog.write("Human is looking for " + AroundCurrentHumanIter.base()->Type);
            if (AroundCurrentHumanIter.base()->Type == "CashBox") {
                changeHStatus(HumanStatus::Buying);
            } else {
                changeHStatus(HumanStatus::Taking);
            }
        }
    }
    MLog.write("Human looked");
}

void Map::take() {
    MLog.write("Human is taking");
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
            MLog.write("Nothing found");
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
        MLog.write("Adding " + chosen->Type + " to Taking products");
        CurrentHuman->TakenProducts.push_back(*chosen.base());
    }
    for (const auto& i: toDelete) {
        CurrentHuman->ToBuyList.erase(i);
        MLog.write("Deleting " + i.base()->Type + " from ToBuyList");
    }
    changeHStatus(HumanStatus::Looking);
    MLog.write("Human took everything");
}

void Map::buy() {
    MLog.write("Human is buying");
    for (const auto& i: CurrentHuman->TakenProducts) {
        CurrentHuman->Money += i.Price;
        MLog.write("Deleting " + i.Type + " from TakenProducts");
    }
    CurrentHuman->TakenProducts.clear();
    changeHStatus(HumanStatus::Looking);
    MLog.write("Human bought");
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
    result.reserve(Dict.size());
    for (const auto& i : Dict) {
        result.emplace_back(i.first + QString(" - ") + i.second);
    }
    MLog.write("Map Legend created");
    return result;
}

std::vector<QString> Map::generateStandContent() {
    MLog.write("Generating stand content");
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
    MLog.write("Stand content generated");
    return result;
}

std::vector<QString> Map::generateToBuyList() {
    MLog.write("Generating ToBuyList");
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
    MLog.write("ToBuyList generated");
    return result;
}

std::vector<QString> Map::generateTakenProducts() {
    MLog.write("Generating TakenProducts");
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
    MLog.write("TakenProducts generated");
    return result;
}

std::vector<QString> Map::generateCurrentHumanInfo() {
    MLog.write("Generating CurrentHumanInfo");
    std::vector<QString> result;
    if (CurrentHuman == nullptr) {
        result.emplace_back("None");
    } else {
        result.emplace_back(std::to_string(*CurrentHuman).c_str());
    }
    MLog.write("CurrentHumanInfo generated");
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
    MLog.write("Human status change to " + QString(std::to_string(status).c_str()));
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