#include "mainwindow.h"
#include <QtWidgets>
#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPrinter>
#include <QPrintDialog>
#include "additiondatadialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    co = 0;
    yAdd =0;
    blco = 0;
    blyAdd = 0;
    flag = false;
    checkFS();
//    currentDir = "D:/";
    QTextCodec *rusCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(rusCodec);
    resize(1200,800);
    mainw = new QWidget();
    tabWidget = new QTabWidget(this);

    sqlmodel = new QSqlTableModel;
    sqlmodel->setTable("users");
    sqlmodel->setHeaderData(1,Qt::Horizontal, tr("Фамилия"));
    sqlmodel->setHeaderData(2,Qt::Horizontal, tr("Имя"));
    sqlmodel->setHeaderData(3,Qt::Horizontal, tr("Отчество"));
    sqlmodel->setHeaderData(4,Qt::Horizontal, tr("Отр"));
    sqlmodel->setEditStrategy(QSqlTableModel::OnFieldChange);
    sqlmodel->setSort(1,Qt::AscendingOrder);
    sqlmodel->select();

    QFont tableFont;
    tableFont.setStyleHint(QFont::Serif);
    tableFont.setFamily("Arial");
    tableFont.setPixelSize(11);
    view = new QTableView;
    view->setFixedWidth(380);
    view->setModel(sqlmodel);
    view->setFont(tableFont);
    view->setColumnHidden(0, true);
    view->setColumnWidth(4,32);
    view->setSelectionBehavior( QAbstractItemView::SelectRows );
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(view, SIGNAL(pressed(QModelIndex)),this,SLOT(currentIndexViewChanged()));

    setCentralWidget(mainw);

    gview = new QGraphicsView;
    scene = new QGraphicsScene(QRectF(0,0,630,891));
    scene->setBackgroundBrush(QBrush(Qt::white,Qt::SolidPattern));
    gview->setScene(scene);

    blgview = new QGraphicsView;
    blscene = new QGraphicsScene(QRectF(0,0,630,891));
    blscene->setBackgroundBrush(QBrush(Qt::white,Qt::SolidPattern));
    blgview->setScene(blscene);

    tabWidget->addTab(gview, QString(tr("Нагрудные знаки")));
    tabWidget->addTab(blgview, QString(tr("Прикроватные таблички")));

    createActions();
    createMenus();
    createToolBars();
    createGUI();

    view->addAction(createLabelAct);
    view->addAction(createBadLabelAct);
    view->addAction(addAdditionAct);
    view->addAction(delUserAct);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    QSqlQuery query;
}

MainWindow::~MainWindow()
{
 
}

void MainWindow::createGUI()
{
    QVBoxLayout *vlayout = new QVBoxLayout;
    QHBoxLayout *hlayout = new QHBoxLayout;
    QHBoxLayout *findHLayout = new QHBoxLayout;
    QVBoxLayout *photoLayout = new QVBoxLayout;
    QVBoxLayout *vlayout2 = new QVBoxLayout;
    QLabel* findL = new QLabel(this);
    findL->setPixmap(QPixmap(":/images/Search.png"));
    findL->setFixedWidth(24);
    findLE = new QLineEdit;
    findLE->setFixedWidth(266);
    findOtrLE = new QLineEdit;
    findOtrLE->setFixedWidth(20);
    clearFindButtton = new QPushButton(tr("Очистить"));
    clearFindButtton->setFixedWidth(80);
    connect(clearFindButtton,SIGNAL(clicked()), findLE, SLOT(clear()));
    photoLabel = new QLabel(this);
    photoLabel->setPixmap(QPixmap(":/images/frame.png"));

    printButton = new QPushButton(tr("Печать"));
    printButton->setEnabled(false);
    connect(printButton, SIGNAL(clicked()), this, SLOT(printSlot()));

    photoLayout->addWidget(photoLabel);
    photoLayout->addStretch();
    photoLabel->setFixedWidth(160);
    photoLabel->addAction(editInPaintAct);
    photoLabel->setContextMenuPolicy(Qt::ActionsContextMenu);
    findHLayout->addWidget(findL);
    findHLayout->addWidget(findOtrLE);
    findHLayout->addWidget(findLE);
    findHLayout->addWidget(clearFindButtton);
    connect(findLE, SIGNAL(textChanged(QString)), this, SLOT(FindTextChanged(QString)));
    connect(findOtrLE, SIGNAL(textChanged(QString)), this, SLOT(FindTextChanged(QString)));
    vlayout->addLayout(findHLayout);
    vlayout->addWidget(view);

    vlayout2->addWidget(tabWidget);
    vlayout2->addWidget(printButton);
    hlayout->addLayout(vlayout);
    hlayout->addLayout(photoLayout);
    hlayout->addLayout(vlayout2);
//    hlayout->addWidget(tabWidget);
    mainw->setLayout(hlayout);

}

