//
// Created by player7004 on 04.12.2020.
//

#include "window.hpp"

Window::Window() {
    WLog.write("Window Initializing");
    try {
        setupBase();
        setupUI();
        connectUI();
    } catch (...) {
        WLog.write("Can`t initialize window", WriteTypes::Error);
        this->close();
    }
}

void Window::connectUI() {
    WLog.write("Connecting Buttons");
    // Закрывает окно
    auto QuitFunc = [this]{
        this->close();
        WLog.write("Closing window");
    };
    connect(QuitButton, &QPushButton::clicked, QuitFunc);

    // Открывает файл
    // auto LoadFunc = [this] {
    //     // WLog.write("Loaded new file: " + file);
    // };
    // connect(LoadShopButton, &QPushButton::clicked, LoadFunc);

    // Меняет режим работы на ручной
    auto SetManualModeFunc = [this] {
        this->Mode = ModeStatuses::Manual;
        WLog.write("Mode set to Manual");
    };
    // Меняет режим работы на автоматический
    auto SetAutoModeFunc = [this] {
        this->Mode = ModeStatuses::Auto;
        WLog.write("Mode set to Auto");
    };
    connect(ManualModeButton, &QRadioButton::clicked, SetManualModeFunc);
    connect(AutoModeButton, &QRadioButton::clicked, SetAutoModeFunc);

    // Меняет объект отображения
    auto SetToDrawItemFunc = [this](int index) {
        this->Item = static_cast<ToDrawItem>(index);
        // this->drawItem();
        QString str = std::to_string(Item).c_str();
        WLog.write("Item to draw set to " + str);
    };
    connect(MenuBox, QOverload<int>::of(&QComboBox::currentIndexChanged), SetToDrawItemFunc);

    // Меняет задержку автоматического режима на указанную
    auto SetDelayFunc = [this](int delay) {
        Delay = delay;
        QString str = "Delay changed to: ";
        str += std::to_string(delay).c_str();
        WLog.write(str);
    };
    connect(DelaySpin, QOverload<int>::of(&QSpinBox::valueChanged), SetDelayFunc);

    // Меняет карту
    auto LoadNewFileFunc = [this] {
        QString str = QFileDialog::getOpenFileName(this, "Choose Shop file", "", "*.json");
        this->clearWindow();
        //this->MapObject = Map(str);
        WLog.write("File changed to " + str);
    };
    connect(LoadShopButton, &QPushButton::clicked, LoadNewFileFunc);

    // Делает следующий шаг
    auto NextStepFunc = [this] {
        // rebuild map
        WLog.write("Next step button was clicked");
    };
    connect(NextStepButton, &QPushButton::clicked, NextStepFunc);

    WLog.write("UI connected");
}

void Window::setupUI() {
    WLog.write("Setting up UI");
    // Основное окно
    this->resize(500, 400);
    this->setMinimumSize(500, 400);
    this->setMaximumSize(500, 400);
    // Меню
    MenuBox = new QComboBox(this);
    MenuBox->setGeometry(335, 40, 160, 30);
    MenuBox->addItem("Список покупок");
    MenuBox->addItem("Взятые продукты");
    MenuBox->addItem("Содержание стенда");
    MenuBox->addItem("Легенда карты");
    // Кнопка Смены файла
    LoadShopButton = new QPushButton(this);
    LoadShopButton->setGeometry(335, 365, 78, 30);
    LoadShopButton->setText(QString("Открыть"));
    // Кнопка Дальше
    NextStepButton = new QPushButton(this);
    NextStepButton->setGeometry(335, 5, 160, 30);
    NextStepButton->setText(QString("Дальше"));
    // Лист с отображаемыми элементами
    InfoList = new QListWidget(this);
    InfoList->setGeometry(335, 80, 160, 280);
    // Кнопка выхода
    QuitButton = new QPushButton(this);
    QuitButton->setGeometry(417, 365, 80, 30);
    QuitButton->setText(QString("Выход"));
    // Кнопка ручного режима
    ManualModeButton = new QRadioButton(this);
    ManualModeButton->setText(QString("Ручной режим"));
    ManualModeButton->setGeometry(5, 330, 160, 30 );
    ManualModeButton->setChecked(true);
    // Кнопка Автоматического режима
    AutoModeButton = new QRadioButton(this);
    AutoModeButton->setGeometry(5, 365, 200, 30);
    AutoModeButton->setText(QString("Автоматический режим"));
    // Окно просматра графики
    GraphicView = new QGraphicsView(this);
    GraphicView->setGeometry(5, 5, 325, 325);
    // Окно изменения задержки
    DelaySpin = new QSpinBox(this);
    DelaySpin->setGeometry(280, 365, 50, 30);
    DelaySpin->setValue(2);
    DelaySpin->setMaximum(60);
    DelaySpin->setMinimum(1);
    // Надпись Задержка(с): 
    DelayLabel = new QLabel(this);
    DelayLabel->setText(QString("Задержка(с): "));
    DelayLabel->setGeometry(200, 330, 120, 30);
    QFont font;
    font.setPointSize(14);
    DelayLabel->setFont(font);
    
    this->show();

    WLog.write("UI set up");
}

