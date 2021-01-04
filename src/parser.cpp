#include "parser.hpp"

Parser::Parser(const std::string& filename) {
    FileName = filename;
}

std::vector<Human*> Parser::getHumans() const {
    return Humans;
}

std::vector<Object*> Parser::getObjects() const {
    return AllObjects;
}

Entities Parser::getEntityType(const std::string& type) {
    if (type == "Object") {
        return Entities::Object;
    } else if(type == "Product") {
        return Entities::Product;
    } else {
        return Entities::Undefined;
    }
}

Objects Parser::getObjectType(const std::string& otype) {
    if (otype == "EmptySpace") {
        return Objects::EmptySpace;
    } else if (otype == "Door") {
        return Objects::Door;
    } else if (otype == "Stand") {
        return Objects::Stand;
    } else if (otype == "Human") {
        return Objects::Human;
    } else if(otype == "CashBox") {
        return Objects::CashBox;
    } else if(otype == "Wall") {
        return Objects::Wall;
    } else {
        return Objects::Undefined;
    }
}

char Parser::getObjectSymbol(const Objects& otype) {
    if (otype == Objects::EmptySpace) {
        return 'E';
    } else if (otype == Objects::Door) {
        return 'D';
    } else if (otype == Objects::Stand) {
        return 'S';
    } else if (otype == Objects::CashBox) {
        return 'C';
    } else if (otype == Objects::Human) {
        return 'H';
    } else if (otype == Objects::Wall) {
        return 'W';
    } else {
        return 'U';
    }
}

Products Parser::getProductType(const std::string& ptype) {
    if (ptype == "Milk") {
        return Products::Milk;
    } else if (ptype == "Bread") {
        return Products::Bread;
    } else if (ptype == "BuckWheat") {
        return Products::BuckWheat;
    } else if (ptype == "Butter") {
        return Products::Butter;
    } else if (ptype == "Cheese") {
        return Products::Cheese;
    } else if (ptype == "Eggs") {
        return Products::Eggs;
    } else if (ptype == "Fish") {
        return Products::Fish;
    } else if (ptype == "Apples") {
        return Products::Apples; 
    } else if (ptype == "Beer") {
        return Products::Beer;
    } else if (ptype == "Lemon") {
        return Products::Lemon;
    } else if (ptype == "Meet") {
        return Products::Meet;
    } else if (ptype == "ToiletPaper") {
        return Products::ToiletPaper;
    } else if (ptype == "Tomato") {
        return Products::Tomato;
    } else {
        return Products::Undefined;
    }
}

std::vector<Product>* Parser::getProducts(const Json::Value& val) {
    std::vector<Product>* result = new std::vector<Product>;
    for (const auto& obj: val) {
        Entities type;
        Products ptype;
        unsigned short price;
        float attractiveness;
        // Берём тип сущности
        if (obj["Type"]) {
            type = getEntityType(obj["Type"].asString());
        } else {
            std::cerr << "Can`t find product type" << std::endl;
            continue;
        }
        if (type == Entities::Product) {
            // Берём тип продукта
            if (obj["PType"]) {
                ptype = getProductType(obj["PType"].asString());
            } else {
                std::cerr << "Can`t find product type" << std::endl;
                continue;
            }
            // Берём цену продукта
            if (obj["Price"]) {
                price = obj["Price"].asUInt();
            } else {
                std::cerr << "Can`t find product price" << std::endl;
                continue;
            }
            // Берём привлекательность
            if (obj["Attractiveness"]) {
                attractiveness = obj["Attractiveness"].asFloat();
            } else {
                std::cerr << "Can`t find product attractiveness" << std::endl;
                continue;
            }
        } else {
            std::cerr << "Type is Undefined or an Object" << std::endl;
            continue;
        }
        result->emplace_back(Product(ptype, price, attractiveness));
    }
    return result;
}

std::vector<std::pair<unsigned short, unsigned short>>* Parser::getWay(const Json::Value& way) {
    std::vector<std::pair<unsigned short, unsigned short>>* result = new std::vector<std::pair<unsigned short, unsigned short>>;
    for (const auto& obj: way) {
        if (obj[0] and obj[1]) {
            result->emplace_back(std::pair<unsigned short, unsigned short>{obj[0].asUInt(), obj[1].asUInt()});
        }
    }
    return result;
}

