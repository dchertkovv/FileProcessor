#include "mainwindow.h"
#include <QErrorMessage>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPalette>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    exception = false;
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
    fandr->setObjectName("mainButton");
    rollback->setObjectName("mainButton");
    connect(fandr, SIGNAL(clicked()), this, SLOT(readText()));
    connect(rollback, SIGNAL(clicked()), this, SLOT(clickRollback()));

    layout->addWidget(mask, 0, 0);
    layout->addWidget(labMask, 1, 0, Qt::AlignTop);
    layout->addWidget(find, 0, 1);
    layout->addWidget(replace, 0, 2);

    buttonMaskInit();

    mainWidget = new MainWidget(this);

    mainWidget->setStyleSheet(widgetStyleSheet());
    layout->addWidget(mainWidget, 3, 1, 3, 3);
    layout->addWidget(clear, 6, 1);

    connect(clear, SIGNAL(clicked()), mainWidget, SLOT(clearContents()));

    QVBoxLayout* fandrLayout = new QVBoxLayout(this);
    fandrLayout->addWidget(labReplace);
    fandrLayout->addWidget(fandr);

    QVBoxLayout* rollbackLayout = new QVBoxLayout(this);
    rollbackLayout->addWidget(labFind);
    rollbackLayout->addWidget(rollback);

    QVBoxLayout* maskLayout = new QVBoxLayout(this);
    QFrame* maskFrame = new QFrame;

    buttonLayoutInit(maskLayout, maskFrame);

    browse = new QPushButton("Обзор...", this);
    browse->setObjectName("mainButton");
    connect(browse, SIGNAL(clicked()), this, SLOT(clickBrowse()));

    layout->addWidget(browse, 6, 2);
    layout->addWidget(maskFrame, 3, 0);

    layout->addLayout(fandrLayout, 1, 2, Qt::AlignTop);
    layout->addLayout(rollbackLayout, 1, 1, Qt::AlignTop);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    initStyleSheet();
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
    clear->setObjectName("mainButton");
}

void MainWindow::buttonLayoutInit(QVBoxLayout* maskLayout, QFrame* maskFrame)
{
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
}

void MainWindow::readText()
{
    QString strMask = mask->text();
    QString strFind = find->text();
    QString strReplace = replace->text();

    try {
        if (strMask.contains(QRegExp("\\S*[\\/\\\\:\\*\\?\\|\\<\\>]\\S*"))) {
            throw 1;
        }
        if (strFind.contains(
                    QRegExp("\\S*[\\/\\\\\\:\\*\\?\\|\\<\\>\\[\\]]\\S*"))) {
            throw 2;
        }
        if (strReplace.contains(
                    QRegExp("\\S*[\\/\\\\\\:\\*\\?\\|\\<\\>\\[\\]]\\S*"))) {
            throw 3;
        }
        if (strReplace.isEmpty()) {
            throw 4;
        }
        if (strMask.isEmpty()) {
            throw 5;
        }

        Mask mask(strReplace, strMask);
        mask.readName();
        exception = false;
        replacing(mask, strFind);
    } catch (int a) {
        exception = true;
        QMessageBox* dialog = new QMessageBox(this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        QString error;
        switch (a) {
        case 1: {
            error = QString("Строка маски имеет запрещенные символы");
            break;
        }
        case 2: {
            error = QString("Строка поиска имеет запрещенные символы");
            break;
        }
        case 3: {
            error = QString("Строка замены имеет запрещенные символы");
            break;
        }
        case 4: {
            error = QString("Строка замены не должна быть пуста");
            break;
        }
        case 5: {
            error = QString("Строка маски не должна быть пуста");
            break;
        }
        case -1: {
            error = QString("Открывающих скобок больше чем закрывающих");
            break;
        }
        case -2: {
            error = QString("Закрывающих скобок больше чем открывающих");
        }
        }
        dialog->setText(error);
        dialog->exec();
    } catch (ExceptionMask exp) {
        exception = true;
        QMessageBox* dialog = new QMessageBox(this);
        dialog->setStyleSheet("align=center");
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        QString errorString("Ошибка в ");
        errorString += exp.mask;
        if (exp.type == ExceptionMask::TypeError::Number) {
            errorString += ": некорректное число";
        } else {
            errorString += ": некорректная маска";
        }
        if (!exp.expected.isEmpty()) {
            errorString += ". Ожидается " + exp.expected;
        }
        dialog->setText(errorString);

        dialog->exec();
    } catch (ExceptionFile exp) {
        exception = true;
        QMessageBox* dialog = new QMessageBox(this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setText(exp.error);
        dialog->exec();
    } catch (ExceptionReplacing exp) {
        exception = true;
        QMessageBox* dialog = new QMessageBox(this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setText(exp.error);
        dialog->setInformativeText("Все изменения отменены");
        dialog->exec();
    }

    if (exception)
        return;
}
void MainWindow::clickBrowse()
{
    insertDialog = new QDialog(this);
    insertDialog->setAttribute(Qt::WA_DeleteOnClose);
    auto style = insertStyleSheet();
    insertDialog->setStyleSheet(style.first);
    pushInsert = new PushInsert(insertDialog);
    QVBoxLayout* layout = new QVBoxLayout(insertDialog);
    QHBoxLayout* buttonLayout = new QHBoxLayout(insertDialog);
    QPushButton* ok = new QPushButton("Ок", insertDialog);
    QPushButton* cancel = new QPushButton("Отмена", insertDialog);

    pushInsert->setStyleSheet(style.second);

    connect(pushInsert,
            SIGNAL(selectIndex(QFileInfo*)),
            this,
            SLOT(selectBrowse(QFileInfo*)));
    connect(ok, SIGNAL(clicked()), this, SLOT(clickOk()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(clickCancel()));

    layout->addWidget(pushInsert);
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    layout->addLayout(buttonLayout);
    insertDialog->setLayout(layout);
    insertDialog->resize(400, 500);
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

void MainWindow::clickRollback()
{
    QFile oldNames("~temp.log");
    if (!oldNames.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox* error = new QMessageBox(this);
        error->setAttribute(Qt::WA_DeleteOnClose);
        error->setText("Невозможно восстановить имена");
        error->exec();
        return;
    }
    reset(oldNames, false);
}

MainWindow::~MainWindow()
{
}
