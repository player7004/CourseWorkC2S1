//
// Created by player7004 on 04.12.2020.
//

#include "window.hpp"

void Ui_MainWindow::setupUi(QMainWindow *MainWindow)
{
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(800, 600);
    MainWindow->setMinimumSize(QSize(800, 600));
    MainWindow->setMaximumSize(QSize(800, 600));
    MAIN = new QWidget(MainWindow);
    MAIN->setObjectName(QString::fromUtf8("MAIN"));
    ShopMap = new QListWidget(MAIN);
    ShopMap->setObjectName(QString::fromUtf8("ShopMap"));
    ShopMap->setGeometry(QRect(10, 10, 321, 321));
    QFont font;
    font.setFamily(QString::fromUtf8("DejaVu Sans Mono"));
    font.setBold(true);
    font.setWeight(75);
    ShopMap->setFont(font);
    StandContenLabel = new QLabel(MAIN);
    StandContenLabel->setObjectName(QString::fromUtf8("StandContenLabel"));
    StandContenLabel->setGeometry(QRect(10, 340, 321, 20));
    QFont font1;
    font1.setPointSize(14);
    StandContenLabel->setFont(font1);
    StandContentList = new QListWidget(MAIN);
    StandContentList->setObjectName(QString::fromUtf8("StandContentList"));
    StandContentList->setGeometry(QRect(10, 370, 321, 221));
    StandContentList->setFont(font);
    HumanNameLabel = new QLabel(MAIN);
    HumanNameLabel->setObjectName(QString::fromUtf8("HumanNameLabel"));
    HumanNameLabel->setGeometry(QRect(340, 10, 121, 21));
    QFont font2;
    font2.setPointSize(12);
    HumanNameLabel->setFont(font2);
    ToBuyList = new QListWidget(MAIN);
    ToBuyList->setObjectName(QString::fromUtf8("ToBuyList"));
    ToBuyList->setGeometry(QRect(340, 70, 221, 261));
    ToBuyList->setFont(font);
    TakenProductsList = new QListWidget(MAIN);
    TakenProductsList->setObjectName(QString::fromUtf8("TakenProductsList"));
    TakenProductsList->setGeometry(QRect(570, 70, 221, 261));
    TakenProductsList->setFont(font);
    CashBoxLabel = new QLabel(MAIN);
    CashBoxLabel->setObjectName(QString::fromUtf8("CashBoxLabel"));
    CashBoxLabel->setGeometry(QRect(470, 10, 151, 21));
    CashBoxLabel->setFont(font2);
    MoneyLabel = new QLabel(MAIN);
    MoneyLabel->setObjectName(QString::fromUtf8("MoneyLabel"));
    MoneyLabel->setGeometry(QRect(630, 10, 161, 21));
    MoneyLabel->setFont(font1);
    ToBuyLabel = new QLabel(MAIN);
    ToBuyLabel->setObjectName(QString::fromUtf8("ToBuyLabel"));
    ToBuyLabel->setGeometry(QRect(340, 40, 221, 21));
    ToBuyLabel->setFont(font1);
    TakenProductsLabbel = new QLabel(MAIN);
    TakenProductsLabbel->setObjectName(QString::fromUtf8("TakenProductsLabbel"));
    TakenProductsLabbel->setGeometry(QRect(570, 40, 221, 21));
    TakenProductsLabbel->setFont(font1);
    MapLegendLabel = new QLabel(MAIN);
    MapLegendLabel->setObjectName(QString::fromUtf8("MapLegendLabel"));
    MapLegendLabel->setGeometry(QRect(340, 340, 221, 20));
    MapLegendLabel->setFont(font1);
    MapLegendList = new QListWidget(MAIN);
    MapLegendList->setObjectName(QString::fromUtf8("MapLegendList"));
    MapLegendList->setGeometry(QRect(340, 370, 221, 221));
    MapLegendList->setFont(font);
    NicknameLabel = new QLabel(MAIN);
    NicknameLabel->setObjectName(QString::fromUtf8("NicknameLabel"));
    NicknameLabel->setGeometry(QRect(718, 530, 81, 20));
    GitHubLabel = new QLabel(MAIN);
    GitHubLabel->setObjectName(QString::fromUtf8("GitHubLabel"));
    GitHubLabel->setGeometry(QRect(580, 550, 221, 20));
    VKLabel = new QLabel(MAIN);
    VKLabel->setObjectName(QString::fromUtf8("VKLabel"));
    VKLabel->setGeometry(QRect(610, 570, 191, 20));
    OthersLabel = new QLabel(MAIN);
    OthersLabel->setObjectName(QString::fromUtf8("OthersLabel"));
    OthersLabel->setGeometry(QRect(570, 340, 221, 20));
    OthersLabel->setFont(font1);
    PauseButton = new QPushButton(MAIN);
    PauseButton->setObjectName(QString::fromUtf8("PauseButton"));
    PauseButton->setGeometry(QRect(570, 370, 111, 28));
    ContinueButton = new QPushButton(MAIN);
    ContinueButton->setObjectName(QString::fromUtf8("ContinueButton"));
    ContinueButton->setGeometry(QRect(682, 370, 111, 28));
    LoadShopButton = new QPushButton(MAIN);
    LoadShopButton->setObjectName(QString::fromUtf8("LoadShopButton"));
    LoadShopButton->setGeometry(QRect(570, 400, 223, 28));
    QuitButton = new QPushButton(MAIN);
    QuitButton->setObjectName(QString::fromUtf8("QuitButton"));
    QuitButton->setGeometry(570, 430, 223, 28);
    MainWindow->setCentralWidget(MAIN);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
}

