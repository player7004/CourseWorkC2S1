#include "parser.hpp"

Parser::Parser() {
    log.open("Parser.log");
}

void Parser::clear() {
    AllHumans.clear();
    AllObjects.clear();
    log.write("Parser cleared");
}

bool Parser::getProduct(const Json::Value& val, Product& res) {
    log.write("Parsing product");
    // Необходимый нам тип
    std::set<QString>::const_iterator TypeToUse = Types.find(QString("Product"));
    // Цена
    ushort Price = 0;
    // Имя
    QString Name{};
    // Привлекательность
    ushort Attractiveness = 0;
    // Тип
    QString Type{};
    // Тип продукта
    QString PType{};
    // Если типа нет, то прекращает
    if (!val["Type"]) {
        log.write("Product type not found", WriteTypes::Info);
        log.write("Product parsing stopped");
        return false;
    }
    // Если тип указан неверно или не тот, что нужен, то прекращаем
    if (Types.find(QString(val["Type"].asCString())) == Types.cend() or Types.find(QString(val["Type"].asCString())) != TypeToUse) {
        QString str = "Worng type. Expected Product, but got ";
        str += val["Type"].asCString();
        log.write(str, WriteTypes::Info);
        log.write("Product parsing stopped");
        return false;
    }
    // Цена
    if (val["Price"]) {
        log.write("Getting Price");
        try {
            Price = val["Price"].asUInt();
            log.write("Got price");
        } catch(...) {
            log.write("Can`t get price", WriteTypes::Info);
            log.write("Product parsing stopped");
            return false;
        }
    } else {
        log.write("Price not found", WriteTypes::Info);
        log.write("Product parsing stopped");
        return false;
    }
    // Имя
    if (val["Name"]) {
        log.write("Getting Name");
        try {
            Name = QString(val["Name"].asCString());
            log.write("Got name");
        } catch(...) {
            log.write("Can`t get name", WriteTypes::Info);
            log.write("Product parsing stopped");
            return false;
        }
    } else  {
        Name = QString("Something");
        log.write("Name not found, setting standart name");
    }
    // Привлекательность
    if (val["Attractiveness"]) {
        log.write("Getting attractiveness");
        try {
            Attractiveness = val["Attractiveness"].asUInt();
            log.write("Got Attractiveness");
        } catch(...) {
            log.write("Can`t get attractiveness", WriteTypes::Info);
            log.write("Product parsing stopped");
            return false;
        }
    } else {
        log.write("Can`t found attraciveness", WriteTypes::Info);
        log.write("Product parsing stopped");
        return false;
    }
    if (Attractiveness > 100) {
        log.write("Wrong Attractiveness value", WriteTypes::Info);
        log.write("Product parsing stopped");
        return false;
    }
    // Тип продукта
    if (val["PType"]) {
        log.write("Getting PType");
        try {
            if (PTypes.find(QString(val["PType"].asCString())) == PTypes.cend()) {
                log.write("Wrong PType", WriteTypes::Info);
                log.write("Product parsing stopped");
                return false;
            } else {
                PType = QString(val["PType"].asCString());
                log.write("Got PType");
            }
        } catch(...) {
            log.write("Can`t get PType", WriteTypes::Info);
            log.write("Product parsing stopped");
            return false;
        }
    } else {
        log.write("PType not found", WriteTypes::Info);
        log.write("Product parsing stopped");
        return false;
    }
    res.Name = Name;
    res.Attractiveness = Attractiveness;
    res.Price = Price;
    res.PType = PType;
    log.write("Product parsing done");
    return true;
}

