#include "shopEngine.hpp"

ShopEngine::ShopEngine() {
	Status = MapStatus::Initialized;
    CurrentHumanIter = {};
    CurrentHuman = nullptr;
    AroundCurrentHuman = {};
    AroundCurrentHumanIter = {};
    CurrentHumanWay = nullptr;
    BoughtProducts = {};
    changeHStatus(HumanStatus::None);
}

ShopEngine::~ShopEngine() = default;

bool ShopEngine::open(const QString& file) {
	clear();
	Parser obj;
	if (obj.parse(file)) {
		AllObjects = obj.AllObjects;
		AllHumans = obj.AllHumans;
        BoughtProducts = obj.BoughtProducts;
		Status = MapStatus::Opened;
		return true;
	}
	else {
		Status = MapStatus::Error;
		return false;
	}
}

bool ShopEngine::create() {
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

void ShopEngine::clear() {
	Status = MapStatus::Initialized;
	AllObjects.clear();
	AllHumans.clear();
	BoughtProducts.clear();
	OutMap.clear();
	clearHuman();
}

void ShopEngine::rebuild() {
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

void ShopEngine::initializeHuman() {
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
        BoughtProducts.insert({CurrentHuman->Name, {}});
        CurrentHumanWay = &CurrentHuman->Way;
        CurrentHumanIter++;
        updateOutMap();
        changeHStatus(HumanStatus::Initialized);
    }
}

void ShopEngine::clearHuman() {
    CurrentHuman = nullptr;
    changeHStatus(HumanStatus::End);
    CurrentHumanIter = {};
    CurrentHumanWay = nullptr;
    AroundCurrentHumanIter = {};
    AroundCurrentHuman = {};
}

void ShopEngine::resetHuman() {
    CurrentHuman = nullptr;
    changeHStatus(HumanStatus::None);
    AroundCurrentHuman.clear();
    AroundCurrentHumanIter = {};
    // CurrentHumanWayIter = {};
    CurrentHumanWay = nullptr;
}

void ShopEngine::move() {
    if (CurrentHumanWay->empty()) {
        if (CurrentHuman->TakenProducts.empty() and CurrentHuman->ToBuyList.empty()) {
            changeHStatus(HumanStatus::Done);
        } else {
            changeHStatus(HumanStatus::Error);
        }
    }
    if (HStatus == HumanStatus::Initialized) {
        placeHuman();
        CurrentHumanWayLastPos = *CurrentHumanWay->begin();
        CurrentHumanWay->erase(CurrentHumanWay->begin());
        changeHStatus(HumanStatus::Walked);
        return;
    }
    if (HStatus == HumanStatus::Walked) {
        placeHuman();
        CurrentHumanWayLastPos = *CurrentHumanWay->begin();
        CurrentHumanWay->erase(CurrentHumanWay->begin());
        if (!(CurrentHuman->ToBuyList.empty() and CurrentHuman->TakenProducts.empty())) {
            changeHStatus(HumanStatus::Looking);
        }
    }
}

void ShopEngine::placeHuman() {
    auto pos = CurrentHumanWay->begin().base();
    updateOutMap();
    OutMap[pos->first][pos->second] = CurrentHuman->Symbol;
}

void ShopEngine::look() {
    if (AroundCurrentHuman.empty() and AroundCurrentHumanIter == std::vector<Object>::iterator()) {
        QString Type;
        if (CurrentHuman->ToBuyList.empty()) {
            Type = "CashBox";
        } else {
            Type = "Stand";
        }
        // Слева
        if (CurrentHumanWayLastPos.first - 1 >= 0) {
            if (ObjectMap[CurrentHumanWayLastPos.first -1][CurrentHumanWayLastPos.second]->Type == Type) {
                AroundCurrentHuman.push_back(*ObjectMap[CurrentHumanWayLastPos.first - 1][CurrentHumanWayLastPos.second]);
            }
        }
        // Справа
        if (CurrentHumanWayLastPos.first + 1 < ObjectMap.size()) {
            if (ObjectMap[CurrentHumanWayLastPos.first + 1][CurrentHumanWayLastPos.second]->Type == Type) {
                AroundCurrentHuman.push_back(*ObjectMap[CurrentHumanWayLastPos.first + 1][CurrentHumanWayLastPos.second]);
            }
        }
        // Сверху
        if (CurrentHumanWayLastPos.second - 1 >= 0) {
            if (ObjectMap[CurrentHumanWayLastPos.first][CurrentHumanWayLastPos.second - 1]->OType == Type) {
                AroundCurrentHuman.push_back(*ObjectMap[CurrentHumanWayLastPos.first][CurrentHumanWayLastPos.second - 1]);
            }
        }
        // Снизу
        if (CurrentHumanWayLastPos.first + 1 < ObjectMap.back().size()) {
            if (ObjectMap[CurrentHumanWayLastPos.first][CurrentHumanWayLastPos.second + 1]->Type == Type) {
                AroundCurrentHuman.push_back(*ObjectMap[CurrentHumanWayLastPos.first][CurrentHumanWayLastPos.second + 1]);
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

void ShopEngine::take() {
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
        float bestOne = 1.0f + found.back().Price - (found.back().Price * found.back().Attractiveness);
        auto chosen = found.end() - 1;
        for (auto j = found.begin(); j != found.end(); j++) {
            if (1.0f + float(j->Price) - float(j->Price) * j->Attractiveness < bestOne) {
                bestOne = 1.0f + float(j->Price) - float(j->Price) * j->Attractiveness;
                chosen = j;
            }
        }
        // Теперь смотрим дополнительные продукты
        std::vector<std::vector<Product>::iterator> allProducts;
        allProducts.push_back(std::find(AroundCurrentHumanIter->Content.begin(), AroundCurrentHumanIter->Content.end(), *chosen.base()));
        auto attr = chosen->Attractiveness;
        for (auto iter = std::find(AroundCurrentHumanIter->Content.begin(), AroundCurrentHumanIter->Content.end(), *chosen.base());
        iter != AroundCurrentHumanIter->Content.begin() and attr > 0;
        iter--, attr -= 0.21) {
            if (std::find(allProducts.begin(), allProducts.end(), iter) != allProducts.end()  or
            std::find(CurrentHuman->TakenProducts.begin(), CurrentHuman->TakenProducts.end(), *iter) != CurrentHuman->TakenProducts.end()) {
                continue;
            }
            if (ushort(randUshort(0, 100) * (1 + chosen->Attractiveness - iter->Attractiveness)) < ushort(attr * 100)) {
                allProducts.push_back(iter);
            }
        }
        attr = chosen->Attractiveness;
        for (auto iter = std::find(AroundCurrentHumanIter->Content.begin(),AroundCurrentHumanIter->Content.end(), *chosen.base());
                                   iter != AroundCurrentHumanIter->Content.end() and attr > 0;
                                   iter++, attr -= 0.21) {
            if (std::find(allProducts.begin(), allProducts.end(), iter) != allProducts.end()  or
                std::find(CurrentHuman->TakenProducts.begin(), CurrentHuman->TakenProducts.end(), *iter) != CurrentHuman->TakenProducts.end()) {
                continue;
            }
            if (ushort(randUshort(0, 100) * (1 + chosen->Attractiveness - iter->Attractiveness)) < ushort(attr * 100)) {
                allProducts.push_back(iter);
            }
        }
        toDelete.push_back(i);
        for (const auto& p : allProducts) {
            CurrentHuman->TakenProducts.push_back(*p.base());
        }
    }
    for (const auto& i: toDelete) {
        CurrentHuman->ToBuyList.erase(i);
    }
    changeHStatus(HumanStatus::Looking);
}

void ShopEngine::buy() {
    for (const auto& i: CurrentHuman->TakenProducts) {
        CurrentHuman->Money += i.Price;
        BoughtProducts[CurrentHuman->Name].push_back(i);
    }
    CurrentHuman->TakenProducts.clear();
    changeHStatus(HumanStatus::Looking);
}

void ShopEngine::updateOutMap() {
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

std::vector<QString> ShopEngine::generateMapLegend() {
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

std::vector<QString> ShopEngine::generateStandContent() {
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

std::vector<QString> ShopEngine::generateToBuyList() {
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

std::vector<QString> ShopEngine::generateTakenProducts() {
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

std::vector<QString> ShopEngine::generateCurrentHumanInfo() {
    std::vector<QString> result;
    if (CurrentHuman == nullptr) {
        result.emplace_back("None");
    } else {
        result.emplace_back(std::to_string(*CurrentHuman).c_str());
    }
    return result;
}

std::vector<QString> ShopEngine::generateAllHumans() {
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

void inline ShopEngine::changeHStatus(const HumanStatus& status) {
    HStatus = status;
}

bool ShopEngine::save(const QString &filename) {
    return Saver::save(filename, AllHumans, AllObjects, BoughtProducts);
}

ushort ShopEngine::randUshort(const ushort &a, const ushort &b) {
    std::mt19937_64 gen(time(NULL));
    std::uniform_int_distribution<ushort> range(a, b + 1);
    return range(gen);
}

std::vector<QString> ShopEngine::generateBoughtProducts() {
    std::vector<QString> result;
    if (AllHumans.empty()) {
        result.emplace_back("None");
        return result;
    }
    if (BoughtProducts.empty()) {
        result.emplace_back("Empty");
        return result;
    }
    for (const auto &i : BoughtProducts) {
        result.push_back(i.first);
        if (i.second.empty()) {
            result.emplace_back("Empty");
        }
        for (const auto& j: i.second) {
            result.emplace_back(std::to_string(j).c_str());
        }
    }
    return result;
}