void Ui_MainWindow::retranslateUi(QMainWindow *MainWindow)
{
    MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
    StandContenLabel->setText(QCoreApplication::translate("MainWindow", "Stand content:", nullptr));
    HumanNameLabel->setText(QCoreApplication::translate("MainWindow", "HumanName", nullptr));
    CashBoxLabel->setText(QCoreApplication::translate("MainWindow", "CashBox Money: ", nullptr));
    MoneyLabel->setText(QCoreApplication::translate("MainWindow", "Money", nullptr));
    ToBuyLabel->setText(QCoreApplication::translate("MainWindow", "To Buy:", nullptr));
    TakenProductsLabbel->setText(QCoreApplication::translate("MainWindow", "Taken Products: ", nullptr));
    MapLegendLabel->setText(QCoreApplication::translate("MainWindow", "ShopMap Legend:", nullptr));
    NicknameLabel->setText(QCoreApplication::translate("MainWindow", "player7004", nullptr));
    GitHubLabel->setText(QCoreApplication::translate("MainWindow", "https://github.com/player7004", nullptr));
    VKLabel->setText(QCoreApplication::translate("MainWindow", "https://vk.com/player7004", nullptr));
    OthersLabel->setText(QCoreApplication::translate("MainWindow", "Others:", nullptr));
    PauseButton->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
    ContinueButton->setText(QCoreApplication::translate("MainWindow", "Continue", nullptr));
    LoadShopButton->setText(QCoreApplication::translate("MainWindow", "Load Shop", nullptr));
    QuitButton->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
}

namespace Ui
{
    Window::Window()
    {
        setupUi(this);
        this->show();
        // connect(LoadShopButton, &QPushButton::clicked, [this]() {
        //     this->TheHuman = nullptr;
        //     this->ShopMap->clear();
        //     QString str = QFileDialog::getOpenFileName(this, "Choose Shop file", "", "*.json");
        //     this->map = Map(str.toStdString());
        //     this->ShopStatus = map.getStatus();
        //     this->printShop();
        //     std::thread some([this] {
        //         this->running = false;
        //         std::this_thread::sleep_for(std::chrono::seconds(2));
        //         this->running = true;
        //         while (*this->ShopStatus != Statuses::NoMoreHumans)
        //         {
        //             while (this->running)
        //             {
        //                 TheHuman = map.getTheHuman();
        //                 this->HumanNameLabel->setText(QString(TheHuman->getName().c_str()));
        //                 printToBuyList();
        //                 printTakenProducts();
        //                 this->map.rebuildMap();
        //                 this->printShop();
        //                 this->ShopStatus = this->map.getStatus();
        //                 std::this_thread::sleep_for(std::chrono::seconds(2));
        //             }
        //         }
        //     });
        //     some.detach();
        // });
        connect(PauseButton, &QPushButton::clicked, [this]() {
            this->running = false;
        });
        connect(ContinueButton, &QPushButton::clicked, [this]() {
            this->running = true;
        });
        connect(QuitButton, &QPushButton::clicked, [this]() {
            this->running = false;
            this->close();
        });
    }
    // void MainWindow::printShop()
    // {
    //     ShopMap->clear();
    //     if (*ShopStatus != Statuses::Empty and *ShopStatus != Statuses::Loaded)
    //     {
    //         C A Mp = map.getMap();
    //         for (const auto &str : Mp)
    //         {
    //             ShopMap->addItem(QString::fromUtf8(str.c_str()));
    //         }
    //     }
    //     else
    //     {
    //         if (*ShopStatus == Statuses::Empty)
    //         {
    //             loadErrorWindow("Can`t load file.", "Check it or chose another.");
    //         }
    //         else if (*ShopStatus == Statuses::Loaded)
    //         {
    //             loadErrorWindow("Can`t create Shop from this file.", "Check it or chose another.");
    //         }
    //         std::cerr << "Shop isn`t loaded" << EL;
    //     }
    // }