bool Parser::getObject(const Json::Value& val, Object& res) {
    log.write("Parsing object");
    // Необходимый нам тип
    std::set<QString>::const_iterator TypeToUse = Types.find(QString("Object"));
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
        log.write("Type not found", WriteTypes::Info);
        log.write("Object parsing stopped");
        return false;
    }
    // Если тип указан неверно или не тот, что нужен, то прекращаем
    if (Types.find(QString(val["Type"].asCString())) == Types.cend() or Types.find(QString(val["Type"].asCString())) != TypeToUse) {
        log.write("Wrong Type", WriteTypes::Info);
        log.write("Object parsing stopped");
        return false;
    }
    // Тип объекта
    if (val["OType"]) {
        log.write("Getting OType");
        try {
            if (OTypes.find(QString(val["OType"].asCString())) == OTypes.cend()) {
                log.write("Wrong OType", WriteTypes::Info);
                log.write("Object parsing stopped");
                return false;
            } else {
                OType = QString(val["OType"].asCString());
                log.write("Got OType");
            }
        } catch(...) {
            log.write("Can`t get OType", WriteTypes::Info);
            log.write("Object parsing stopped");
            return false;
        }
    } else {
        log.write("OType not found");
        log.write("Object parsing stopped");
        return false;
    }
    // Если тип продукта - стенд, то берём контент
    if (OTypes.find(QString("Stand")) == OTypes.find(OType)) {
        if (val["Content"]) {
            log.write("Getting content");
            try {
                for (const auto& i: val["Content"]) {
                    Product res;
                    if (getProduct(i, res)) {
                        Content.push_back(res);
                    } else {
                        continue;
                    }
                    log.write("Got content");
                }
            } catch(...) {
                log.write("Can`t get content", WriteTypes::Info);
                log.write("Object parsing stopped");
                return false;
            }
        } else {
            log.write("Content not found", WriteTypes::Info);
            log.write("Object parsing stopped");
            return false;
        }
    }
    // Имя
    if (val["Name"]) {
        log.write("Geting name");
        try {
            Name = val["Name"].asCString();
            log.write("Got name");
        } catch(...) {
            log.write("Can`t get name", WriteTypes::Info);
            log.write("Object parsing stopped");
            return false;
        }
    } else {
        Name = "Something";
        log.write("Name not found. Setting standart name", WriteTypes::Info);
    }
    // Символ
    if (val["Symbol"]) {
        log.write("Getting symbol");
        try {
            Symbol = QChar(val["Symbol"].asCString()[0]);
            log.write("Got symbol");
        } catch(...) {
            log.write("Can`t get symbol", WriteTypes::Info);
            log.write("Object parsing stopped");
            return false;
        }
    } else {
        Symbol = OType[0];
        log.write("Symbol not found. Setting standart", WriteTypes::Info);
    }
    // Позиция
    if (val["Position"]) {
        log.write("Getting position");
        try {
            Position = std::pair<ushort, ushort>{val["Position"][0].asUInt(), val["Position"][1].asUInt()};
            log.write("Got position");
        }
        catch(...) {
            log.write("Can`t get position");
            log.write("Object parsing stopped");
            return false;
        }
    } else {
        log.write("Position not found", WriteTypes::Info);
        log.write("Object parsing stopped");
        return false;
    }
    // Размер
    if (val["Size"]) {
        log.write("Getting size");
        try {
            Size = std::pair<ushort, ushort>{val["Size"][0].asUInt(), val["Size"][1].asUInt()};
            log.write("Got Size");
        }
        catch(...) {
            log.write("Cant get size", WriteTypes::Info);
            log.write("Object parsing stopped");
            return false;
        }
    } else {
        log.write("Size not found", WriteTypes::Info);
        log.write("Object parsing stopped");
        return false;
    }
    res.OType = OType;
    res.Content = Content;
    res.Symbol = Symbol;
    res.Size = Size;
    res.Position = Position;
    res.Name = Name;
    log.write("Object parsing done");
    return true;
}

