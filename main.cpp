#include "src/window.hpp"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Ui::Window w;
  return a.exec();
//   std::vector<Product>::const_iterator some;
//   if (!some.base()) {
//       std::cout << some.base();
//   }
//   std::vector<Product> test{Product()};
//   some = test.begin();
//   if (some.base()) {
//       std::cout << some.base();
//   }
}