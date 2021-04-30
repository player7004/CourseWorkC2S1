#pragma once

#include <QtCore/QVariant>
#include <QtWidgets>
#include <QMessageBox>

#include <thread>
#include <chrono>
#include <atomic>
#include "shopEngine.hpp"

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
    BoughtProducts
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
    // Окно с графикой
    QListWidget* GraphicView;
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
    // Отображаемый объект
    ToDrawItem Item;
    // Очищает окно графики
    void inline resetGraphic() {
        for (auto& i: Modules) {
            i.second->setText("");
        }
    };
    // Отрисовывает графику
    void drawGraphics();
    // Отображает лист информации
    void drawInfoList();
    // Карта
    ShopEngine WMap;

    // Модули оторажения и их содержимое
    std::vector<std::pair<std::unique_ptr<QListWidgetItem>, std::unique_ptr<QLabel>>> Modules;
    // Создаёт модули отображение
    void createModules();

    // Открыает окно ошибки загрузки карты
    static int openFileErrorWindow();
    // Открывает окно сохранения в файл
    static int openSaveInFileWindow();
};

QT_END_NAMESPACE