QStringList MainWindow::selectByID(int id)
{
    QSqlQuery query;
    QStringList lst;
    query.exec(QString("SELECT lname,name,pname,otr FROM users WHERE user_id =%1").arg(id));
    if (query.next()){
        lst.append(query.value(0).toString());
        lst.append(query.value(1).toString());
        lst.append(query.value(2).toString());
        lst.append(query.value(3).toString());
        lst.append(QString(tr("%1Work/Бирки/bd/%2.jpg")).arg(currentDir).arg(id));
    }
    return lst;
}

void MainWindow::createActions()
{
    saveAsAct = new QAction(tr("Сохранить как"), this);
    saveAsAct->setIcon(QIcon(":/images/gtk-save-as.png"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
    saveAsAct->setEnabled(false);

    addFromDirAct = new QAction(tr("Добавить пользователей из папки"),this);
    addFromDirAct->setIcon(QIcon(":/images/list-add-user.png"));
    connect(addFromDirAct, SIGNAL(triggered()), this, SLOT(addUsersFromDir()));

    delUserAct = new QAction(tr("Удалить пользователя"),this);
    delUserAct->setIcon(QIcon(":/images/delete.png"));
    connect(delUserAct, SIGNAL(triggered()), this, SLOT(delUser()));

    labelsClearAct = new QAction("Очистить нагрудные знаки", this);
    labelsClearAct->setIcon(QIcon(":/images/edit-clear.png"));
    connect(labelsClearAct,SIGNAL(triggered()), this, SLOT(clearLabelsField()));

    createLabelAct = new QAction(tr("Создать нагрудный знак"), this);
    connect(createLabelAct, SIGNAL(triggered()), this, SLOT(createLabel()));

    editInPaintAct = new QAction(tr("Редактировать фото"),this);
    connect(editInPaintAct, SIGNAL(triggered()), this, SLOT(editInPaint()));

    createBadLabelAct = new QAction(tr("Создать прикроватную табличку"), this);
    connect(createBadLabelAct, SIGNAL(triggered()), this, SLOT(createBadLabel()));

    blSaveAsAct = new QAction(tr("Сохранить файл прикроватных табличек как..."), this);
    blSaveAsAct->setIcon(QIcon(":/images/save-as_pk.png"));
    connect(blSaveAsAct, SIGNAL(triggered()), this, SLOT (saveBLas()));
    blSaveAsAct->setEnabled(false);

    addAdditionAct = new QAction(tr("Добавить/редактировать дополнительные данные"), this);
    connect(addAdditionAct, SIGNAL(triggered()), this, SLOT(addAdditionData()));

    restoreFromTxtAct = new QAction(tr("Заполнить базу из файла дампа"), this);
    connect(restoreFromTxtAct, SIGNAL(triggered()), this, SLOT(restoreFromTxt()));

    blLabelsClearAct = new QAction(tr("Очистить прикроватные таблички"), this);
    blLabelsClearAct->setIcon(QIcon(":/images/edit-clear-pk.png"));
    connect(blLabelsClearAct, SIGNAL(triggered()), this, SLOT(clearBlField()));


}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("Файл"));
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(blSaveAsAct);

    fileMenu->addAction(addFromDirAct);
    fileMenu->addAction(restoreFromTxtAct);

    userMenu = menuBar()->addMenu(tr("Пользователи"));
    userMenu->addAction(delUserAct);

}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("Файл"));
    fileToolBar->addAction(addFromDirAct);
    fileToolBar->addAction(delUserAct);
    fileToolBar->addAction(saveAsAct);
    fileToolBar->addAction(labelsClearAct);
    fileToolBar->addAction(blSaveAsAct);
    fileToolBar->addAction(blLabelsClearAct);

}

