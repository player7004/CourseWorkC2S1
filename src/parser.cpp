#include "parser.hpp"

Parser::Parser() = default;

void Parser::clear() {
    AllHumans.clear();
    AllObjects.clear();
}

bool Parser::getProduct(const Json::Value& val, Product& res) {
    // Необходимый нам тип
    auto TypeToUse = Types.find(QString("Product"));
    // Цена
    ushort Price = 0;
    // Имя
    QString Name{};
    // Привлекательность
    float Attractiveness = 0;
    // Тип продукта
    QString PType{};
    // Если типа нет, то прекращает
    if (!val["Type"]) {
        return false;
    }
    // Если тип указан неверно или не тот, что нужен, то прекращаем
    if (Types.find(QString(val["Type"].asCString())) == Types.cend() or Types.find(QString(val["Type"].asCString())) != TypeToUse) {
        return false;
    }
    // Цена
    if (val["Price"]) {
        try {
            Price = val["Price"].asUInt();
        } catch(...) {
            return false;
        }
    } else {
        return false;
    }
    // Имя
    if (val["Name"]) {
        try {
            Name = QString(val["Name"].asCString());
        } catch(...) {
            return false;
        }
    } else  {
        Name = QString("Something");
    }
    // Привлекательность
    if (val["Attractiveness"]) {
        try {
            Attractiveness = val["Attractiveness"].asFloat();
        } catch(...) {
            return false;
        }
    } else {
        return false;
    }
    if (Attractiveness > 100) {
        return false;
    }
    // Тип продукта
    if (val["PType"]) {
        try {
            if (PTypes.find(QString(val["PType"].asCString())) == PTypes.cend()) {
                return false;
            } else {
                PType = QString(val["PType"].asCString());
            }
        } catch(...) {
            return false;
        }
    } else {
        return false;
    }
    res.Name = Name;
    res.Attractiveness = Attractiveness;
    res.Price = Price;
    res.PType = PType;
    return true;
}

bool Parser::getObject(const Json::Value& val, Object& res) {
    // Необходимый нам тип
    auto TypeToUse = Types.find(QString("Object"));
    // Положение
    std::pair<ushort, ushort> Position{0, 0};
    // Размер
    std::pair<ushort, ushort> Size{1, 1};
    // Имя
    QString Name{};
    // Символ
    QChar Symbol{};
    // Тип объекта
    QString OType{};
    // Содержание
    std::vector<Product> Content;
    // Если нет типа, то прекращаем
    if (!val["Type"]) {
        return false;
    }
    // Если тип указан неверно или не тот, что нужен, то прекращаем
    if (Types.find(QString(val["Type"].asCString())) == Types.cend() or Types.find(QString(val["Type"].asCString())) != TypeToUse) {
        return false;
    }
    // Тип объекта
    if (val["OType"]) {
        try {
            if (OTypes.find(QString(val["OType"].asCString())) == OTypes.cend()) {
                return false;
            } else {
                OType = QString(val["OType"].asCString());
            }
        } catch(...) {
            return false;
        }
    } else {
        return false;
    }
    // Если тип продукта - стенд, то берём контент
    if (OTypes.find(QString("Stand")) == OTypes.find(OType)) {
        if (val["Content"]) {
            try {
                for (const auto& i: val["Content"]) {
                    Product res;
                    if (getProduct(i, res)) {
                        Content.push_back(res);
                    } else {
                        continue;
                    }
                }
            } catch(...) {
                return false;
            }
        } else {
            return false;
        }
    }
    // Имя
    if (val["Name"]) {
        try {
            Name = val["Name"].asCString();
        } catch(...) {
            return false;
        }
    } else {
        Name = "Something";
    }
    // Символ
    if (val["Symbol"]) {
        try {
            Symbol = QChar(val["Symbol"].asCString()[0]);
        } catch(...) {
            return false;
        }
    } else {
        Symbol = OType[0];
    }
    // Позиция
    if (val["Position"]) {
        try {
            Position = std::pair<ushort, ushort>{val["Position"][0].asUInt(), val["Position"][1].asUInt()};
        }
        catch(...) {
            return false;
        }
    } else {
        return false;
    }
    // Размер
    if (val["Size"]) {
        try {
            Size = std::pair<ushort, ushort>{val["Size"][0].asUInt(), val["Size"][1].asUInt()};
        }
        catch(...) {
            return false;
        }
    } else {
        return false;
    }
    res.OType = OType;
    res.Content = Content;
    res.Symbol = Symbol;
    res.Size = Size;
    res.Position = Position;
    res.Name = Name;
    return true;
}

