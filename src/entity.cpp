#include "entity.hpp"

bool find(const std::set<QString>& set, const QString& str) {
    if (set.find(str) != set.cend()) {
        return true;
    } else {
        return false;
    }
};

Product::Product() {
    Price = 0;
    Attractiveness = 0;
    PType = "Undefined";
    Name = "Something";
}

Object::Object() {
    Position = std::pair<unsigned short, unsigned short>{0, 0};
    Size = std::pair<unsigned short, unsigned short>{1, 1};
    Name = "Something";
    OType = "Undefined";
    Symbol = 'U';
    Content = std::vector<Product>{};
}

Human::Human() {
    Name = "Someone";
    Symbol = 'H';
    Way = std::vector<std::pair<ushort, ushort>>{};
    ToBuyList = std::vector<Product>{};
    TakenProducts = std::vector<Product>{};
    Money = 0;
}

std::string std::to_string(Object val) {
    string result = "Name: " + val.Name.toStdString() + "\n" + 
    "Type: " + val.Type.toStdString() + "\n" +
    "OType: " + val.OType.toStdString() + "\n" +
    "Symbol: " + char(val.Symbol.unicode()) + "\n" +
    "Position: " + "(" + std::to_string(val.Position.first) + ", " + std::to_string(val.Position.second) + ")\n" +
    "Size: " + "(" + std::to_string(val.Size.first) + ", " + std::to_string(val.Size.second) + ")\n" +
    "Content: \n" ;
    for (const auto& i: val.Content) {
        result += to_string(i) + "\n";
    }
    return result;
}

std::string std::to_string(Human val) {
    string result = "Name: " + val.Name.toStdString() + "\n" +
    "Symbol: " + char(val.Symbol.unicode()) + "\n" +
    "Money: " + std::to_string(val.Money) + "\n" +
    "Way: " + "\n";
    for (const auto& i: val.Way) {
        result += "   ( " + std::to_string(i.first) + ", " + std::to_string(i.second) + ")" + "\n";
    }
    return result;
}

std::string std::to_string(Product val) {
    string result = "Name: " + val.Name.toStdString() + "\n" +
    "Type: " + val.Type.toStdString() + "\n" + 
    "PType: " + val.PType.toStdString() + "\n" + 
    "Price: " + to_string(val.Price) + "\n" +
    "Attractiveness: " + to_string(ushort(val.Attractiveness * 100)) + "\n";
    return result;
}