void Window::setupBase() {
    Mode = ModeStatuses::Manual;
    Item = ToDrawItem::MapLegend;
    Delay = 2;
    WLog.open("Window.log");
    // MapObject = Map();

    WLog.write("Base set up");
}

void Window::clearWindow() {
    InfoList->clear();
    // reset graphic
    WLog.write("Window cleared");
}

std::string std::to_string(ToDrawItem val) {
    switch (val)
    {
    case ToDrawItem::ToBuyList:
        return "ToBuyList";
    case ToDrawItem::MapLegend:
        return "MapLegend";
    case ToDrawItem::StandContent:
        return "StandContent";
    case ToDrawItem::TakenProducts:
        return "TakenProducts";
    default:
        return "";
    }
}

std::string std::to_string(ModeStatuses val) {
    switch (val)
    {
    case ModeStatuses::Auto:
        return "Auto";
    case ModeStatuses::Manual:
        return "Manual";
    default:
        break;
    }
}

// void Ui_MainWindow::setupUi(QMainWindow *MainWindow)
// {
//     if (MainWindow->objectName().isEmpty())
//         MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
//     MainWindow->resize(800, 600);
//     MainWindow->setMinimumSize(QSize(800, 600));
//     MainWindow->setMaximumSize(QSize(800, 600));
//     MAIN = new QWidget(MainWindow);
//     MAIN->setObjectName(QString::fromUtf8("MAIN"));
//     ShopMap = new QListWidget(MAIN);
//     ShopMap->setObjectName(QString::fromUtf8("ShopMap"));
//     ShopMap->setGeometry(QRect(10, 10, 321, 321));
//     QFont font;
//     font.setFamily(QString::fromUtf8("DejaVu Sans Mono"));
//     font.setBold(true);
//     font.setWeight(75);
//     ShopMap->setFont(font);
//     StandContenLabel = new QLabel(MAIN);
//     StandContenLabel->setObjectName(QString::fromUtf8("StandContenLabel"));
//     StandContenLabel->setGeometry(QRect(10, 340, 321, 20));
//     QFont font1;
//     font1.setPointSize(14);
//     StandContenLabel->setFont(font1);
//     StandContentList = new QListWidget(MAIN);
//     StandContentList->setObjectName(QString::fromUtf8("StandContentList"));
//     StandContentList->setGeometry(QRect(10, 370, 321, 221));
//     StandContentList->setFont(font);
//     HumanNameLabel = new QLabel(MAIN);
//     HumanNameLabel->setObjectName(QString::fromUtf8("HumanNameLabel"));
//     HumanNameLabel->setGeometry(QRect(340, 10, 121, 21));
//     QFont font2;
//     font2.setPointSize(12);
//     HumanNameLabel->setFont(font2);
//     ToBuyList = new QListWidget(MAIN);
//     ToBuyList->setObjectName(QString::fromUtf8("ToBuyList"));
//     ToBuyList->setGeometry(QRect(340, 70, 221, 261));
//     ToBuyList->setFont(font);
//     TakenProductsList = new QListWidget(MAIN);
//     TakenProductsList->setObjectName(QString::fromUtf8("TakenProductsList"));
//     TakenProductsList->setGeometry(QRect(570, 70, 221, 261));
//     TakenProductsList->setFont(font);
//     CashBoxLabel = new QLabel(MAIN);
//     CashBoxLabel->setObjectName(QString::fromUtf8("CashBoxLabel"));
//     CashBoxLabel->setGeometry(QRect(470, 10, 151, 21));
//     CashBoxLabel->setFont(font2);
//     MoneyLabel = new QLabel(MAIN);
//     MoneyLabel->setObjectName(QString::fromUtf8("MoneyLabel"));
//     MoneyLabel->setGeometry(QRect(630, 10, 161, 21));
//     MoneyLabel->setFont(font1);
//     ToBuyLabel = new QLabel(MAIN);
//     ToBuyLabel->setObjectName(QString::fromUtf8("ToBuyLabel"));
//     ToBuyLabel->setGeometry(QRect(340, 40, 221, 21));
//     ToBuyLabel->setFont(font1);
//     TakenProductsLabbel = new QLabel(MAIN);
//     TakenProductsLabbel->setObjectName(QString::fromUtf8("TakenProductsLabbel"));
//     TakenProductsLabbel->setGeometry(QRect(570, 40, 221, 21));
//     TakenProductsLabbel->setFont(font1);
//     MapLegendLabel = new QLabel(MAIN);
//     MapLegendLabel->setObjectName(QString::fromUtf8("MapLegendLabel"));
//     MapLegendLabel->setGeometry(QRect(340, 340, 221, 20));
//     MapLegendLabel->setFont(font1);
//     MapLegendList = new QListWidget(MAIN);
//     MapLegendList->setObjectName(QString::fromUtf8("MapLegendList"));
//     MapLegendList->setGeometry(QRect(340, 370, 221, 221));
//     MapLegendList->setFont(font);
//     NicknameLabel = new QLabel(MAIN);
//     NicknameLabel->setObjectName(QString::fromUtf8("NicknameLabel"));
//     NicknameLabel->setGeometry(QRect(718, 530, 80, 20));
//     GitHubLabel = new QLabel(MAIN);
//     GitHubLabel->setObjectName(QString::fromUtf8("GitHubLabel"));
//     GitHubLabel->setGeometry(QRect(603, 570, 221, 20));
//     VKLabel = new QLabel(MAIN);
//     VKLabel->setObjectName(QString::fromUtf8("VKLabel"));
//     VKLabel->setGeometry(QRect(628, 550, 191, 20));
//     OthersLabel = new QLabel(MAIN);
//     OthersLabel->setObjectName(QString::fromUtf8("OthersLabel"));
//     OthersLabel->setGeometry(QRect(570, 340, 221, 20));
//     OthersLabel->setFont(font1);
//     PauseButton = new QPushButton(MAIN);
//     PauseButton->setObjectName(QString::fromUtf8("PauseButton"));
//     PauseButton->setGeometry(QRect(570, 370, 111, 28));
//     ContinueButton = new QPushButton(MAIN);
//     ContinueButton->setObjectName(QString::fromUtf8("ContinueButton"));
//     ContinueButton->setGeometry(QRect(682, 370, 111, 28));
//     LoadShopButton = new QPushButton(MAIN);
//     LoadShopButton->setObjectName(QString::fromUtf8("LoadShopButton"));
//     LoadShopButton->setGeometry(QRect(570, 422, 223, 28));
//     QuitButton = new QPushButton(MAIN);
//     QuitButton->setObjectName(QString::fromUtf8("QuitButton"));
//     QuitButton->setGeometry(570, 452, 223, 28);
//     LogStatus = new QRadioButton(MAIN);
//     LogStatus->setObjectName(QString::fromUtf8("LogStatusRadioButton"));
//     LogStatus->setGeometry(570, 400, 200, 20);
//     MainWindow->setCentralWidget(MAIN);

