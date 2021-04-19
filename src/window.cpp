//
// Created by player7004 on 04.12.2020.
//

#include "window.hpp"

Window::Window() {
    try {
        setupUI();
        setupBase();
        connectUI();
        drawInfoList();
    } catch (...) {
        this->close();
    }
}

void Window::closeEvent(QCloseEvent* event) {
    running = false;
    event->accept();
}

void Window::connectUI() {
    // Закрывает окно
    auto QuitFunc = [this]{
        stopThread();
        if (WMap.generateAllHumans()[0] != "Empty" and WMap.generateAllHumans()[0] != "None") {
            switch(openSaveInFileWindow()) {
                case QMessageBox::Save:
                    this->WMap.save(QFileDialog::getOpenFileName(this, "Выберите файл для сохранения", "", "*.json"));
                    break;
                case QMessageBox::Discard:
                    break;
            }
        }
        this->close();
    };
    connect(QuitButton, &QPushButton::clicked, QuitFunc);

    // Меняет режим работы на ручной
    auto SetManualModeFunc = [this] {
        this->Mode = ModeStatuses::Manual;
        stopThread();
    };
    // Меняет режим работы на автоматический
    auto SetAutoModeFunc = [this] {
        this->Mode = ModeStatuses::Auto;
        startThread();
    };
    connect(ManualModeButton, &QRadioButton::clicked, SetManualModeFunc);
    connect(AutoModeButton, &QRadioButton::clicked, SetAutoModeFunc);

    // Меняет объект отображения
    auto SetToDrawItemFunc = [this](int index) {
        this->Item = static_cast<ToDrawItem>(index);
        drawInfoList();
    };
    connect(MenuBox, QOverload<int>::of(&QComboBox::currentIndexChanged), SetToDrawItemFunc);

    // Меняет карту
    auto LoadNewFileFunc = [this] {
        this->stopThread();
        if (WMap.generateAllHumans()[0] != "Empty" and WMap.generateAllHumans()[0] != "None") {
            switch(openSaveInFileWindow()) {
                case QMessageBox::Save:
                    this->WMap.save(QFileDialog::getOpenFileName(this, "Выберите файл для сохранения", "", "*.json"));
                    break;
                case QMessageBox::Discard:
                    break;
            }
        }
        QString str = QFileDialog::getOpenFileName(this, "Choose Shop file", "", "*.json");
        this->clearWindow();
        WMap.clear();
        if (!WMap.open(str) or !WMap.create()) {
            switch (openFileErrorWindow()) {
                case QMessageBox::Ok:
                    return;
            }
        }
        drawInfoList();
    };
    connect(LoadShopButton, &QPushButton::clicked, LoadNewFileFunc);

    // Делает следующий шаг
    auto NextStepFunc = [this] {
        if (this->TStatus == ThreadStatuses::Running) {
            stopThread();
            this->Mode = ModeStatuses::Manual;
            this->ManualModeButton->setChecked(true);
            this->AutoModeButton->setChecked(false);
        }
        WMap.rebuild();
        this->drawGraphics();
        this->drawInfoList();
    };
    connect(NextStepButton, &QPushButton::clicked, NextStepFunc);
}

void Window::setupUI() {
    // Основное окно
    this->resize(500, 400);
    this->setMinimumSize(500, 400);
    this->setMaximumSize(500, 400);
    // Меню
    MenuBox = new QComboBox(this);
    MenuBox->setGeometry(335, 40, 160, 30);
    MenuBox->addItem("Легенда карты");
    MenuBox->addItem("Информация о человеке");
    MenuBox->addItem("Список покупок");
    MenuBox->addItem("Взятые продукты");
    MenuBox->addItem("Содержание стенда");
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
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(20);
    GraphicView->setFont(font);
    
    this->show();
}

void Window::setupBase() {
    Mode = ModeStatuses::Manual;
    Item = ToDrawItem::MapLegend;
    running = true;
    Worker = new std::thread(ThreadFunc, this);
    Worker->detach();
    TStatus = ThreadStatuses::Sleeping;
    WMap.open("examples/EmptyShop.json");
    WMap.create();
}

void Window::clearWindow() {
    stopThread();
    resetGraphic();
    InfoList->clear();
}

void Window::drawGraphics() {
    resetGraphic();
    for (const auto& i : WMap.OutMap) {
        GraphicView->addItem(i);
    }
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

void Window::ThreadFunc(Window* object) {
    while (object->running) {
        if (object->TStatus == ThreadStatuses::Running) {
            object->WMap.rebuild();
            object->drawGraphics();
            object->drawInfoList();
        }
        if (object->TStatus == ThreadStatuses::Stopping) {
            object->TStatus = ThreadStatuses::Sleeping;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    object->TStatus = ThreadStatuses::Sleeping; 
}

void Window::stopThread() {
    this->TStatus = ThreadStatuses::Stopping;
    // В теории в данном случае необязательно ждать завершения поток
    while (this->TStatus != ThreadStatuses::Sleeping);
}

void Window::startThread() {
    TStatus = ThreadStatuses::Running;
}