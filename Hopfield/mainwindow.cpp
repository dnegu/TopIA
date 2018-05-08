#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <fstream>
#include <QFile>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

vector<int> Matriz2Vector(CImg<float> Matriz)
{
    int x=Matriz.height();
    int y=Matriz.width();
    vector<int> salida;
    for(register int i=0;i<x;++i)
        for(register int j=0;j<y;++j)
            (Matriz(j,i)==0)? salida.push_back(-1):salida.push_back(1);
    return salida;
}

CImg<float> Vector2Matrix(vector<int> Vec)
{
    int tam = sqrt(Vec.size());
    CImg<float> salida(tam,tam);
    for(register size_t i=0;i<Vec.size();++i)
        (Vec[i]==1)?salida(i%tam,int(i/tam))=255:salida(i%tam,int(i/tam))=0;
    return salida;
}

CImg<float> transformada(string nombre)
{
    int tam = 70;
    CImg<float> img2(nombre.c_str());
    img2.resize(tam,tam);
    CImg<float> nueva(img2.width(),img2.height(),1,1,0);
    cimg_forXY(img2,x,y) {
        /*int R = (int)img2(x,y,0,0);
        int G = (int)img2(x,y,0,1);
        int B = (int)img2(x,y,0,2);
        int grayValue = (int)(0.33*R + 0.33*G + 0.33*B);*/
        (img2(x,y)>255/2)?nueva(x,y,0,0)=255:nueva(x,y,0,0)=0;
    }
    return nueva;
}

void imprimirV(vector<int> V)
{
    for(register size_t i=0;i<V.size();++i)
        cout<<V[i];
    cout<<endl;
}

void imprimirM(vector<vector<int> > M)
{
    for(register size_t i=0;i<M.size();++i){
        for(register size_t j=0;j<M[0].size();++j)
            cout<<M[i][j];
        cout<<endl;
    }
}

vector<vector<int> > calcularPesos(vector<vector<int> > entradas)
{
    int temp;
    int tam = entradas[0].size();
    vector<vector<int> > SalidaPesos(tam);
    for (register int i=0; i<tam; ++i) {
            for (register int j=0; j<tam; ++j) {
                temp = 0;
                if (i!=j) {
                    for (register size_t n=0; n<entradas.size(); n++) {
                        temp += entradas[n][i] * entradas[n][j];
                    }
                }
                SalidaPesos[i].push_back(temp);
            }
        }
    return SalidaPesos;
}

vector<int> funcion(vector<int> Vec)
{
    vector<int> salida(Vec.size());
    for(register size_t i=0; i<Vec.size(); ++i)
        (Vec[i]>=0)? salida[i]=1:salida[i]=-1;
    return salida;
}

bool vectoresIguales(vector<int> Vec,vector<int> Vec2)
{
    for(register size_t i=0; i<Vec.size(); ++i)
        if(Vec[i]!=Vec2[i]) return false;
    return true;
}

vector<int> VecPorMat(vector<int> Vec,vector<vector<int> > Matriz)
{
    vector<int> salida;
    int sum;
    for(register size_t i=0; i<Matriz.size(); i++)
    {
        sum=0;
            for(register size_t j=0; j<Matriz[0].size(); j++)
                sum+=Matriz[i][j]*Vec[j];
        salida.push_back(sum);
     }
    return salida;
}



void MainWindow::on_btn_mostrar_clicked()
{
    /*CImg<float> img = transformada("entrenar1.pgm");
    img.display();
    CImg<float> img2("entrenar1.pgm");
    img2.resize(40,40);
    //img2.display();
    /*for(int i=0;i<img.width();++i){
        for(int j=0;j<img.height();++j)
            (img(j,i)==255)? cout<<1:cout<<0;
        cout<<endl;
    }*/
    //imprimirV( Matriz2Vector(img));
    /*int M[2][4]={{1,1,-1,-1},{-1,-1,1,1}};
    vector<vector<int> > M2(2);
    for(register int i=0; i<2; i++)
            for(register int j=0; j<4; j++)
                M2[i].push_back(M[i][j]);

    vector<vector<int> > Pesos = calcularPesos(M2);
    vector<int> v2;
    v2.push_back(-1);
    v2.push_back(-1);
    v2.push_back(-1);
    v2.push_back(1);*/
    vector<vector<int> > M2;
    M2.push_back( Matriz2Vector(transformada("entrenar1.pgm")));
    M2.push_back( Matriz2Vector(transformada("entrenar2.pgm")));
    M2.push_back( Matriz2Vector(transformada("entrenar3.pgm")));
    M2.push_back( Matriz2Vector(transformada("parrot.ppm")));
    M2.push_back( Matriz2Vector(transformada("imagen.jpg")));
    //imprimirV(Matriz2Vector(transformada("entrenar1.pgm")));
    vector<vector<int> > Pesos = calcularPesos(M2);
    transformada("imagen2.jpg").display();
    vector<int> v2 = Matriz2Vector(transformada("imagen2.jpg"));
    while(!vectoresIguales(funcion(VecPorMat(v2,Pesos)),v2))
    {
        //imprimirV(v2);
        v2=funcion(VecPorMat(v2,Pesos));
    }
    //imprimirV(v2);
    CImg<float>resultado= Vector2Matrix(v2);
    resultado.display();
    /*for(int i=0;i<resultado.width();++i){
        for(int j=0;j<resultado.height();++j)
            cout<<resultado(i,j)<<" ";
        cout<<endl;
    }*/
    //imprimirM(Vector2Matrix(v2));
}