//     retranslateUi(MainWindow);

//     QMetaObject::connectSlotsByName(MainWindow);
// }

// void Ui_MainWindow::retranslateUi(QMainWindow *MainWindow)
// {
//     MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
//     StandContenLabel->setText(QCoreApplication::translate("MainWindow", "Stand content:", nullptr));
//     HumanNameLabel->setText(QCoreApplication::translate("MainWindow", "HumanName", nullptr));
//     CashBoxLabel->setText(QCoreApplication::translate("MainWindow", "CashBox Money: ", nullptr));
//     MoneyLabel->setText(QCoreApplication::translate("MainWindow", "Money", nullptr));
//     ToBuyLabel->setText(QCoreApplication::translate("MainWindow", "To Buy:", nullptr));
//     TakenProductsLabbel->setText(QCoreApplication::translate("MainWindow", "Taken Products: ", nullptr));
//     MapLegendLabel->setText(QCoreApplication::translate("MainWindow", "ShopMap Legend:", nullptr));
//     NicknameLabel->setText(QCoreApplication::translate("MainWindow", "player7004", nullptr));
//     GitHubLabel->setText(QCoreApplication::translate("MainWindow", "https://github.com/player7004", nullptr));
//     VKLabel->setText(QCoreApplication::translate("MainWindow", "https://vk.com/player7004", nullptr));
//     OthersLabel->setText(QCoreApplication::translate("MainWindow", "Others:", nullptr));
//     PauseButton->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
//     ContinueButton->setText(QCoreApplication::translate("MainWindow", "Continue", nullptr));
//     LoadShopButton->setText(QCoreApplication::translate("MainWindow", "Load Shop", nullptr));
//     QuitButton->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
//     LogStatus->setText(QCoreApplication::translate("MainWindow", "Save result in file", nullptr));
// }

