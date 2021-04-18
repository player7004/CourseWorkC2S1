#include "src/window.hpp"

int main(int argc, char *argv[]) {
    try {
        QApplication a(argc, argv);
        Window w;
        return QApplication::exec();
    } catch(...) {
        return 1;
    }
}
