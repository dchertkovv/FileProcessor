#include "mainwindow.h"
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    this->setAcceptDrops(true);
    QGridLayout* layout = new QGridLayout(this);
    mask = new QLineEdit(this);
    QLabel* labMask = new QLabel("Маска имени", this);
    find = new QLineEdit(this);
    replace = new QLineEdit(this);
    QLabel* labFind = new QLabel("Найти", this);
    QLabel* labReplace = new QLabel("Заменить", this);
    fandr = new QPushButton("Найти и заменить", this);
    rollback = new QPushButton("Откат", this);
    layout->addWidget(mask, 0, 0);
    layout->addWidget(labMask, 1, 0, Qt::AlignTop);
    layout->addWidget(find, 0, 1);
    layout->addWidget(replace, 0, 2);

    buttonMaskInit();

    mainWidget = new MainWidget(this);
    layout->addWidget(mainWidget, 3, 1, 3, 3);
    layout->addWidget(clear, 6, 1);

    connect(clear, SIGNAL(clicked()), mainWidget, SLOT(clearContents()));

    QVBoxLayout* fandrLayout = new QVBoxLayout(this);
    fandrLayout->addWidget(labFind);
    fandrLayout->addWidget(fandr);

    QVBoxLayout* rollbackLayout = new QVBoxLayout(this);
    rollbackLayout->addWidget(labReplace);
    rollbackLayout->addWidget(rollback);

    QVBoxLayout* maskLayout = new QVBoxLayout(this);
    QFrame* maskFrame = new QFrame;
    maskFrame->setLayout(maskLayout);
    maskFrame->setObjectName("maskButtonFrame");
    maskFrame->setFrameStyle(QFrame::Raised);
    maskFrame->setFrameShape(QFrame::StyledPanel);
    maskLayout->addWidget(mN);
    maskLayout->addWidget(mNn);
    maskLayout->addWidget(mNxy);
    maskLayout->addWidget(mNnn);
    maskLayout->addWidget(mNpn);
    maskLayout->addWidget(mNxan);
    maskLayout->addWidget(mNnpy);
    maskLayout->addWidget(mCn);
    maskLayout->addWidget(mCna);
    maskLayout->addWidget(mYMD);
    maskLayout->addWidget(mHMS);
    maskLayout->addWidget(mE);
    maskLayout->addWidget(mExy);

    this->setStyleSheet(
            "QPushButton {border-style: "
            "outset; border-width: 3px; border-color: #c0c0c0;} "
            "QPushButton:hover {border-style: ridge; border-width: "
            "3px;} "
            "QPushButton:pressed {border-style: inset; border-width: "
            "3px;} "
            "QFrame#maskButtonFrame {border: 2px; border-radius: 4px; "
            "border-style: inset; border-width: 3px; "
            "border-color: #c0c0c0;}");

    insertDialog = new QDialog(this);
    browse = new QPushButton("Обзор...", this);
    connect(browse, SIGNAL(clicked()), this, SLOT(clickBrowse()));

    layout->addWidget(browse, 6, 2);
    layout->addWidget(maskFrame, 3, 0);

    layout->addLayout(fandrLayout, 1, 1, Qt::AlignTop);
    layout->addLayout(rollbackLayout, 1, 2, Qt::AlignTop);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    this->resize(800, 600);
}

void MainWindow::buttonMaskInit()
{
    mN = new QPushButton("[N]", this);
    mN->setObjectName("maskButton");
    mNn = new QPushButton("[Nn]", this);
    mNn->setObjectName("maskButton");
    mNxy = new QPushButton("[Nx-y]", this);
    mNxy->setObjectName("maskButton");
    mNnn = new QPushButton("[Nn-]", this);
    mNnn->setObjectName("maskButton");
    mNpn = new QPushButton("[N-n]", this);
    mNpn->setObjectName("maskButton");
    mNxan = new QPushButton("[Nx,n]", this);
    mNxan->setObjectName("maskButton");
    mNnpy = new QPushButton("[Nn--y]", this);
    mNnpy->setObjectName("maskButton");
    mCn = new QPushButton("[Cn]", this);
    mCn->setObjectName("maskButton");
    mCna = new QPushButton("[Cn,a]", this);
    mCna->setObjectName("maskButton");
    mYMD = new QPushButton("[YMD]", this);
    mYMD->setObjectName("maskButton");
    mHMS = new QPushButton("[hms]", this);
    mHMS->setObjectName("maskButton");
    mE = new QPushButton("[E]", this);
    mE->setObjectName("maskButton");
    mExy = new QPushButton("[Ex-y]", this);
    mExy->setObjectName("maskButton");
    clear = new QPushButton("Очистить", this);
}

void MainWindow::clickBrowse()
{
    QGridLayout* layout = new QGridLayout(insertDialog);
    pushInsert = new PushInsert(insertDialog);
    QPushButton* ok = new QPushButton("Ок", insertDialog);
    QPushButton* cancel = new QPushButton("Отмена", insertDialog);

    connect(pushInsert,
            SIGNAL(selectIndex(QFileInfo*)),
            this,
            SLOT(selectBrowse(QFileInfo*)));
    connect(ok, SIGNAL(clicked()), this, SLOT(clickOk()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(clickCancel()));

    layout->addWidget(pushInsert, 0, 0, 2, 2);
    layout->addWidget(ok, 1, 0);
    layout->addWidget(cancel, 1, 1);
    insertDialog->setLayout(layout);
    insertDialog->resize(400, 400);
    insertDialog->exec();
}

void MainWindow::selectBrowse(QFileInfo* info)
{
    mainWidget->addElement(info);
    insertDialog->close();
}

void MainWindow::clickOk()
{
    for (auto info : pushInsert->selectedInfo()) {
        mainWidget->addElement(&info);
    }
    insertDialog->close();
}

void MainWindow::clickCancel()
{
    insertDialog->close();
}

MainWindow::~MainWindow()
{
}
