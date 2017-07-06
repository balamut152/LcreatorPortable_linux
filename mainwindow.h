#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QSqlTableModel>
#include "additiondatadialog.h"

class AdditionDataDialog;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString currentDir;
private:
    int co;
    int blco;
    int yAdd;
    int blyAdd;
    bool flag;
    QWidget *mainw;
    QTableView *view;
    QLineEdit *findLE;
    QLineEdit *findOtrLE;
    QPushButton *clearFindButtton;
    QPushButton *printButton;
    QGraphicsScene *scene;
    QGraphicsScene *blscene;
    QGraphicsView *gview;
    QGraphicsView *blgview;
    QLabel *photoLabel;
    QLabel *movieLabel;
    QMovie *movie;
    QTabWidget *tabWidget;
    AdditionDataDialog *dialog;

    QAction *saveAsAct;
    QAction *addFromDirAct;
    QDialog *addUserDialog;
    QAction *delUserAct;
    QAction *labelsClearAct;
    QAction *blLabelsClearAct;
    QAction *createLabelAct;
    QAction *editInPaintAct;
    QAction *createBadLabelAct;
    QAction *blSaveAsAct;
    QAction *addAdditionAct;
    QAction *restoreFromTxtAct;

    QToolBar *fileToolBar;

    QMenu *fileMenu;
    QMenu *userMenu;

    QSqlTableModel *sqlmodel;
    void createGUI();
    QStringList selectByID(int id);

    void createActions();
    void createMenus();
    void createToolBars();
    void drawLabel(int x, int y,int id, QStringList lst);
    void drawBadLabel(int x, int y,int id, QStringList lst);
    void labelCountClear();
    bool userExcists(QString lname, QString name,QString pname );
    int getID(QString lname, QString name,QString pname, QString otr);
    void checkFS();


private slots:
    void FindTextChanged(QString txt);
    void currentIndexViewChanged();
    void createLabel();
    void saveAs();
    void addUserSL();
    void addUsersFromDir();
    void delUser();
    void clearLabelsField();
    void editInPaint();
    void createBadLabel();
    void saveBLas();
    void clearBlField();
    void addAdditionData();
    void additionDataChanged(QString str);
    void okAddDataButtonClicked();
    void cancelDataButtonClicked();
    void restoreFromTxt();
    void printSlot();
};

#endif // MAINWINDOW_H
