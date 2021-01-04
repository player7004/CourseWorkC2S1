#include "src/window.hpp"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Ui::Window w;
  return a.exec();
}