/********************************************************************************
** Form generated from reading UI file 'MainWindowpCaIuZ.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/
#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QRadioButton>
#include <thread>
#include <chrono>
#include "map.hpp"

QT_BEGIN_NAMESPACE

class Ui_MainWindow : public QMainWindow
{
public:
    QWidget *MAIN;
    QListWidget *ShopMap;
    QLabel *StandContenLabel;
    QListWidget *StandContentList;
    QLabel *HumanNameLabel;
    QListWidget *ToBuyList;
    QListWidget *TakenProductsList;
    QLabel *CashBoxLabel;
    QLabel *MoneyLabel;
    QLabel *ToBuyLabel;
    QLabel *TakenProductsLabbel;
    QLabel *MapLegendLabel;
    QListWidget *MapLegendList;
    QLabel *NicknameLabel;
    QLabel *GitHubLabel;
    QLabel *VKLabel;
    QLabel *OthersLabel;
    QPushButton *PauseButton;
    QPushButton *ContinueButton;
    QPushButton *LoadShopButton;
    QPushButton *QuitButton;
    QRadioButton *LogStatus;
    // Инициализирует все объекты
    void setupUi(QMainWindow *MainWindow);
    // Выставляет всем объектам базовые значеня
    void retranslateUi(QMainWindow *MainWindow);
};

enum class ThreadStatuses {
    // Спит
    Sleeping,
    // Работает
    Running,
    // Подан сигнал на остановка
    Stopping
};

namespace Ui
{
    class Window : public Ui_MainWindow
    {
    private:
        // Очищает ShopMap
        // Очищает ToBuyList
        // Очищает TakenProducts
        void clearAll();
        // Используется для полной остановки магазина
        // Например при смене файла
        void resetWindow();
        MapStatuses MapStatus;
        ThreadStatuses ThreadStatus;
        // Карта
        Map map = Map();
        // Показывает Можно ли дальше ходить или нет
        bool running;
        // Выводит магазин в Его карту
        void printShop();
        // Указатель на текущего человека
        const Human* TheHuman;
        // Печатает список покупок человека
        void printToBuyList();
        // Печатает список взятых продуктов
        void printTakenProducts();
        // Меняет имя на Имя текущего человека
        void printHumanName();
    public:
        // Выводит окно ошибки
        static void loadErrorWindow(const std::string &text, const std::string &hint);
        // Конструктор
        Window();
    };
} // namespace Ui

QT_END_NAMESPACE