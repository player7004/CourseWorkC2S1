#include "src/window.hpp"

int main(int argc, char *argv[]) {
//    try {
//        QApplication a(argc, argv);
//        Window w;
//        return QApplication::exec();
//    } catch(...) {
//        return 1;
//    }
    Map obj;
    obj.open("Shop2.json");
    obj.create();
    std::cout << "Objects: " << std::endl;
    for (const auto& i: obj.AllObjects) {
        std::cout << std::to_string(i) << std::endl;
    }
    std::cout << "Humans: " << std::endl;
    for (const auto& i: obj.AllHumans) {
        std::cout << std::to_string(i);
        std::cout << "ToBuyList: " << std::endl;
        for (const auto& j: i.ToBuyList) {
            std::cout << "\t" << std::to_string(j);
        }
        std::cout << "TakenProducts: " << std::endl;
        for (const auto& j: i.TakenProducts) {
            std::cout << "\t" << std::to_string(j);
        }
    }
    Map::save("result.json", obj.AllHumans, obj.AllObjects);
    obj.clear();
    obj.open("result.json");
    obj.create();
    std::cout << std::endl << std::endl << "Objects: " << std::endl;
    for (const auto& i: obj.AllObjects) {
        std::cout << std::to_string(i) << std::endl;
    }
    std::cout << "Humans: " << std::endl;
    for (const auto& i: obj.AllHumans) {
        std::cout << std::to_string(i);
        std::cout << "ToBuyList: " << std::endl;
        for (const auto& j: i.ToBuyList) {
            std::cout << "\t" << std::to_string(j);
        }
        std::cout << "TakenProducts: " << std::endl;
        for (const auto& j: i.TakenProducts) {
            std::cout << "\t" << std::to_string(j);
        }
    }
}
