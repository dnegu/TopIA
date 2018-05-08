#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void sacar();

    void poner();

    void funcionMain();

    void funcionMain2();

    bool esNumero(vector<int>);

private slots:
    void on_btn_mostrar_clicked();

    void on_btn_entrenar_clicked();

    void on_btn_comp_clicked();

    void on_btn_limpiar_clicked();

    void on_btn_selec_clicked();

private:
    Ui::MainWindow *ui;
    vector< vector<int> > Pesos;
    vector<int> v2;
};

#endif // MAINWINDOW_H