void MainWindow::FindTextChanged(QString txt)
{
   QString otrFilter;
   if (!findOtrLE->text().isEmpty()) otrFilter=QString(" AND otr=%1").arg(findOtrLE->text());
   else otrFilter = QString("");
   sqlmodel->setFilter(QString("lname LIKE \"%1%\""+otrFilter).arg(findLE->text()));
   sqlmodel->select();
}

void MainWindow::currentIndexViewChanged()
{
    int curind = view->currentIndex().row();
    int id = sqlmodel->index(curind,0).data().toInt();
    QString path = QString(tr("%1Work/Бирки/bd/%2.jpg")
                   .arg(currentDir)
                   .arg(id));
    QSqlQuery query;
    if (!path.isEmpty()) photoLabel->setPixmap(QPixmap(path).scaledToWidth(160,Qt::SmoothTransformation));
    else photoLabel->setPixmap(QPixmap(":/images/frame.png"));
    query.exec(QString("SELECT born_d, end FROM addition WHERE user_id=%1")
               .arg(id));
    if (query.next()) {
        statusBar()->showMessage(QString(tr("Год рождения: %1 Конец срока: %2"))
                                 .arg(query.value(0).toString())
                                 .arg(query.value(1).toString()));
        createBadLabelAct->setEnabled(true);
    }
    else{
        statusBar()->showMessage(QString(tr("Нет данных")));
        createBadLabelAct->setEnabled(false);
    }

}