// namespace Ui
// {
//     Window::Window()
//     {
//         setupUi(this);
//         this->show();
//         running = false;
//         TheHuman = nullptr;
//         connect(LoadShopButton, &QPushButton::clicked, [this]{
//             resetWindow();
//             std::string str = QFileDialog::getOpenFileName(this, "Choose Shop file", "", "*.json").toStdString();
//             map = Map(str);
//             MapStatus = MapStatuses::NotInitialized;
//             std::thread some([this]{
//                 this->running = true;
//                 this->ThreadStatus = ThreadStatuses::Running;
//                 while (this->ThreadStatus == ThreadStatuses::Running) {
//                     if (this->running) {
//                         this->TheHuman = this->map.getCurrentHuman();
//                         this->MapStatus = this->map.rebuildMap(this->MapStatus);
//                         this->printShop();
//                         this->printToBuyList();
//                         this->printTakenProducts();
//                         this->printHumanName();
//                         if (this->MapStatus == MapStatuses::Error) {
//                             this->ThreadStatus = ThreadStatuses::Stopping;
//                         } else if (this->MapStatus == MapStatuses::Done) {
//                             this->ThreadStatus = ThreadStatuses::Stopping;
//                         }
//                     }
//                     std::this_thread::sleep_for(std::chrono::seconds(2));
//                 }
//                 this->ThreadStatus = ThreadStatuses::Sleeping;
//             });
//             some.detach();
//         });
//         connect(PauseButton, &QPushButton::clicked, [this]() {
//             this->running = false;
//         });
//         connect(ContinueButton, &QPushButton::clicked, [this]() {
//             this->running = true;
//         });
//         connect(QuitButton, &QPushButton::clicked, [this]() {
//             this->running = false;
//             this->resetWindow();
//             this->close();
//         });
//     }

//     void Window::printShop() {
//         clearAll();
//         const std::vector<std::vector<char>>* const thing = map.getCurrentMap();
//         for (std::vector<std::vector<char>>::const_iterator i = thing->begin(); i != thing->end(); i++) {
//             std::string result;
//             for (std::vector<char>::const_iterator j = i->begin(); j != i->end(); j++) {
//                 result += *j;
//             }
//             ShopMap->addItem(QString(result.c_str()));
//         }
//     }

