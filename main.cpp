#include "src/window.hpp"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   Window w;
   return a.exec();
//   Map test;
//   if (test.open("Shop.json") and test.create()) {
//     for (const auto& y: test.CurrentMap) {
//       for (const auto& x: y) {
//         std::cout << char(x.unicode());
//       }
//       std::cout << std::endl;
//     }
//     return 0;
//   } else {
//     return 1;
//   }
}