void MainWindow::funcionMain()
{
    for(size_t i=0;i<v2.size();++i)
        if(v2[i]==0) v2[i]=-1;
}

void MainWindow::funcionMain2()
{
    for(size_t i=0;i<v2.size();++i)
        if(v2[i]==-1) v2[i]=0;
}

void MainWindow::sacar()
{
    v2.clear();
    v2.push_back(ui->cb1->isChecked());v2.push_back(ui->cb2->isChecked()); v2.push_back(ui->cb3->isChecked());v2.push_back(ui->cb4->isChecked());v2.push_back(ui->cb5->isChecked());
    v2.push_back(ui->cb6->isChecked());v2.push_back(ui->cb7->isChecked()); v2.push_back(ui->cb8->isChecked());v2.push_back(ui->cb9->isChecked());v2.push_back(ui->cb10->isChecked());
    v2.push_back(ui->cb11->isChecked());v2.push_back(ui->cb12->isChecked()); v2.push_back(ui->cb13->isChecked());v2.push_back(ui->cb14->isChecked());v2.push_back(ui->cb15->isChecked());
    v2.push_back(ui->cb16->isChecked());v2.push_back(ui->cb17->isChecked()); v2.push_back(ui->cb18->isChecked());v2.push_back(ui->cb19->isChecked());v2.push_back(ui->cb20->isChecked());
    v2.push_back(ui->cb21->isChecked());v2.push_back(ui->cb22->isChecked()); v2.push_back(ui->cb23->isChecked());v2.push_back(ui->cb24->isChecked());v2.push_back(ui->cb25->isChecked());
    funcionMain();
}

void MainWindow::poner()
{
    funcionMain2();
    ui->cb1->setChecked(v2[0]);ui->cb2->setChecked(v2[1]);ui->cb3->setChecked(v2[2]);ui->cb4->setChecked(v2[3]);ui->cb5->setChecked(v2[4]);
    ui->cb6->setChecked(v2[5]);ui->cb7->setChecked(v2[6]);ui->cb8->setChecked(v2[7]);ui->cb9->setChecked(v2[8]);ui->cb10->setChecked(v2[9]);
    ui->cb11->setChecked(v2[10]);ui->cb12->setChecked(v2[11]);ui->cb13->setChecked(v2[12]);ui->cb14->setChecked(v2[13]);ui->cb15->setChecked(v2[14]);
    ui->cb16->setChecked(v2[15]);ui->cb17->setChecked(v2[16]);ui->cb18->setChecked(v2[17]);ui->cb19->setChecked(v2[18]);ui->cb20->setChecked(v2[19]);
    ui->cb21->setChecked(v2[20]);ui->cb22->setChecked(v2[21]);ui->cb23->setChecked(v2[22]);ui->cb24->setChecked(v2[23]);ui->cb25->setChecked(v2[24]);
}

void MainWindow::on_btn_entrenar_clicked()
{
    int tam = 5,temp;
    ifstream archivo("numeros.data");
    vector<vector<int> > numeros(tam);
    for(int i = 0;i<tam;++i)
        for(int j = 0;j<25;++j){
            archivo>>temp;
            numeros[i].push_back(temp);
    }
    archivo.close();
    Pesos=calcularPesos(numeros);
}

bool MainWindow::esNumero(vector<int> num)
{
    for(int i=0;i<Pesos.size();++i)
        if (!vectoresIguales(num,Pesos[i])) return false;
    return true;
}

void MainWindow::on_btn_comp_clicked()
{
    sacar();
    while(!vectoresIguales(funcion(VecPorMat(v2,Pesos)),v2))
    {
        v2=funcion(VecPorMat(v2,Pesos));
    }
    //imprimirV(v2);
    poner();
    //imprimirM(Pesos);
}

void MainWindow::on_btn_limpiar_clicked()
{
    ui->cb1->setChecked(0);ui->cb2->setChecked(0);ui->cb3->setChecked(0);ui->cb4->setChecked(0);ui->cb5->setChecked(0);
    ui->cb6->setChecked(0);ui->cb7->setChecked(0);ui->cb8->setChecked(0);ui->cb9->setChecked(0);ui->cb10->setChecked(0);
    ui->cb11->setChecked(0);ui->cb12->setChecked(0);ui->cb13->setChecked(0);ui->cb14->setChecked(0);ui->cb15->setChecked(0);
    ui->cb16->setChecked(0);ui->cb17->setChecked(0);ui->cb18->setChecked(0);ui->cb19->setChecked(0);ui->cb20->setChecked(0);
    ui->cb21->setChecked(0);ui->cb22->setChecked(0);ui->cb23->setChecked(0);ui->cb24->setChecked(0);ui->cb25->setChecked(0);
}

void MainWindow::on_btn_selec_clicked()
{
    QFile file;
}
