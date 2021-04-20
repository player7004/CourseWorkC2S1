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
#include <QMessageBox>

#include <thread>
#include <chrono>
#include <atomic>
#include "shopEngine.hpp"

enum class ModeStatuses {
    // Ручной режим
    Manual,
    // Автоматический режим
    Auto
};

enum class ToDrawItem {
    // Легенда карты
    MapLegend,
    // Информация о текущем человеке
    HumanInfo,
    // Список покупок
    ToBuyList,
    // Взятые продукты
    TakenProducts,
    // Содержание стенды
    StandContent,
    // Все имеющиеся люди
    AllHumans,
    // Все взятые продукты
    AllTakenProducts
};

enum class ThreadStatuses {
    // Спит
    Sleeping,
    // В процессе работы
    Running,
    // Дана команда на остановку
    Stopping
};

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
    QListWidget* GraphicView;
    // Конструктор
    Window();
    // Закрытие окна
    void closeEvent(QCloseEvent* event) override;

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
    // Очищает окно графики
    void inline resetGraphic() {
        GraphicView->clear();
    };
    // Отрисовывает графику
    void drawGraphics();
    // Отображает лист информации
    void drawInfoList();
    // Поток для автоматического режима
    std::thread* Worker = nullptr;
    // Статус потока
    std::atomic<ThreadStatuses> TStatus;
    // Функция для потока
    static void ThreadFunc(Window* object);
    // Останавливает поток
    void stopThread();
    // Запускает поток
    void startThread();
    // Индикатор работы потока
    std::atomic<bool> running;
    // Карта
    ShopEngine WMap;

    // Открыает окно ошибки загрузки карты
    static int openFileErrorWindow();
    // Открывает окно сохранения в файл
    static int openSaveInFileWindow();
};

QT_END_NAMESPACE