bool Parser::getHuman(const Json::Value& val, Human& res) {
    // Необходимый нам тип
    auto TypeToUse = Types.find(QString("Human"));
    // Имя
    QString Name{};
    // Символ
    QChar Symbol{};
    // Путь
    std::vector<std::pair<ushort, ushort>> Way{};
    // Список покупок
    std::vector<Product> ToBuyList{};
    // Список взятых продуктов
    std::vector<Product> TakenProducts{};
    // Кол-во потраченных денег
    ushort Money;
    // Если нет типа, то прекращаем
    if (!val["Type"]) {
        return false;
    }
    // Если тип указан неверно или не тот, что нужен, то прекращаем
    if (Types.find(QString(val["Type"].asCString())) == Types.cend() or Types.find(QString(val["Type"].asCString())) != TypeToUse) {
        return false;
    }
    // Имя
    if (val["Name"]) {
        try {
            Name = QString(val["Name"].asCString());
        } catch(...) {
            return false;
        }
    } else {
        return false;
    }
    // Символ
    if (val["Symbol"]) {
        try {
            Symbol = val["Symbol"].asCString()[0];
        } catch(...) {
            return false;
        }
    } else {
        Symbol = 'H';
    }
    // Список покупок
    if (val["ToBuyList"]) {
        try {
            for (const auto& i: val["ToBuyList"]) {
                Product res;
                if (getProduct(i, res)) {
                    ToBuyList.push_back(res);
                } else {
                    continue;
                }
            }
        } catch(...) {
            return false;
        }
    } else {
        return false;
    }
    // Путь
    if (val["Way"]) {
        try {
            for (const auto& i: val["Way"]) {
                Way.emplace_back(std::pair<ushort, ushort>{i[0].asInt(), i[1].asUInt()});
            }
        } catch(...) {
            return false;
        }
    } else {
        return false;
    }
    // Список взятых продуктов
    if (val["TakenProducts"]) {
        try {
            for (const auto& i: val["TakenProducts"]) {
                Product res;
                if (getProduct(i, res)) {
                    TakenProducts.push_back(res);
                } else {
                    continue;
                }
            }
        } catch(...) {
            return false;
        }
    }
    // Кол-во потраченных денег
    if (val["Money"]) {
        try {
            Money = val["Money"].asUInt();
        } catch (...) {
            return false;
        }
    } else {
        Money = 0;
    }
    res.Name = Name;
    res.Symbol = Symbol;
    res.ToBuyList = ToBuyList;
    res.Way = Way;
    res.TakenProducts = TakenProducts;
    res.Money = Money;
    return true;
}

bool Parser::parse(const QString& file) {
    clear();
    std::ifstream File(file.toStdString());
    if (!File.is_open()) {
        return false;
    }
    Json::Value JFile;
    File >> JFile;
    File.close();
    Json::Value Humans = JFile["Humans"], Objects = JFile["Objects"], TakenProducts = JFile["TakenProducts"];
    if (!Objects) {
        return false;
    }
    // Объекты
    for (const auto& i: Objects) {
        Object res;
        if (getObject(i, res)) {
            AllObjects.push_back(res);
        } else {
            return false;
        }
    }
    // Люди
    if (Humans) {
        for (const auto& i: Humans) {
            Human res;
            if (getHuman(i, res)) {
                AllHumans.push_back(res);
            } else {
                continue;
            }
        }
    }
    // Взятые продукты
    if (TakenProducts) {
        for (const auto& i: TakenProducts) {
            QString name;
            std::vector<Product> taken;
            if (i["Name"]) {
                try {
                    name = i["Name"].asCString();
                } catch (...) {
                    continue;
                }
            }
            if (i["Products"]) {
                try {
                    for (const auto& j: i["Products"]) {
                        Product res;
                        if (getProduct(j, res)) {
                            taken.push_back(res);
                        }
                    }
                } catch(...) {
                }
            }
            AllTakenProducts.insert({name, taken});
        }
    }
    return true;
}

