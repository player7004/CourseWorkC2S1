#include "entity.hpp"

// Object
Object::Object(const std::pair<unsigned short, unsigned short>& position,
                    const std::pair<unsigned short, unsigned short>& size,
                    const Objects& otype,
                    const char& symbol,
                    std::vector<Product>* content) {
                        Type = Entities::Object;
                        Position = position;
                        Size = size;
                        OType = otype;
                        Symbol = symbol;
                        Content = content;
}

std::pair<unsigned short, unsigned short> Object::getPosition() const {
    return Position;
};

std::pair<unsigned short, unsigned short> Object::getSize() const {
    return Size;
};

Objects Object::getOType() const {
    return OType;
}
char Object::getSymbol() const {
    return Symbol;
}

// Product
Product::Product(const Products& ptype,
        unsigned short price,
        float attractiveness) {
            Type = Entities::Product;
            PType = ptype;
            Price = price;
            Attractiveness = attractiveness;
}

Products Product::getPType() const {
    return PType;
}

unsigned short Product::getPrice() const {
    return Price;
}

float Product::getAttractiveness() const {
    return Attractiveness;
}

// Human
Human::Human(const std::string name,
        const std::pair<unsigned short, unsigned short>& position,
        const std::pair<unsigned short, unsigned short>& size,
        std::vector<Product>* tobuylist,
        std::vector<Product>* takenproducts,
        std::vector<std::pair<unsigned short, unsigned short>>* way) :Object(
            position,
            size,
            Objects::Human,
            'H'
        ) {
            Way = way;
            ToBuyList = tobuylist;
            TakenProducts = takenproducts;
}

const std::vector<Product>* Human::getToBuyList() const {
    return ToBuyList;
}

const std::vector<Product>* Human::getTakenProducts() const {
    return TakenProducts;
}

void Human::updateTakenProducts(const Product& product) {
    TakenProducts->push_back(product);
}

void Human::deleteFromToBuyList(const Product& product) {
    int ind = 0;
    bool found = false;
    for (auto i = ToBuyList->begin(); i != ToBuyList->end(); i++) {
        if (i->getPType() == product.getPType()) {
            found = true;
            break;
        }
        ind++;
    }
    if (found) {
        ToBuyList->erase(ToBuyList->begin() + ind);
    }
}

const std::vector<std::pair<unsigned short, unsigned short>>* Human::getWay() const {
    return Way;
}

std::pair<unsigned short, unsigned short> Human::getPos(const int& ind) const {
    if (ind < Way->size()) {

        return *(Way->begin() + ind);
    } else {
        return std::pair<unsigned short, unsigned short>{0, 0};
    }
}

std::string Human::getName() const {
    return Name;
}