//     void Window::resetWindow() {
//         map = Map();
//         running = false;
//         MapStatus = MapStatuses::NotInitialized;
//         if (ThreadStatus == ThreadStatuses::Running) {
//             ThreadStatus = ThreadStatuses::Stopping;
//             while (ThreadStatus != ThreadStatuses::Sleeping);
//         }
//         clearAll();
//     }

//     void Window::loadErrorWindow(const std::string &text, const std::string &hint)
//     {
//         // Инициализация самого окна
//         auto *obj = new QWidget(nullptr);
//         obj->setAttribute(Qt::WA_DeleteOnClose);
//         obj->setAttribute(Qt::WA_ShowModal);
//         obj->resize(400, 150);
//         obj->setMinimumSize(400, 150);
//         obj->setMaximumSize(400, 150);
//         // Инициализация кнопки
//         auto *butt = new QPushButton(obj);
//         butt->setGeometry(10, 115, 380, 30);
//         butt->setText(QString("Quit"));
//         connect(butt, &QPushButton::clicked, [obj]() {
//             obj->close();
//         });
//         // Инициализация шрифта
//         QFont font;
//         font.setPointSize(14);
//         // Иницаиализация надписей
//         // Надпись Что=то случилось
//         auto *constLabel = new QLabel(obj);
//         constLabel->setGeometry(10, 5, 380, 25);
//         constLabel->setFont(font);
//         constLabel->setText("<b>Something unexpected happened: </b>");
//         // Текст ошибки
//         auto *errorTextLabel = new QLabel(obj);
//         QPalette errorColors;
//         errorColors.setColor(QPalette::Window, Qt::white);
//         errorColors.setColor(QPalette::WindowText, Qt::red);
//         errorTextLabel->setGeometry(30, 35, 350, 25);
//         errorTextLabel->setFont(font);
//         errorTextLabel->setPalette(errorColors);
//         errorTextLabel->setText(QString(text.c_str()));
//         // Совет по исправлению
//         auto *hintLabel = new QLabel(obj);
//         QPalette hintColors;
//         hintColors.setColor(QPalette::Window, Qt::white);
//         hintColors.setColor(QPalette::WindowText, Qt::green);
//         hintLabel->setPalette(hintColors);
//         hintLabel->setGeometry(30, 65, 350, 25);
//         hintLabel->setFont(font);
//         hintLabel->setText(QString(hint.c_str()));
//         // Показ окна
//         obj->show();
//     }

//     void Window::printToBuyList() {
//         if (TheHuman == nullptr) {
//             return;
//         }
//         auto list = TheHuman->getContent();
//         ToBuyList->clear();
//         for (auto i = list->begin(); i != list ->end(); i++) {
//             std::string data = std::to_string(i->getPType()) + " Price: " + std::to_string(i->getPrice()) + " Attractiveness: " + std::to_string(int(i->getAttractiveness()*100));
//             ToBuyList->addItem(QString(data.c_str()));
//         }
//     }

//     void Window::printTakenProducts() {
//         if (TheHuman == nullptr) {
//             return;
//         }
//         auto list = TheHuman->getTakenProducts();
//         TakenProductsList->clear();
//         for (auto i = list->begin(); i != list->end(); i++) {
//             std::string data = std::to_string(i->getPType()) + " Price: " + std::to_string(i->getPrice()) + " Attractiveness: " + std::to_string(int(i->getAttractiveness()*100));
//             TakenProductsList->addItem(QString(data.c_str()));
//         }
//     }

//     void Window::printHumanName() {
//         if (TheHuman == nullptr) {
//             HumanNameLabel->setText(QCoreApplication::translate("MainWindow", "HumanName", nullptr));
//             std::cout << TheHuman << std::endl;
//         } else {
//             HumanNameLabel->setText(QCoreApplication::translate("MainWindow", TheHuman->getName().c_str(), nullptr));
//             std::cout << TheHuman->getName() << std::endl;
//         }
//         HumanNameLabel->update();
//     };

//     void Window::clearAll() {
//         ShopMap->clear();
//         ToBuyList->clear();
//         TakenProductsList->clear();
//         TheHuman = nullptr;
//     }

// } // namespace Ui