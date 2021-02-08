/********************************************************************************
** Form generated from reading UI file 'MainWindowpCaIuZ.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/
#pragma once

#include <QtCore/QVariant>
#include <QtWidgets>

#include <thread>
#include <chrono>
#include "map.hpp"

enum class ModeStatuses {
    // Ручной режим
    Manual,
    // Автоматический режим
    Auto
};

enum class ToDrawItem {
    // Список покупок
    ToBuyList = 0,
    // Взятые продукты
    TakenProducts,
    // Содержание стенды
    StandContent,
    // Легенда карты
    MapLegend
};

enum class ThreadStatuses {
    // Спит
    Sleeping,
    // В процессе работы
    Running,
    // Дана команда на остановку
    Stopping
};

namespace std {
    // преобразование в строку для ToDrawItem
    string to_string(ToDrawItem val);
    // преобразование в строку для ModeStatuses
    string to_string(ModeStatuses val);
}

QT_BEGIN_NAMESPACE

class Window: public QWidget {
public:
    // Устанавливает все базовые значения для интерфейса
    void setupUI();
    // Коннектит все кнопки с их функциями
    void connectUI();
    // Меню
    QComboBox* MenuBox;
    // Загрузить магазин
    QPushButton* LoadShopButton;
    // Следующий шаг
    QPushButton* NextStepButton;
    // Лист для отображения информации
    QListWidget* InfoList;
    // Кнопка выйти
    QPushButton* QuitButton;
    // Кнопка ручного режима
    QRadioButton* ManualModeButton;
    // Кнопка сохранения в файл
    QRadioButton* AutoModeButton;
    // Окно с графикой
    QGraphicsView* GraphicView;
    // Указывает задержку автоматического режима
    QSpinBox* DelaySpin;
    // Надпись
    QLabel* DelayLabel;
    // Конструктор
    Window();

    // Устанавливает базовые значения
    // Mode
    // Item
    // Delay
    // MapObject
    void setupBase();
    // Очищает окно для загрузки нового магазина
    void clearWindow();
    // Режим переключения
    ModeStatuses Mode;
    // Отображаемый объект
    ToDrawItem Item;
    // Лог окна
    Log WLog;
    // Задержка автоматического режима
    unsigned short Delay;
    // Карта
    // Map WMap;
};

// class Ui_MainWindow : public QMainWindow
// {
// public:
//     QWidget *MAIN;
//     QListWidget *ShopMap;
//     QLabel *StandContenLabel;
//     QListWidget *StandContentList;
//     QLabel *HumanNameLabel;
//     QListWidget *ToBuyList;
//     QListWidget *TakenProductsList;
//     QLabel *CashBoxLabel;
//     QLabel *MoneyLabel;
//     QLabel *ToBuyLabel;
//     QLabel *TakenProductsLabbel;
//     QLabel *MapLegendLabel;
//     QListWidget *MapLegendList;
//     QLabel *NicknameLabel;
//     QLabel *GitHubLabel;
//     QLabel *VKLabel;
//     QLabel *OthersLabel;
//     QPushButton *PauseButton;
//     QPushButton *ContinueButton;
//     QPushButton *LoadShopButton;
//     QPushButton *QuitButton;
//     QRadioButton *LogStatus;
//     // Инициализирует все объекты
//     void setupUi(QMainWindow *MainWindow);
//     // Выставляет всем объектам базовые значеня
//     void retranslateUi(QMainWindow *MainWindow);
// };

// enum class ThreadStatuses {
//     // Спит
//     Sleeping,
//     // Работает
//     Running,
//     // Подан сигнал на остановка
//     Stopping
// };

// namespace Ui
// {
//     class Window : public Ui_MainWindow
//     {
//     private:
//         // Очищает ShopMap
//         // Очищает ToBuyList
//         // Очищает TakenProducts
//         void clearAll();
//         // Используется для полной остановки магазина
//         // Например при смене файла
//         void resetWindow();
//         MapStatuses MapStatus;
//         ThreadStatuses ThreadStatus;
//         // Карта
//         Map map = Map();
//         // Показывает Можно ли дальше ходить или нет
//         bool running;
//         // Выводит магазин в Его карту
//         void printShop();
//         // Указатель на текущего человека
//         const Human* TheHuman;
//         // Печатает список покупок человека
//         void printToBuyList();
//         // Печатает список взятых продуктов
//         void printTakenProducts();
//         // Меняет имя на Имя текущего человека
//         void printHumanName();
//     public:
//         // Выводит окно ошибки
//         static void loadErrorWindow(const std::string &text, const std::string &hint);
//         // Конструктор
//         Window();
//     };
// } // namespace Ui

QT_END_NAMESPACE