void MainWindow::createLabel()
{
    tabWidget->setCurrentIndex(0);
    int x,y;
    QStringList lst;
    int curind = view->currentIndex().row();
    int id = sqlmodel->index(curind,0).data().toInt();
    lst = selectByID(id);
    if (co%2 == 1){
        x = 315;
    }
    else {
        x = 30;
        yAdd++;
        if (co == 0) yAdd = 0;
    }
    y = yAdd*140+20;
    saveAsAct->setEnabled(true);
    printButton->setEnabled(true);
    drawLabel(x ,y, id,lst);

}
void MainWindow::drawLabel(int x, int y, int id, QStringList lst)
{
    QString lname,name,pname,otr,path;
    lname = lst[0];
    lname[0] = lname[0].toUpper();
    name = lst[1];
    name[0] = name[0].toUpper();
    pname = lst[2];
    pname[0] = pname[0].toUpper();
    otr = lst[3] + QString(tr(" отряд"));
    path = QString(tr("%1Work/Бирки/bd/%2.jpg")
                   .arg(currentDir)
                   .arg(id));
    QFont fl;
    fl.setBold(true);
    fl.setPixelSize(20);
    fl.setStyleHint(QFont::Serif);
    fl.setFamily("Times");
    QFontMetrics fontMet(fl);
    QGraphicsRectItem *rectItem = new QGraphicsRectItem();
    scene->addItem(rectItem);
    rectItem->setPen(QPen(Qt::black,4));
    rectItem->setRect(0,0,280,135);
    rectItem->setPos(x,y);

    QGraphicsRectItem *photoRectItem = new QGraphicsRectItem();
    photoRectItem->setPen(QPen(Qt::black,2));
    photoRectItem->setRect(0,0,100,127);
    photoRectItem->setPos(x+4,y+4);
    scene->addItem(photoRectItem);

    QGraphicsRectItem *textRectItem = new QGraphicsRectItem();
    textRectItem->setPen(QPen(Qt::black,2));
    textRectItem->setRect(0,0,168,127);
    textRectItem->setPos(x+108,y+4);
    scene->addItem(textRectItem);

    QPixmap photoPix(path);
    photoPix = photoPix.scaled(96,122,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    QGraphicsPixmapItem *photoItem = new QGraphicsPixmapItem(photoPix);
    photoItem->setPos(x+8,y+6);
    scene->addItem(photoItem);

    QGraphicsTextItem *lnameItem = new QGraphicsTextItem(lname);
    lnameItem->setDefaultTextColor(Qt::black);
    int wLname = fontMet.width(lname);
    lnameItem->setPos(x+100+(168-wLname)/2,y+22);
    lnameItem->setFont(fl);
    scene->addItem(lnameItem);

    QGraphicsTextItem *nameItem = new QGraphicsTextItem(name);
    nameItem->setDefaultTextColor(Qt::black);
    nameItem->setPos(x+100+(168-fontMet.width(name))/2,y+44);
    nameItem->setFont(fl);
    scene->addItem(nameItem);

    QGraphicsTextItem *pNameItem = new QGraphicsTextItem(pname);
    pNameItem->setDefaultTextColor(Qt::black);
    pNameItem->setPos(x+100+(168-fontMet.width(pname))/2,y+64);
    pNameItem->setFont(fl);
    scene->addItem(pNameItem);

    QGraphicsTextItem *oNameItem = new QGraphicsTextItem(otr);
    oNameItem->setDefaultTextColor(Qt::black);
    oNameItem->setPos(x+100+(168-fontMet.width(otr))/2,y+92);
    oNameItem->setFont(fl);
    scene->addItem(oNameItem);

    co++;
    if (co >= 12) {
        saveAs();
//        if (labelFl == 0) saveAs();
//        else saveLabelsByOtr(labelByOtrOtr.toInt());
    }
}

void MainWindow::drawBadLabel(int x, int y, int id, QStringList lst)
{
    QString lname,name,pname,otr,path,born,stat,period,begin,end;
    lname = lst[0];
    lname[0] = lname[0].toUpper();
    name = lst[1];
    name[0] = name[0].toUpper();
    pname = lst[2];
    pname[0] = pname[0].toUpper();
    otr = lst[3];
    path = QString(tr("%1Work/Бирки/bd/%2.jpg")
                   .arg(currentDir)
                   .arg(id));
    born = lst[5];
    stat = lst[6];
    period = lst[7];
    begin = lst[8];
    end = lst[9];
    QFont fl;
    fl.setBold(true);
    fl.setPixelSize(20);
    fl.setStyleHint(QFont::Serif);
    fl.setFamily("Times");
    QFontMetrics fontMet(fl);

    QFont fl1;
    fl1.setBold(true);
//    fl1.setItalic(true);
    fl1.setPixelSize(14);
    fl1.setStyleHint(QFont::Serif);
    fl1.setStyleHint(QFont::Serif);
    fl1.setFamily("Times");

    QFont fl2;
    fl2.setBold(true);
    fl2.setPixelSize(12);
    fl2.setStyleHint(QFont::Serif);
    fl2.setFamily("Times");

    QGraphicsRectItem *rectItem = new QGraphicsRectItem();
    rectItem->setPen(QPen(Qt::black,4));
    rectItem->setRect(0,0,280,200);
    rectItem->setPos(x,y);
    blscene->addItem(rectItem);

    QGraphicsRectItem *rectitem2 = new QGraphicsRectItem();
    rectitem2->setPen(QPen(Qt::black,2));
    rectitem2->setRect(4,4,272,192);
    rectitem2->setPos(x,y);
    blscene->addItem(rectitem2);

    QGraphicsRectItem *rectitem3 = new QGraphicsRectItem();
    rectitem3->setPen(QPen(Qt::black,2));
    rectitem3->setRect(252,4,24,26);
    rectitem3->setPos(x,y);
    blscene->addItem(rectitem3);


    QGraphicsLineItem *lineitem = new QGraphicsLineItem();
    lineitem->setPen(QPen(Qt::black,2));
    lineitem->setLine(6,130,274,130);
    lineitem->setPos(x,y);
    blscene->addItem(lineitem);

    QGraphicsLineItem *lineitem2 = new QGraphicsLineItem();
    lineitem2->setPen(QPen(Qt::black,2));
    lineitem2->setLine(110,4,110,130);
    lineitem2->setPos(x,y);
    blscene->addItem(lineitem2);

    QGraphicsLineItem *lineitem3 = new QGraphicsLineItem();
    lineitem3->setPen(QPen(Qt::black,1));
    lineitem3->setLine(6,152,274,152);
    lineitem3->setPos(x,y);
    blscene->addItem(lineitem3);

    QGraphicsLineItem *lineitem4 = new QGraphicsLineItem();
    lineitem4->setPen(QPen(Qt::black,1));
    lineitem4->setLine(6,174,274,174);
    lineitem4->setPos(x,y);
    blscene->addItem(lineitem4);




    QPixmap photoPix(path);
    photoPix = photoPix.scaled(96,122,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    QGraphicsPixmapItem *photoItem = new QGraphicsPixmapItem(photoPix);
    photoItem->setPos(x+8,y+6);
    blscene->addItem(photoItem);

    QGraphicsTextItem *lnameItem = new QGraphicsTextItem(lname);
    lnameItem->setDefaultTextColor(Qt::black);
    int wLname = fontMet.width(lname);
    lnameItem->setPos(x+100+(168-wLname)/2,y+12);
    lnameItem->setFont(fl);
    blscene->addItem(lnameItem);

    QGraphicsTextItem *nameItem = new QGraphicsTextItem(name);
    nameItem->setDefaultTextColor(Qt::black);
    nameItem->setPos(x+100+(168-fontMet.width(name))/2,y+34);
    nameItem->setFont(fl);
    blscene->addItem(nameItem);

    QGraphicsTextItem *pNameItem = new QGraphicsTextItem(pname);
    pNameItem->setDefaultTextColor(Qt::black);
    pNameItem->setPos(x+100+(168-fontMet.width(pname))/2,y+54);
    pNameItem->setFont(fl);
    blscene->addItem(pNameItem);

    QGraphicsTextItem *otrItem = new QGraphicsTextItem(otr);
    otrItem->setDefaultTextColor(Qt::black);
    otrItem->setPos(x + 251,y + 1);
    otrItem->setFont(fl);

    blscene->addItem(otrItem);

    QGraphicsTextItem *bornItem = new QGraphicsTextItem(QString(tr("Год рождения: %1"))
                                                        .arg(born));
    bornItem->setDefaultTextColor(Qt::black);
    bornItem->setPos(x+120, y+84);
    bornItem->setFont(fl1);
    blscene->addItem(bornItem);

    QGraphicsTextItem *periodItem = new QGraphicsTextItem(QString(tr("Срок: %1"))
                                                        .arg(period));
    periodItem->setDefaultTextColor(Qt::black);
    periodItem->setPos(x+120, y+104);
    periodItem->setFont(fl1);
    blscene->addItem(periodItem);

    QGraphicsTextItem *statItem = new QGraphicsTextItem(QString(tr("Статья: %1"))
                                                        .arg(stat));
    statItem->setDefaultTextColor(Qt::black);
    statItem->setPos(x+6, y+128);
    statItem->setFont(fl2);
    blscene->addItem(statItem);

    QGraphicsTextItem *beginItem = new QGraphicsTextItem(QString(tr("Начало срока: %1"))
                                                        .arg(begin));
    beginItem->setDefaultTextColor(Qt::black);
    beginItem->setPos(x+6, y+150);
    beginItem->setFont(fl2);
    blscene->addItem(beginItem);

    QGraphicsTextItem *endItem = new QGraphicsTextItem(QString(tr("Конец   срока: %1"))
                                                        .arg(end));
    endItem->setDefaultTextColor(Qt::black);
    endItem->setPos(x+6, y+172);
    endItem->setFont(fl2);
    blscene->addItem(endItem);


    blco++;
    if (blco >= 8) saveBLas();

}

void MainWindow::labelCountClear(){co = 0; yAdd = 0;scene->clear(); saveAsAct->setEnabled(false);}

bool MainWindow::userExcists(QString lname, QString name, QString pname)
{
    QSqlQuery query;
    query.exec(QString("SELECT user_id FROM users WHERE lname=\"%1\" AND name=\"%2\" AND pname=\"%3\";")
               .arg(lname).arg(name).arg(pname));
    if (query.next()) return true;
    else return false;
}

int MainWindow::getID(QString lname, QString name, QString pname, QString otr)
{
    QSqlQuery query;
    query.exec(QString("SELECT user_id FROM users WHERE lname=\"%1\" AND name=\"%2\" AND pname=\"%3\" AND otr=%4;")
               .arg(lname).arg(name).arg(pname).arg(otr));
    if (query.next()) return query.value(0).toInt();
    else return 0;
}

void MainWindow::checkFS()
{
    QFileInfo fi("settings.conf");
    if (fi.exists()){
        QFile file("settings.conf");
        if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this,tr("Ошибка открытия файла"),
                                  tr("Невозможно открыть файл настроек, возможно файл поврежден"));
            return;
        }
        QTextStream in(&file);
        in >> currentDir;
        currentDir += "/";
        qDebug() << "Exists: " << currentDir;
        return;
    }
    else {
        QFile file("settings.conf");
        if (!file.open(QIODevice::WriteOnly)){
            QMessageBox::critical(this, tr("Ошибка записи файла"),
                                  tr("Не могу сохранить настройки"));
            return;
        }
        QTextStream out(&file);
        QString dirname = QFileDialog::getExistingDirectory(this,
                                                           tr("Выберите папку где будет храниться база данных"),
                                                           tr("/"));
         if (dirname.isEmpty()) return;
         currentDir = dirname;
         out << currentDir;
//         currentDir += "//";
         QDir dir(currentDir);
         if (!dir.exists("Work")){
             dir.mkpath(tr("Work/Бирки/Tmp"));
             dir.mkpath(tr("Work/Бирки/Render"));
             dir.mkpath(tr("Work/Бирки/Render_pk"));
             dir.mkpath(tr("Work/Бирки/bd"));
         }
    }
    return;
}

