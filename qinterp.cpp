#include "qinterp.h"
#include <vector>
#include <functional>

string num2str(double num)
{
    stringstream stream;
    stream<<num;
    return stream.str();
}
double str2num(string str)
{
    stringstream stream(str);
    double num;
    stream>>num;
    return num;
}


QInterp::QInterp(QWidget *parent) : QWidget(parent)
{
    x = new QLineEdit;
    y = new QLineEdit;
    X = new QLineEdit;
    fX = new QLineEdit;
    func = new QTextEdit;
    func->setReadOnly(true);
    fX->setReadOnly(true);

    QLabel *xLabel = new QLabel("&x");
    QLabel *yLabel = new QLabel("&y");
    QLabel *XLabel = new QLabel("x&*");
    QLabel *fXLabel = new QLabel("&L(x*)");

    lagrange = new QPushButton("&Lagrange");
    calc = new QPushButton("&Calc");
    calcNewtonForward = new QPushButton("Calc&Newton");


    xLabel->setBuddy(x);
    yLabel->setBuddy(y);
    XLabel->setBuddy(X);
    fXLabel->setBuddy(fX);

    QHBoxLayout *xLay = new QHBoxLayout;
    xLay->addWidget(xLabel);
    xLay->addWidget(x);
    QHBoxLayout *yLay = new QHBoxLayout;
    yLay->addWidget(yLabel);
    yLay->addWidget(y);
    QHBoxLayout *XLay = new QHBoxLayout;
    XLay->addWidget(XLabel);
    XLay->addWidget(X);
    XLay->addWidget(fXLabel);
    XLay->addWidget(fX);
    QHBoxLayout *buttonLay = new QHBoxLayout;
    buttonLay->addWidget(lagrange);
    buttonLay->addWidget(calc);
    buttonLay->addWidget(calcNewtonForward);

    QVBoxLayout *mainLay = new QVBoxLayout;
    mainLay->addLayout(xLay);
    mainLay->addLayout(yLay);
    mainLay->addLayout(XLay);
    mainLay->addLayout(buttonLay);
    mainLay->addWidget(func);

    setLayout(mainLay);

    QObject::connect(lagrange, SIGNAL(clicked(bool)), SLOT(lagrangeSlot(bool)));
    QObject::connect(calc, SIGNAL(clicked(bool)), SLOT(calcSlot(bool)));
    QObject::connect(calcNewtonForward, SIGNAL(clicked(bool)), SLOT(calcSlotNewtonForward(bool)));
}


void QInterp::lagrangeSlot(bool)
{

    lag = new Lagrange(x->text().toStdString(), y->text().toStdString());

    string L = "L(x) = " + lag->getL();
    for(string i : lag->getl())
        L += "l(x) = " + i;

    func->setText( L.c_str() );
}

void QInterp::calcSlot(bool)
{

    double num = X->text().toDouble();
    double fx = lag->calc(num);
    fX->setText(num2str(fx).c_str());
}

void QInterp::calcSlotNewtonForward(bool)
{

    double num = X->text().toDouble();
    double fx = lag->calcNewtoneForward(num);
    fX->setText(num2str(fx).c_str());
}

///////////////////////////////////////////

Lagrange::Lagrange(vector<double> _x, vector<double> _y)
{
    setX(_x); setY(_y);
    create();
}

Lagrange::Lagrange(string _x, string _y, string sep)
{
    setX(_x, sep); setY(_y, sep);
    create();
}

void Lagrange::create()
{
    for (auto xi = begin(x), yi = begin(y); xi != end(x) && yi != end(y); ++xi, ++yi)
    {
        string li = " ";
        for (auto xj = begin(x); xj != end(x); ++xj)
            if (xj != xi)
            {
                li += "(x-" + num2str(*xj) + ")/" + "(" + num2str(*xi - *xj) + ")*";

                calcText.push_back("x");
                calcText.push_back( num2str(*xj) );
                calcText.push_back("-");
                calcText.push_back( num2str(*xi - *xj) );
                calcText.push_back("/");
            }
        li += num2str(*yi) + "\n";
        l.push_back(li);

        calcText.push_back( num2str(*yi) );
        for(size_t k = 1; k<x.size(); ++k)
            calcText.push_back("*");

    }


    L = " ";
    for(string i : l)
    {

        i.erase(i.end()-1);
        L  += i + "+";

        calcText.push_back("+");
    }
    L.erase(L.end()-1);;
    L += "\n";

    calcText.pop_back();
}

double Lagrange::calcNewtoneForward(double _x)
{
    int n = x.size();
    double res = y[0], F, den;
    int i, j, k;
    for (i = 1; i <= n; i++)
    {
        F = 0;
        for (j = 0; j <= i; j++)
        {
            den = 1;
            for (k = 0; k <= i; k++)
                if (k != j) den *= (x[j] - x[k]);
            F += y[j] / den;
        }
        for (k = 0; k < i; k++)
            F *= (_x - x[k]);
        res += F;
    }
    return res;
}

//////CALC////////////////////////////
double Lagrange::calc(double n)
{
    map<string, function<double(double, double)> >  op;
    op.insert(pair<string, function<double(double, double)> >("+", function<double(double, double)>([](double f, double s){return f+s;})));
    op.insert(pair<string, function<double(double, double)> >("-", function<double(double, double)>([](double f, double s){return f-s;})));
    op.insert(pair<string, function<double(double, double)> >("*", function<double(double, double)>([](double f, double s){return f*s;})));
    op.insert(pair<string, function<double(double, double)> >("/", function<double(double, double)>([](double f, double s){return f/s;})));

    double first, second;
    vector<double> v;
    for(auto i : calcText)
    {
        if("x" == i)
            v.push_back(n);
        else if(0 == op.count(i))
            v.push_back( str2num(i) );
        else
        {
            second = v.back();
            v.pop_back();
            first = v.back();
            v.pop_back();
            v.push_back( (op.at(i))(first, second) );
        }
    }
    return v.back();
}

//////////////GET//////////////////////////
string Lagrange::getL()
{
    return L;
}
vector<string> Lagrange::getl()
{
    return l;
}
vector<double> Lagrange::getX()     //+
{
    return x;
}
vector<double> Lagrange::getY()     //+
{
    return y;
}
//////////SET///////////////////////////
void Lagrange::setX(vector<double> _x)
{
    x = _x;
}
void Lagrange::setY(vector<double> _y)
{
    y = _y;
}
void Lagrange::setX(string _x, string sep)
{
    regex regex(sep);
    for(sregex_token_iterator it(begin(_x), end(_x), regex, -1), last; it != last; ++it)
      x.push_back(str2num(it->str()));
}
void Lagrange::setY(string _y, string sep)
{
    regex regex(sep);
    for(sregex_token_iterator it(begin(_y), end(_y), regex, -1), last; it != last; ++it)
      y.push_back(str2num(it->str()));
}
