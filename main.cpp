#include <QTableWidget>
#include <QApplication>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMap> //a data structure that acts a key value pair to store passwords
#include <QPair>
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>

//changes for encryption
#include <QCryptographicHash> //adds cryptographic hashing

QMap <QString,QPair<QString, QString>> passwords; //defining the key-value pair

QMainWindow *mainWindow;
QTableWidget *passwordTable;

QPushButton *addButton;
QPushButton *copyButton;
QPushButton *removeButton;

void setupUI();
void addPassword();
void copyPassword();
void removePassword();

//encryption
bool verifyMasterPassword();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    setupUI();


    if(verifyMasterPassword()){
        mainWindow -> show();
    }

    return app.exec();
}

void setupUI(){
    mainWindow = new QMainWindow;
    mainWindow -> setWindowTitle("Password Manager");
    mainWindow -> setMinimumSize(600,400);

    QWidget *centralWidget = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    passwordTable = new QTableWidget(0,3); //Creates table to store password
    passwordTable -> setHorizontalHeaderLabels({"Services", "Username","Password"}); //Sets table header
    passwordTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView :: Stretch); //makes them stretched to occupy the space in the mainWindow
    passwordTable -> setEditTriggers(QAbstractItemView :: NoEditTriggers); //does not allow the table to be edited
    passwordTable -> setSelectionBehavior(QAbstractItemView :: SelectRows); //allows user to select row but not edit

    mainLayout -> addWidget(passwordTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout; //Horizontal Button Creation

    //add Password Button
    addButton = new QPushButton("Add Password");
    copyButton = new QPushButton("Copy");
    removeButton = new QPushButton("Remove Password");

    QObject :: connect(addButton, &QPushButton :: clicked, addPassword);//calls addPassword function when clicked
    QObject :: connect(copyButton, &QPushButton :: clicked, copyPassword); //calls copyPassword when clicked
    QObject :: connect(removeButton, &QPushButton :: clicked, removePassword); //calls removePassword when called


    buttonLayout -> addWidget(addButton);
    buttonLayout -> addWidget(copyButton);
    buttonLayout -> addWidget(removeButton);

    mainLayout -> addLayout(buttonLayout); //adds button to mainLayout
    mainWindow -> setCentralWidget(centralWidget); //mainWindow design

}

void addPassword(){

    //Implementation
    QString service = QInputDialog :: getText(nullptr, "Add Password", "Service");

    if (service.isEmpty()) return;

    QString username = QInputDialog :: getText(nullptr, "Add Password", "Username");
    QString password = QInputDialog :: getText(nullptr, "Add Password", "Password");

    passwords[service] = qMakePair(username, password); //creates keyVal pair

    int row = passwordTable -> rowCount();

    passwordTable -> insertRow(row);
    passwordTable -> setItem(row, 0, new QTableWidgetItem(service));
    passwordTable -> setItem(row, 1, new QTableWidgetItem(username));
    passwordTable -> setItem(row, 2, new QTableWidgetItem("**********")); //masked password

}

void copyPassword(){
    //Imp here
    int row = passwordTable -> currentRow();

    if(row >= 0){
        QString service = passwordTable -> item(row, 0) -> text();
        QApplication :: clipboard() -> setText(passwords[service].second);
        QMessageBox :: information(nullptr, "Copied", "Password copied to clipboard");
    }
}

void removePassword(){
    //Imp here
    int row = passwordTable -> currentRow();

    if(row >= 0){
        QString service = passwordTable -> item(row,0) -> text();
        passwords.remove(service);
        passwordTable -> removeRow(row);
    }
}

bool verifyMasterPassword(){
    QString input = QInputDialog :: getText(nullptr, "Master Password", "Enter Master Password", QLineEdit :: Password);

    if(input.isEmpty()) return false;

    //hash method
    QString storedHash = QCryptographicHash :: hash (
        QString("12345").toUtf8(),
        QCryptographicHash :: Sha256
                             ).toHex();

    //hash method
    QString inputHash = QCryptographicHash :: hash (
                             input.toUtf8(),
                             QCryptographicHash :: Sha256
                             ).toHex();

    return inputHash == storedHash;

}