void MainWindow::saveAs()
{

    QString fname = QFileDialog::getSaveFileName(this, tr("Сохранить файл нагрудных знаков как"),
                                                 QString(tr("%1Work/Бирки/Render/")
                                                         .arg(currentDir)), "*.png");
    if (fname.isEmpty()) return;
    QFileInfo fi(fname);
    QString ext = fi.suffix();
    if (ext != "png") fname += ".png";
    QImage image(scene->width(), scene->height(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    scene->render(&painter);
    image.save(fname);
    labelCountClear();
}

void MainWindow::addUserSL()
{

}

void MainWindow::addUsersFromDir()
{
    QString dirname = QFileDialog::getExistingDirectory(this,
                                                      tr("Выберите папку с фотографиями для добавления"),
                                                      QString(tr("%1Work/Бирки/Tmp/")).arg(currentDir));
    if (dirname.isEmpty()) return;
    dirname += "/";
    QDir dir(dirname);
    QFile file;
    int id;
    int countL = 0;
    QSqlQuery query;
    QStringList lst,extlst;
    QString lname, name, pname, otr,newname;
    QStringList fileLst = dir.entryList();
    int fileCount = fileLst.count();
    if (fileCount == 0) return;
    for (int i=0; i< fileCount; i++){
        extlst = fileLst[i].split(".");
        lst = extlst[0].split(" ");
        if (lst.count() == 4){
            lname = lst[0].trimmed().toLower();
            name = lst[1].trimmed().toLower();
            pname = lst[2].trimmed().toLower();
            otr = lst[3].trimmed().toLower();
            if (!userExcists(lname,name,pname)){
               query.exec(QString("INSERT INTO users VALUES(NULL,\"%1\",\"%2\",\"%3\",%4);")
                          .arg(lname)
                          .arg(name)
                          .arg(pname)
                          .arg(otr));
               id = getID(lname, name, pname, otr);
               countL++;
            QPixmap pix(dirname+fileLst[i]);
            if (pix.width() > 480) pix = pix.scaledToWidth(480,Qt::SmoothTransformation);
            newname = QString(tr("%1Work/Бирки/bd/%2.jpg")).arg(currentDir).arg(id);
            pix.save(newname);
            file.setFileName(dirname+fileLst[i]);
            file.remove();
            }
            sqlmodel->select();
        }
    }
    QMessageBox::information(this,tr("Добавление из папки с фото"),
                             QString(tr("Добавлено %1 пользователей")).arg(countL));
}

void MainWindow::delUser()
{
    int res = QMessageBox::question(this,tr("Удаление пользователя"),
                                    tr("Вы действительно хотите удалить пользователя?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::No) return;
    QFile file;
    QSqlQuery query;
    int curind = view->currentIndex().row();
    int id = sqlmodel->index(curind,0).data().toInt();
    file.setFileName(QString(tr("%1Work/Бирки/bd/%2.jpg")).arg(currentDir).arg(id));
    file.remove();
    query.exec(QString("DELETE FROM users WHERE user_id=%1;").arg(id));
    sqlmodel->select();
    query.exec(QString("DELETE FROM addition WHERE user_id=%1;").arg(id));
}

void MainWindow::clearLabelsField()
{
    labelCountClear();
}

void MainWindow::editInPaint()
{
    int curind = view->currentIndex().row();
    int id = sqlmodel->index(curind,0).data().toInt();
    QString path = QString(tr("%1Work/Бирки/bd/%2.jpg")).arg(currentDir).arg(id);
    QProcess* proc = new QProcess(this);
    path = path.replace("/","\\");
    proc->start(QString("mspaint %1").arg(path));

}

void MainWindow::createBadLabel()
{
    tabWidget->setCurrentIndex(1);
    int x,y;
    QStringList lst;
    QSqlQuery query;
    int curind = view->currentIndex().row();
    int id = sqlmodel->index(curind,0).data().toInt();
    lst = selectByID(id);
    query.exec(QString("SELECT * FROM addition WHERE user_id=%1").arg(id));
    if (!query.next()) {
        statusBar()->showMessage(tr("Нет дополнительных данных"), 3000);
        return;
    }
    lst.append(query.value(1).toString());
    lst.append(query.value(2).toString());
    lst.append(query.value(3).toString());
    lst.append(query.value(4).toString());
    lst.append(query.value(5).toString());
    if (blco%2 == 1){
        x = 315;
    }
    else {
        x = 30;
        blyAdd++;
        if (blco == 0) blyAdd = 0;
    }
    y = blyAdd*205+20;
    blSaveAsAct->setEnabled(true);
    drawBadLabel(x, y, id, lst);

}

void MainWindow::saveBLas()
{
    if (!blco){
        QMessageBox::critical(this, tr("Ошибка сохранения"), tr("Нечего сохранять\n"
                                                                "Не создано ни одной прикроватной таблички"));
        return;
    }
    QString fname = QFileDialog::getSaveFileName(this, tr("Сохранить файл прикроватных табличек как"),
                                                 QString(tr("%1/Work/Бирки/Render_pk/")
                                                         .arg(currentDir)), "*.png");
    if (fname.isEmpty()) return;
    QFileInfo fi(fname);
    QString ext = fi.suffix();
    if (ext != "png") fname += ".png";
    QImage image(blscene->width(), blscene->height(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    blscene->render(&painter);
    image.save(fname);
    clearBlField();
}

void MainWindow::clearBlField()
{
    blco = 0;
    blyAdd = 0;
    blscene->clear();
    blSaveAsAct->setEnabled(false);
}

void MainWindow::addAdditionData()
{
    int curind = view->currentIndex().row();
    int id = sqlmodel->index(curind,0).data().toInt();
    QStringList lst = selectByID(id);
    QString lname, name, pname;
    lname = lst[0];
    lname[0] = lname[0].toUpper();
    name = lst [1];
    name[0] = name[0].toUpper();
    pname = lst [2];
    pname[0] = pname[0].toUpper();
    QSqlQuery query;
    query.exec(QString("SELECT * FROM  addition WHERE user_id=%1")
               .arg(id));
    lst = selectByID(id);

    dialog = new AdditionDataDialog(this);

    if (query.next()){
        flag = true;
        dialog->bornLE->setText(query.value(1).toString());
        dialog->statLE->setText(query.value(2).toString());
        dialog->periodLE->setText(query.value(3).toString());
        dialog->beginLE->setText(query.value(4).toString());
        dialog->endLE->setText(query.value(5).toString());
        dialog->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else{
        flag = false;
        dialog->bornLE->setText("19");
        dialog->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    connect(dialog->bornLE, SIGNAL(textChanged(QString)), this, SLOT(additionDataChanged(QString)));
    connect(dialog->statLE, SIGNAL(textChanged(QString)), this, SLOT(additionDataChanged(QString)));
    connect(dialog->periodLE, SIGNAL(textChanged(QString)), this, SLOT(additionDataChanged(QString)));
    connect(dialog->beginLE, SIGNAL(textChanged(QString)), this, SLOT(additionDataChanged(QString)));
    connect(dialog->endLE, SIGNAL(textChanged(QString)), this, SLOT(additionDataChanged(QString)));
    connect(dialog->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this,SLOT(okAddDataButtonClicked()));
    connect(dialog->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this,SLOT(cancelDataButtonClicked()));
    dialog->setWindowTitle(QString("%1 %2 %3")
                           .arg(lname)
                           .arg(name)
                           .arg(pname));
    dialog->setWindowIcon(QIcon(":/images/Edit-Male-User.png"));
    dialog->show();
}

void MainWindow::additionDataChanged(QString str){
    if (str.isEmpty()) statusBar()->showMessage(tr("Необходимо заполнить все поля"), 1000);
    if (dialog->beginLE->text().length() == 2) dialog->beginLE->setText(dialog->beginLE->text()+".");
    if (dialog->beginLE->text().length() == 5) dialog->beginLE->setText(dialog->beginLE->text()+".20");
    if (dialog->endLE->text().length() == 2) dialog->endLE->setText(dialog->endLE->text()+".");
    if (dialog->endLE->text().length() == 5) dialog->endLE->setText(dialog->endLE->text()+".20");
    if ((!dialog->bornLE->text().isEmpty())
        &(!dialog->statLE->text().isEmpty())
        &(!dialog->periodLE->text().isEmpty())
        &(!dialog->beginLE->text().isEmpty())
        &(!dialog->endLE->text().isEmpty()))
        dialog->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else  dialog->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void MainWindow::okAddDataButtonClicked()
{
    int curind = view->currentIndex().row();
    int id = sqlmodel->index(curind,0).data().toInt();

    QSqlQuery query;
//    qDebug() << id;
    if (flag) query.exec(QString("UPDATE addition SET born_d=%1,stat=\"%2\",period=\"%3\",begin=\"%4\",end=\"%5\" WHERE user_id=%6")
               .arg(dialog->bornLE->text().trimmed())
               .arg(dialog->statLE->text().trimmed())
               .arg(dialog->periodLE->text().trimmed())
               .arg(dialog->beginLE->text().trimmed())
               .arg(dialog->endLE->text().trimmed())
               .arg(id));
    else query.exec(QString("INSERT INTO addition VALUES(%1,\"%2\",\"%3\",\"%4\",\"%5\",\"%6\")")
                    .arg(id)
                    .arg(dialog->bornLE->text().trimmed())
                    .arg(dialog->statLE->text().trimmed())
                    .arg(dialog->periodLE->text().trimmed())
                    .arg(dialog->beginLE->text().trimmed())
                    .arg(dialog->endLE->text().trimmed()));
}

void MainWindow::cancelDataButtonClicked()
{
    delete dialog;
}

void MainWindow::restoreFromTxt()
{

}

void MainWindow::printSlot()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this) ;
    if (printDialog.exec()){
        QPainter painter(&printer);
        if (tabWidget->currentIndex() == 0) scene->render(&painter);
        else if (tabWidget->currentIndex() == 1) blscene->render(&painter);
    }

}