    void Window::loadErrorWindow(const std::string &text, const std::string &hint)
    {
        // Инициализация самого окна
        auto *obj = new QWidget(nullptr);
        obj->setAttribute(Qt::WA_DeleteOnClose);
        obj->setAttribute(Qt::WA_ShowModal);
        obj->resize(400, 150);
        obj->setMinimumSize(400, 150);
        obj->setMaximumSize(400, 150);
        // Инициализация кнопки
        auto *butt = new QPushButton(obj);
        butt->setGeometry(10, 115, 380, 30);
        butt->setText(QString("Quit"));
        connect(butt, &QPushButton::clicked, [obj]() {
            obj->close();
        });
        // Инициализация шрифта
        QFont font;
        font.setPointSize(14);
        // Иницаиализация надписей
        // Надпись Что=то случилось
        auto *constLabel = new QLabel(obj);
        constLabel->setGeometry(10, 5, 380, 25);
        constLabel->setFont(font);
        constLabel->setText("<b>Something unexpected happened: </b>");
        // Текст ошибки
        auto *errorTextLabel = new QLabel(obj);
        QPalette errorColors;
        errorColors.setColor(QPalette::Window, Qt::white);
        errorColors.setColor(QPalette::WindowText, Qt::red);
        errorTextLabel->setGeometry(30, 35, 350, 25);
        errorTextLabel->setFont(font);
        errorTextLabel->setPalette(errorColors);
        errorTextLabel->setText(QString(text.c_str()));
        // Совет по исправлению
        auto *hintLabel = new QLabel(obj);
        QPalette hintColors;
        hintColors.setColor(QPalette::Window, Qt::white);
        hintColors.setColor(QPalette::WindowText, Qt::green);
        hintLabel->setPalette(hintColors);
        hintLabel->setGeometry(30, 65, 350, 25);
        hintLabel->setFont(font);
        hintLabel->setText(QString(hint.c_str()));
        // Показ окна
        obj->show();
    }

    // V MainWindow::printToBuyList()
    // {
    //     A list = TheHuman->getToBuyList();
    //     ToBuyList->clear();
    //     for (C A &obj : *list)
    //     {
    //         ToBuyList->addItem(QString(getProductAsSTR(obj).c_str()));
    //     }
    // }

    // V MainWindow::printTakenProducts()
    // {
    //     A list = TheHuman->getTakenProducts();
    //     TakenProductsList->clear();
    //     for (C A &obj : *list)
    //     {
    //         TakenProductsList->addItem(QString(getProductAsSTR(obj).c_str()));
    //     }
    // }

    // STR MainWindow::getProductAsSTR(C Product &val)
    // {
    //     STR result;
    //     if (val.getPType() == Products::Milk)
    //     {
    //         result = "Milk";
    //     }
    //     else if (val.getPType() == Products::Bread)
    //     {
    //         result = "Bread";
    //     }
    //     else if (val.getPType() == Products::Beer)
    //     {
    //         result = "Beer";
    //     }
    //     else if (val.getPType() == Products::Cheese)
    //     {
    //         result = "Cheese";
    //     }
    //     else if (val.getPType() == Products::Eggs)
    //     {
    //         result = "Eggs";
    //     }
    //     else if (val.getPType() == Products::BuckWheat)
    //     {
    //         result = "Buckwheat";
    //     }
    //     else if (val.getPType() == Products::ToiletPaper)
    //     {
    //         result = "Toilet Paper";
    //     }
    //     else if (val.getPType() == Products::Meet)
    //     {
    //         result = "Meet";
    //     }
    //     else if (val.getPType() == Products::Fish)
    //     {
    //         result = "Fish";
    //     }
    //     else if (val.getPType() == Products::Butter)
    //     {
    //         result = "Butter";
    //     }
    //     else if (val.getPType() == Products::Tomato)
    //     {
    //         result = "Tomato";
    //     }
    //     else if (val.getPType() == Products::Apples)
    //     {
    //         result = "Apples";
    //     }
    //     else if (val.getPType() == Products::Lemon)
    //     {
    //         result = "Lemon";
    //     }
    //     else
    //     {
    //         result = "ToiletPaper";
    //     }
    //     result += " Price: " + std::to_string(val.getPrice()) + " Attractiveness: " + std::to_string(int(val.getAttractiveness() * 100));
    //     return result;
    // }
} // namespace Ui