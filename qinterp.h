#ifndef QINTERP_H
#define QINTERP_H

#include <QtWidgets>
#include <string>
#include <vector>
#include <sstream>
#include <regex>

using namespace std;




class Lagrange //: public Polinom
{
protected:
    string L;
    vector<string> l;

    vector<double> x;
    vector<double> y;

public:
    vector<string> calcText;

    Lagrange() {}
    Lagrange(vector<double> _x, vector<double> _y);
    Lagrange(string _x, string _y, string sep = ";");

    void create();
    double calc(double n);
    double calcNewtoneForward(double n);

    string getL();
    vector<string> getl();
    vector<double> getX();
    vector<double> getY();

    void setX(vector<double> _x);
    void setY(vector<double> _y);
    void setX(string _x, string sep = ";");
    void setY(string _y, string sep = ";");
};

class QInterp : public QWidget
{
    Q_OBJECT

private:
    QLineEdit *x;
    QLineEdit *y;
    QLineEdit *X;
    QLineEdit *fX;
    QPushButton *lagrange;
    QPushButton *calc;
    QPushButton *calcNewtonForward;
    QTextEdit *func;
    Lagrange *lag;

public:
    QInterp(QWidget *parent = 0);
    //~QInterp();

public slots:
    void lagrangeSlot(bool);
    void calcSlot(bool);
    void calcSlotNewtonForward(bool);
};






#endif // QINTERP_H