bool Parser::parse() {
    std::ifstream file(FileName);
    if (!file.is_open()) {
        std::cerr << FileName + " is unreacheble." << std::endl;
        return false;
    }
    Json::Value JFile;
    file >> JFile;
    file.close();
    Json::Value objects = JFile["Objects"];
    Json::Value humans = JFile["Humans"];
    if (!objects) {
        std::cerr << "Can`t found \"Objects\" in " + FileName << std::endl;
        return false;
    }
    if (!humans) {
        std::cerr << "Can`t found \"Humans\" in " + FileName << std::endl;
        return false;
    }
    if (objects[0]["OType"] != "EmptySpace") {
        std::cerr << "The first object must be EmptySpace. Got " << objects[0]["OType"] << std::endl;
        return false;
    }
    // Проходимся по объектам
    for (const auto& object: objects) {
        Entities type;
        Objects otype;
        std::pair<unsigned short, unsigned short> pos;
        std::pair<unsigned short, unsigned short> size;
        char sym;
        std::vector<Product>* products = nullptr;
        // Берём тип сущности
        if (object["Type"]) {
            type = getEntityType(object["Type"].asString());
        } else {
            std::cerr << "Can`t find entity type" << std::endl;
            continue;
        }
        if (type == Entities::Object) {
            // Берём тип объекта
            if (object["OType"]) {
                otype = getObjectType(object["OType"].asString());
            } else {
                std::cerr << "Can`t finde object type" << std::endl;
                continue;
            }
            // Если не смогли определить тип - пропускаем
            if (otype == Objects::Undefined) {
                std::cerr << "Object type is found but undefined" << std::endl;
                continue;
            }
            // Берём позицию 
            if (object["Position"]) {
                pos = std::pair<unsigned short, unsigned short>{object["Position"][0].asUInt(), object["Position"][1].asUInt()};
            } else {
                std::cerr << "Can`t find object position" << std::endl;
                continue;
            }
            // Берём размер
            if (object["Size"]) {
                size = std::pair<unsigned short, unsigned short>{object["Size"][0].asUInt(), object["Size"][1].asUInt()};
            } else {
                std::cerr << "Can`t find object size" << std::endl;
                continue;
            }
            // Берём символ
            sym = getObjectSymbol(otype);
            // Берём контент
            if (otype == Objects::Stand) {
                if (object["Content"]) {
                    products = getProducts(object["Content"]);
                } else {
                    std::cerr << "Can`t find stand content" << std::endl;
                    continue;
                }
            }
        } else {
            std::cerr << "Type is Undefined or a Product" << std::endl;
            continue;
        }
        if (otype == Objects::Stand) {
            try {
                products->empty();
            } catch (...) {
                std::cerr << "Object type is Stand, but content incomplete" << std::endl;
                continue;
            };
        }
        Object* obj = new Object(pos, size, otype, sym, products);
        AllObjects.push_back(obj);  
    }
    // Проходимся по людям
    for (const auto& human: humans) {
        // Проверочные
        Entities type;
        Objects otype;
        std::string name;
        std::pair<unsigned short, unsigned short> pos{0, 0};
        std::pair<unsigned short, unsigned short> size{1, 1};
        char sym;
        std::vector<Product>* products = nullptr;
        std::vector<std::pair<unsigned short, unsigned short>>* way = nullptr;
        // Берём тип сущности
        if (human["Type"]) {
            type = getEntityType(human["Type"].asString());
        } else {
            std::cerr << "Can`t find entity type" << std::endl;
            continue;
        }
        if (type == Entities::Object) {
            // Берём тип объекта
            if (human["OType"]) {
                otype = getObjectType(human["OType"].asString());
            } else {
                std::cerr << "Can`t finde object type" << std::endl;
                continue;
            }
            // Если не смогли определить тип - пропускаем
            if (otype == Objects::Undefined) {
                std::cerr << "Object type is found but undefined" << std::endl;
                continue;
            }
            // Берём имя
            if (human["Name"]) {
                name = human["Name"].asString();
            } else {
                std::cerr << "Can`t find human name" << std::endl;
                continue;
            }
            // Берём символ
            sym = getObjectSymbol(otype);
            // Берём контент
            if (otype == Objects::Human) {
                if (human["ToBuyList"]) {
                    products = getProducts(human["ToBuyList"]);
                } else {
                    std::cerr << "Can`t find stand content" << std::endl;
                    continue;
                }
            }
            // Берём путь
            if (human["Way"]) {
                way = getWay(human["Way"]);
            } else {
                std::cerr << "Can`t find human way" << std::endl;
                continue;
            }
        } else {
            std::cerr << "Type is Undefined or a Product" << std::endl;
            continue;
        }
        try {
            products->empty();
            way->empty();
        } catch (...) {
            std::cerr << "Human incomplete" << std::endl;
            continue;
        }
        if (!name.empty()) {
            Human* obj = new Human(name, pos, size, products, way);
            Humans.push_back(obj); 
        } else {
            std::cerr << "Human incomplete" << std::endl;
            continue;
        }
    } 
    return true;
}