bool Parser::getHuman(const Json::Value& val, Human& res) {
    log.write("Starting parsing Human");
    // Необходимый нам тип
    std::set<QString>::const_iterator TypeToUse = Types.find(QString("Human"));
    // Имя
    QString Name{};
    // Символ
    QChar Symbol{};
    // Путь
    std::vector<std::pair<ushort, ushort>> Way{};
    // Список покупок
    std::vector<Product> ToBuyList{};
    // Если нет типа, то прекращаем
    if (!val["Type"]) {
        log.write("Type not found", WriteTypes::Info);
        log.write("Human parsing stopped");
        return false;
    }
    // Если тип указан неверно или не тот, что нужен, то прекращаем
    if (Types.find(QString(val["Type"].asCString())) == Types.cend() or Types.find(QString(val["Type"].asCString())) != TypeToUse) {
        log.write("Wrong type", WriteTypes::Info);
        log.write("Human parsing stopped");
        return false;
    }
    // Имя
    if (val["Name"]) {
        log.write("Getting name");
        try {
            Name = QString(val["Name"].asCString());
            log.write("Got name");
        } catch(...) {
            log.write("Can`t get name", WriteTypes::Info);
            log.write("Human parsing stopped");
            return false;
        }
    } else {
        log.write("Name not found", WriteTypes::Info);
        log.write("Human parsing stopped");
        return false;
    }
    // Символ
    if (val["Symbol"]) {
        log.write("Getting symbol");
        try {
            Symbol = val["Symbol"].asCString()[0];
            log.write("Got symbol");
        } catch(...) {
            log.write("Can`t get symbol", WriteTypes::Info);
            log.write("Human parsing stopped");
            return false;
        }
    } else {
        Symbol = 'H';
        log.write("Symbol not found. Setting standart", WriteTypes::Info);
    }
    // Список покупок
    if (val["ToBuyList"]) {
        log.write("Getting ToBuyList");
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
            log.write("Can`t get ToBuyList", WriteTypes::Info);
            log.write("Human parsing stopped");
            return false;
        }
    } else {
        log.write("ToBuyList not found", WriteTypes::Info);
        log.write("Human parsing stopped");
        return false;
    }
    // Путь
    if (val["Way"]) {
        log.write("Getting name");
        try {
            for (const auto& i: val["Way"]) {
                Way.emplace_back(std::pair<ushort, ushort>{i[0].asInt(), i[1].asUInt()});
            }
        } catch(...) {
            log.write("Can`t get Way", WriteTypes::Info);
            log.write("Human parsing stopped");
            return false;
        }
    } else {
        log.write("Way not found", WriteTypes::Info);
        log.write("Human parsing stopped");
        return false;
    }
    res.Name = Name;
    res.Symbol = Symbol;
    res.ToBuyList = ToBuyList;
    res.Way = Way;
    log.write("Human parsing done");
    return true;
}

bool Parser::parse(const QString& file) {
    log.write("Starting parsing file " + file);
    clear();
    std::ifstream File(file.toStdString());
    if (!File.is_open()) {
        log.write(file + " not found or broken", WriteTypes::Error);
        log.write("Parsing stopped");
        return false;
    }
    Json::Value JFile;
    File >> JFile;
    File.close();
    Json::Value Humans = JFile["Humans"], Objects = JFile["Objects"];
    if (!Objects) {
        log.write("Objects in " + file + " not found", WriteTypes::Error);
        log.write("Parsing stopped");
        return false;
    }
    // Объекты
    log.write("Parsing objects");
    for (const auto& i: Objects) {
        Object res;
        if (getObject(i, res)) {
            AllObjects.push_back(res);
        } else {
            log.write("Can`t parse objects", WriteTypes::Error);
            return false;
        }
    }
    log.write("Parsing objects done");
    // Люди
    if (Humans) {
        log.write("Parsing Humans");
        for (const auto& i: Humans) {
            Human res;
            if (getHuman(i, res)) {
                AllHumans.push_back(res);
            } else {
                continue;
            }
        }
        log.write("Parsing Humans done");
    }
    log.write("Parsing done");
    return true;
}