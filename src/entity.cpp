#include "entity.hpp"

namespace std {
    std::string to_string(const Entities& type) {
        if (type == Entities::Object) {
            return "Object";
        } else if(type == Entities::Product) {
            return "Product";
        } else {
            return "Undefined";
        }
    }

    std::string to_string(const Objects& otype) {
    if (otype == Objects::Wall) {
        return "Wall";
    } else if (otype == Objects::CashBox) {
        return "CashBox";
    } else if (otype == Objects::Door) {
        return "Door";
    } else if (otype == Objects::EmptySpace) {
        return "EmptySpace";
    } else if (otype == Objects::Human) {
        return "Human";
    } else if (otype == Objects::Stand) {
        return "Stand";
    } else {
        return "Undefined";
    }
}

std::string to_string(const Products& ptype) {
    if (ptype == Products::Apples) {
        return "Apples";
    } else if (ptype == Products::Beer) {
        return "Beer";
    } else if (ptype == Products::Bread) {
        return "Bread";
    } else if (ptype == Products::BuckWheat) {
        return "BuckWheat";
    } else if (ptype == Products::Butter) {
        return "Butter";
    } else if (ptype == Products::Cheese) {
        return "Cheese";
    } else if (ptype == Products::Eggs) {
        return "Eggs";
    } else if (ptype == Products::Fish) {
        return "Fish";
    } else if (ptype == Products::Lemon) {
        return "Lemon";
    } else if (ptype == Products::Meet) {
        return "Meet";
    } else if (ptype == Products::Milk) {
        return "Milk";
    } else if (ptype == Products::ToiletPaper) {
        return "ToiletPaper";
    } else if (ptype == Products::Tomato) {
        return "Tomato";
    } else {
        return "Undefined";
            }
    }
}


// Object
Object::Object(const std::pair<unsigned short, unsigned short>& position,
                    const std::pair<unsigned short, unsigned short>& size,
                    const Objects& otype,
                    const char& symbol,
                    std::vector<Product> content) {
                        Type = Entities::Object;
                        Position = position;
                        Size = size;
                        OType = otype;
                        Symbol = symbol;
                        Content = content;
                        Cash = 0;
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

const std::vector<Product>* Object::getContent() const {
    return &Content;
}

unsigned int Object::getCash() const {
    return Cash;
}

std::ostream& operator<<(std::ostream& stream, const Product& prod) {
    stream << std::to_string(prod.getPType()) << " Price: " << prod.getPrice() << " Attractiveness: " << prod.getAttractiveness() << std::endl;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Object& obj) {
    stream << "Position: (" << obj.getPosition().first << "," << obj.getPosition().second << ")" << std::endl
           << "Size: (" << obj.getSize().first << "," << obj.getSize().second << ")" << std::endl
           << "Type: Object" << std::endl
           << "OType: " << std::to_string(obj.getOType()) << std::endl
           << "Symbol: " << obj.getSymbol() << std::endl
           << "Cash: " << obj.getCash() << std::endl
           << "Content: " << std::endl;
    if (obj.getContent() == nullptr) {
        return stream;
    }
    for (std::vector<Product>::const_iterator i = obj.getContent()->begin(); i != obj.getContent()->end(); i++) {
        stream << *i;
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Human& hum) {
    stream << "Name: " << hum.getName() << std::endl
           << "Way: " << std::endl;
    for (std::vector<std::pair<unsigned short, unsigned short>>::const_iterator i = hum.getWay()->begin(); i !=hum.getWay()->end(); i++) {
        stream << "(" << i->first << "," << i->second << ")" << std::endl;
    }
    stream << Object(hum) << std::endl;
    return stream;
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
Human::Human(const std::string& name,
        const std::pair<unsigned short, unsigned short>& position,
        const std::pair<unsigned short, unsigned short>& size,
        std::vector<Product> tobuylist,
        std::vector<std::pair<unsigned short, unsigned short>> way) :Object(
            position,
            size,
            Objects::Human,
            'H',
            tobuylist
        ) {
            Name = name;
            Way = way;
            TakenProducts = std::vector<Product>();
}

Human::Human(): Object() {};

const std::vector<Product>* Human::getTakenProducts() const {
    return &TakenProducts;
}

void Human::updateTakenProducts(const Product& product) {
    TakenProducts.push_back(product);
}

void Human::deleteFromToBuyList(const Product& product) {
    int ind = 0;
    bool found = false;
    for (auto i = Content.begin(); i != Content.end(); i++) {
        if (i->getPType() == product.getPType()) {
            found = true;
            break;
        }
        ind++;
    }
    if (found) {
        Content.erase(Content.begin() + ind);
    }
}

const std::vector<std::pair<unsigned short, unsigned short>>* Human::getWay() const {
    return &Way;
}

std::pair<unsigned short, unsigned short> Human::getPos(const int& ind) const {
    if (ind < Way.size()) {

        return *(Way.begin() + ind);
    } else {
        return std::pair<unsigned short, unsigned short>{0, 0};
    }
}

std::string Human::getName() const {
    return Name;
}