void Saver::saveObject(Json::Value &val, const Object &res) {
    Json::Value result;
    // Сохраняем тип
    result["Type"] = res.Type.toStdString();
    // Сохраняем имя
    result["Name"] = res.Name.toStdString();
    // Сохраняем символ
    result["Symbol"] = QString(res.Symbol).toStdString();
    // Сохраняем ОТип
    result["OType"] = res.OType.toStdString();
    // Сохраняем размер
    Json::Value Size(Json::arrayValue);
    Size.append(res.Size.first);
    Size.append(res.Size.second);
    result["Size"] = Size;
    // Сохраняем Позицию
    Json::Value Position(Json::arrayValue);
    Position.append(res.Position.first);
    Position.append(res.Position.second);
    result["Position"] = Position;
    // Сохранеям содержимое
    Json::Value Content(Json::arrayValue);
    for (const auto& i: res.Content) {
        saveProduct(Content, i);
    }
    result["Content"] = Content;
    val.append(result);
}

void Saver::saveProduct(Json::Value &val, const Product &res) {
    Json::Value result;
    // Сохраняем имя
    result["Name"] = res.Name.toStdString();
    // Сохраняем тип
    result["Type"] = res.Type.toStdString();
    // Сохраняем ПТип
    result["PType"] = res.PType.toStdString();
    // Сохраняем цену
    result["Price"] = res.Price;
    // Сохраняем привлекательность
    result["Attractiveness"] = res.Attractiveness;
    val.append(result);
}

void Saver::saveHuman(Json::Value& val, const Human &res) {
    Json::Value result;
    // Сохраняем тип
    result["Type"] = res.Type.toStdString();
    // Сохраняем имя
    result["Name"] = res.Name.toStdString();
    // Сохраняем Список покупок
    Json::Value ToBuyList(Json::arrayValue);
    for (const auto& i: res.ToBuyList) {
        saveProduct(ToBuyList, i);
    }
    result["ToBuyList"] = ToBuyList;
    // Сохраняем список взятых покупок
    Json::Value TakenProducts(Json::arrayValue);
    for (const auto& i: res.TakenProducts) {
        saveProduct(TakenProducts, i);
    }
    result["TakenProducts"] = TakenProducts;
    // Сохраняем путь
    Json::Value Way(Json::arrayValue);
    for (const auto& i: res.Way) {
        Json::Value sit(Json::arrayValue);
        sit.append(i.first);
        sit.append(i.second);
        Way.append(sit);
    }
    result["Way"] = Way;
    // Сохраняем символ
    result["Symbol"] = QString(res.Symbol).toStdString();
    // Сохраняем деньги
    result["Money"] = res.Money;
    val.append(result);
}

bool Saver::save(const QString &filename, const std::vector<Human>& AllHumans, const std::vector<Object>& AllObjects, const std::map<QString, std::vector<Product>>& TakenProducts) {
    std::ofstream file(filename.toStdString());
    if (!file.is_open()) {
        return false;
    }
    // Здесь хранится общий результат
    Json::Value result;
    // Сюда объекты сохраним
    Json::Value objects(Json::arrayValue);
    // Cюда людей сохраним
    Json::Value humans(Json::arrayValue);
    for (const auto& i: AllHumans) {
        saveHuman(humans, i);
    }
    for (const auto& i: AllObjects) {
        saveObject(objects, i);
    }
    result["Objects"] = objects;
    result["Humans"] = humans;
    if (!TakenProducts.empty()) {
        // Сюда сохраним Все взятые продукты
        Json::Value takenProducts(Json::arrayValue);
        for (const auto& i: TakenProducts) {
            Json::Value sit;
            sit["Name"] = i.first.toStdString();
            Json::Value products(Json::arrayValue);
            for (const auto& j: i.second) {
                saveProduct(products, j);
            }
            sit["Products"] = products;
            takenProducts.append(sit);
        }
        result["TakenProducts"] = takenProducts;
    }
    file << result << std::endl;
    file.flush();
    file.close();
    return true;
}
