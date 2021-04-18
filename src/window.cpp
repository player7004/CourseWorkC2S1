//
// Created by player7004 on 04.12.2020.
//

#include "window.hpp"

Window::Window() {
    WLog.write("Window Initializing");
    try {
        setupUI();
        setupBase();
        connectUI();
    } catch (...) {
        WLog.write("Can`t initialize window", WriteTypes::Error);
        this->close();
    }
}

void Window::closeEvent(QCloseEvent* event) {
    running = false;
    event->accept();
}

void Window::connectUI() {
    WLog.write("Connecting Buttons");
    // Закрывает окно
    auto QuitFunc = [this]{
        WLog.write("Closing window");
        stopThread();
        this->close();
    };
    connect(QuitButton, &QPushButton::clicked, QuitFunc);

    // Меняет режим работы на ручной
    auto SetManualModeFunc = [this] {
        this->Mode = ModeStatuses::Manual;
        stopThread();
        WLog.write("Mode set to Manual");
    };
    // Меняет режим работы на автоматический
    auto SetAutoModeFunc = [this] {
        this->Mode = ModeStatuses::Auto;
        startThread();
        WLog.write("Mode set to Auto");
    };
    connect(ManualModeButton, &QRadioButton::clicked, SetManualModeFunc);
    connect(AutoModeButton, &QRadioButton::clicked, SetAutoModeFunc);

    // Меняет объект отображения
    auto SetToDrawItemFunc = [this](int index) {
        this->Item = static_cast<ToDrawItem>(index);
        drawInfoList();
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
        this->stopThread();
        QString str = QFileDialog::getOpenFileName(this, "Choose Shop file", "", "*.json");
        this->clearWindow();
        WMap.clear();
        WMap.open(str);
        WMap.create();
        WLog.write("File changed to " + str);
    };
    connect(LoadShopButton, &QPushButton::clicked, LoadNewFileFunc);

    // Делает следующий шаг
    auto NextStepFunc = [this] {
        if (this->TStatus == ThreadStatuses::Running) {
            stopThread();
            this->Mode = ModeStatuses::Manual;
            this->ManualModeButton->setChecked(true);
            this->AutoModeButton->setChecked(false);
            WLog.write("Mode status set to auto by force");
        }
        WMap.rebuild();
        this->drawGraphics();
        this->drawInfoList();
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
    MenuBox->addItem("Информация о человеке");
    MenuBox->addItem("Список всех людей");
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
    GraphicView = new QListWidget(this);
    GraphicView->setGeometry(5, 5, 325, 325);
    QFont gfont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    gfont.setPointSize(20);
    GraphicView->setFont(gfont);
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
    font.setPointSize(12);
    DelayLabel->setFont(font);
    
    this->show();

    WLog.write("UI set up");
}

void Window::setupBase() {
    WLog.open("Window.log");
    Mode = ModeStatuses::Manual;
    Item = ToDrawItem::MapLegend;
    Delay = 2;
    running = true;
    Worker = new std::thread(ThreadFunc, this);
    Worker->detach();
    TStatus = ThreadStatuses::Sleeping;
    WMap.open("Shop2.json");
    WMap.create();
    WLog.write("Base set up");
}

void Window::clearWindow() {
    stopThread();
    resetGraphic();
    InfoList->clear();
    WLog.write("Window cleared");
}

void Window::resetGraphic() {
    GraphicView->clear();
    WLog.write("Graphics window cleared");
}

void Window::drawGraphics() {
    resetGraphic();
    for (const auto& i : WMap.OutMap) {
        GraphicView->addItem(i);
    }
    WLog.write("Graphics drown");
}

void Window::drawInfoList() {
    InfoList->clear();
    if (Item == ToDrawItem::MapLegend) {
        for (const auto& i : WMap.generateMapLegend()) {
            InfoList->addItem(i);
        }
    }
    else if (Item == ToDrawItem::StandContent) {
        for (const auto& i : WMap.generateStandContent()) {
            InfoList->addItem(i);
        }
    }
    else if (Item == ToDrawItem::TakenProducts) {
        for (const auto& i : WMap.generateTakenProducts()) {
            InfoList->addItem(i);
        }
    }
    else if (Item == ToDrawItem::ToBuyList) {
        for (const auto& i : WMap.generateToBuyList()) {
            InfoList->addItem(i);
        }
    } else if(Item == ToDrawItem::HumanInfo) {
        for (const auto& i: WMap.generateCurrentHumanInfo()) {
            InfoList->addItem(i);
        }
    } else if(Item == ToDrawItem::AllHumans) {
        for (const auto& i: WMap.generateAllHumans()) {
            InfoList->addItem(i);
        }
    }
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
        return "";
    }
}

void Window::ThreadFunc(Window* object) {
    object->WLog.write("Thread started");
    object->currentDelay = 0;
    while (object->running) {
        if (object->TStatus == ThreadStatuses::Running) {
            if (object->currentDelay > object->Delay) {
                object->WMap.rebuild();
                object->drawGraphics();
                object->drawInfoList();
                object->currentDelay = 0;
            } else {
                object->currentDelay += 2;
            }
        }
        if (object->TStatus == ThreadStatuses::Stopping) {
            object->TStatus = ThreadStatuses::Sleeping;
            object->currentDelay = 0;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    object->TStatus = ThreadStatuses::Sleeping; 
}

void Window::stopThread() {
    WLog.write("Stopping thread");
    this->TStatus = ThreadStatuses::Stopping;
    // В теории в данном случае необязательно ждать завершения поток
    while (this->TStatus != ThreadStatuses::Sleeping);
    WLog.write("Thread stopped");
}

void Window::startThread() {
    TStatus = ThreadStatuses::Running;
    //while (TStatus != ThreadStatuses::Sleeping);
    //Worker.detach();
    WLog.write("Thread started");
}