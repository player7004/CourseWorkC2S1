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
        drawGraphics();
    } catch (...) {
        this->close();
    }
}

void Window::connectUI() {
    // Закрывает окно
    auto QuitFunc = [this]{
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

    // Меняет объект отображения
    auto SetToDrawItemFunc = [this](int index) {
        this->Item = static_cast<ToDrawItem>(index);
        drawInfoList();
    };
    connect(MenuBox, QOverload<int>::of(&QComboBox::currentIndexChanged), SetToDrawItemFunc);

    // Меняет карту
    auto LoadNewFileFunc = [this] {
        if (WMap.generateAllHumans()[0] != "Empty" and WMap.generateAllHumans()[0] != "None") {
            switch(openSaveInFileWindow()) {
                case QMessageBox::Save: {
                    QString str = QFileDialog::getOpenFileName(this, "Выберите файл для сохранения", "", "*.json");
                    if (str.isEmpty()) {
                        break;
                    }
                    this->WMap.save(str);
                    break;
                    }
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
        createModules();
        drawInfoList();
        drawGraphics();
    };
    connect(LoadShopButton, &QPushButton::clicked, LoadNewFileFunc);

    // Делает следующий шаг
    auto NextStepFunc = [this] {
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
    MenuBox->addItem("Купленные продукты");
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
    // Окно просматра графики
    GraphicView = new QListWidget(this);
    GraphicView->setGeometry(5, 5, 325, 390);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(20);
    GraphicView->setFont(font);
    
    this->show();
}

void Window::setupBase() {
    Item = ToDrawItem::MapLegend;
    WMap.open("examples/EmptyShop.json");
    WMap.create();
    createModules();
}

void Window::clearWindow() {
    InfoList->clear();
}

void Window::drawGraphics() {
    for (size_t i = 0; i < WMap.OutMap.size(); i++) {
        Modules[i].second->setText(WMap.OutMap.at(i));
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
    } else if (Item == ToDrawItem::BoughtProducts) {
        for (const auto& i: WMap.generateBoughtProducts()) {
            InfoList->addItem(i);
        }
    }
}

int Window::openSaveInFileWindow() {
    QMessageBox box;
    box.setWindowTitle("Важный вопрос!");
    box.setText("Сохранить результат выполнения в файл?");
    box.setStandardButtons(QMessageBox::Discard | QMessageBox::Save);
    box.setDefaultButton(QMessageBox::Save);
    return box.exec();
}

int Window::openFileErrorWindow() {
    QMessageBox box;
    box.setWindowTitle("Ошибка!");
    box.setText("Не удаётся корректно отрыть файл или создать карту");
    box.setInformativeText("Проверьте файл или выберите другой");
    box.setStandardButtons(QMessageBox::Ok);
    box.setDefaultButton(QMessageBox::Cancel);
    return box.exec();
}

void Window::createModules() {
    auto size = WMap.OutMap.size();
    size_t i = 0;
    if (size < Modules.size()) {
        for (auto j = size; j < Modules.size(); j++) {
            Modules[j].second->setText("");
        }
        return;
    } else if (size > Modules.size()) {
        i = Modules.size();
    } else {
        i = size;
    }
    for (; i < size; i++) {
        Modules.emplace_back();
        Modules.back().first = std::unique_ptr<QListWidgetItem>(new QListWidgetItem()), Modules.back().second = std::unique_ptr<QLabel>(new QLabel());
        GraphicView->addItem(Modules.back().first.get());
        GraphicView->setItemWidget(Modules.back().first.get(), Modules.back().second.get());
    }
}

