#include "extenddialog.h"
#include "ui_extenddialog.h"
#include <sstream>
#include <QToolButton>

using namespace std;
using namespace cagd;

ExtendDialog::ExtendDialog(
        DCoordinate3 d_00, DCoordinate3 d_01,
        DCoordinate3 d_02, DCoordinate3 d_03,

        DCoordinate3 d_10, DCoordinate3 d_11,
        DCoordinate3 d_12, DCoordinate3 d_13,

        DCoordinate3 d_20, DCoordinate3 d_21,
        DCoordinate3 d_22, DCoordinate3 d_23,

        DCoordinate3 d_30, DCoordinate3 d_31,
        DCoordinate3 d_32, DCoordinate3 d_33,

        QWidget *parent
    ) :
        QDialog(parent),
        ui(new Ui::ExtendDialog),
        _data(4, 4)
{
    ui->setupUi(this);
    stringstream s;

    s << d_00;
    ui->e_00->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_01;
    ui->e_01->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_02;
    ui->e_02->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_03;
    ui->e_03->setText(QString::fromStdString(s.str()));
    s.str("");


    s << d_10;
    ui->e_10->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_11;
    ui->e_11->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_12;
    ui->e_12->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_13;
    ui->e_13->setText(QString::fromStdString(s.str()));
    s.str("");


    s << d_20;
    ui->e_20->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_21;
    ui->e_21->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_22;
    ui->e_22->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_23;
    ui->e_23->setText(QString::fromStdString(s.str()));
    s.str("");


    s << d_30;
    ui->e_30->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_31;
    ui->e_31->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_32;
    ui->e_32->setText(QString::fromStdString(s.str()));
    s.str("");

    s << d_33;
    ui->e_33->setText(QString::fromStdString(s.str()));

}

void ExtendDialog::accept()
{
    DCoordinate3 d;

    stringstream s00;
    s00 << ui->e_00->text().toStdString();
    s00 >> d;
    _data(0,0) = d;

    stringstream s01;
    s01 << ui->e_01->text().toStdString();
    s01 >> d;
    _data(0,1) = d;

    stringstream s02;
    s02 << ui->e_02->text().toStdString();
    s02 >> d;
    _data(0,2) = d;


    stringstream s03;
    s03 << ui->e_03->text().toStdString();
    s03 >> d;
    _data(0,3) = d;



    stringstream s10;
    s10 << ui->e_10->text().toStdString();
    s10 >> d;
    _data(1,0) = d;


    stringstream s11;
    s11 << ui->e_11->text().toStdString();
    s11 >> d;
    _data(1,1) = d;


    stringstream s12;
    s12 << ui->e_12->text().toStdString();
    s12 >> d;
    _data(1,2) = d;


    stringstream s13;
    s13 << ui->e_13->text().toStdString();
    s13 >> d;
    _data(1,3) = d;



    stringstream s20;
    s20 << ui->e_20->text().toStdString();
    s20 >> d;
    _data(2,0) = d;


    stringstream s21;
    s21 << ui->e_21->text().toStdString();
    s21 >> d;
    _data(2,1) = d;


    stringstream s22;
    s22 << ui->e_22->text().toStdString();
    s22 >> d;
    _data(2,2) = d;


    stringstream s23;
    s23 << ui->e_23->text().toStdString();
    s23 >> d;
    _data(2,3) = d;



    stringstream s30;
    s30 << ui->e_30->text().toStdString();
    s30 >> d;
    _data(3,0) = d;


    stringstream s31;
    s31 << ui->e_31->text().toStdString();
    s31 >> d;
    _data(3,1) = d;


    stringstream s32;
    s32 << ui->e_32->text().toStdString();
    s32 >> d;
    _data(3,2) = d;


    stringstream s33;
    s33 << ui->e_33->text().toStdString();
    s33 >> d;
    _data(3,3) = d;


    QDialog::accept();
}

cagd::Matrix<cagd::DCoordinate3> ExtendDialog::getData()
{
    return _data;
}

ExtendDialog::~ExtendDialog()
{
    delete ui;
}

void ExtendDialog::setup_tl(bool l, bool t)
{
    ui->e_11->setEnabled(false);
    ui->e_13->setEnabled(false);
    ui->e_31->setEnabled(false);
    ui->e_33->setEnabled(false);

    ui->e_00->setText("-3 3 0");



    if (t)
    {
        ui->e_01->setEnabled(false);
        ui->e_03->setEnabled(false);
        ui->e_21->setEnabled(false);
        ui->e_23->setEnabled(false);
    }
    else
    {
        ui->e_01->setText("-1 3 0");
    }

    if (l)
    {
        ui->e_10->setEnabled(false);
        ui->e_12->setEnabled(false);
        ui->e_30->setEnabled(false);
        ui->e_32->setEnabled(false);
    }
    else
    {
        ui->e_10->setText("-3 1 0");
    }
}

void ExtendDialog::setup_t(bool tl, bool tr)
{
    ui->e_10->setEnabled(false);
    ui->e_11->setEnabled(false);
    ui->e_12->setEnabled(false);
    ui->e_13->setEnabled(false);

    ui->e_30->setEnabled(false);
    ui->e_31->setEnabled(false);
    ui->e_32->setEnabled(false);
    ui->e_33->setEnabled(false);

    if (tl)
    {
        ui->e_00->setEnabled(false);
        ui->e_02->setEnabled(false);
        ui->e_20->setEnabled(false);
        ui->e_22->setEnabled(false);
    }
    else
    {
        ui->e_00->setText("-1 3 0");
    }

    if (tr)
    {
        ui->e_01->setEnabled(false);
        ui->e_03->setEnabled(false);
        ui->e_21->setEnabled(false);
        ui->e_23->setEnabled(false);
    }
    else
    {
        ui->e_01->setText("1 3 0");
    }
}

void ExtendDialog::setup_tr(bool t, bool r)
{
    ui->e_10->setEnabled(false);
    ui->e_12->setEnabled(false);
    ui->e_30->setEnabled(false);
    ui->e_32->setEnabled(false);

    ui->e_01->setText("3 3 0");

    if (t)
    {
        ui->e_00->setEnabled(false);
        ui->e_02->setEnabled(false);
        ui->e_20->setEnabled(false);
        ui->e_22->setEnabled(false);
    }
    else
    {
        ui->e_00->setText("1 3 0");
    }

    if (r)
    {
        ui->e_11->setEnabled(false);
        ui->e_13->setEnabled(false);
        ui->e_31->setEnabled(false);
        ui->e_33->setEnabled(false);
    }
    else
    {
        ui->e_11->setText("3 1 0");
    }
}

void ExtendDialog::setup_r(bool tr, bool br)
{
    ui->e_00->setEnabled(false);
    ui->e_02->setEnabled(false);
    ui->e_20->setEnabled(false);
    ui->e_22->setEnabled(false);

    ui->e_10->setEnabled(false);
    ui->e_12->setEnabled(false);
    ui->e_30->setEnabled(false);
    ui->e_32->setEnabled(false);

    if (tr)
    {
        ui->e_01->setEnabled(false);
        ui->e_03->setEnabled(false);
        ui->e_21->setEnabled(false);
        ui->e_23->setEnabled(false);
    }
    else
    {
        ui->e_01->setText("3 1 0");
    }

    if (br)
    {
        ui->e_11->setEnabled(false);
        ui->e_13->setEnabled(false);
        ui->e_31->setEnabled(false);
        ui->e_33->setEnabled(false);
    }
    else
    {
        ui->e_11->setText("3 -1 0");
    }
}

void ExtendDialog::setup_br(bool r, bool b)
{
    ui->e_00->setEnabled(false);
    ui->e_02->setEnabled(false);
    ui->e_20->setEnabled(false);
    ui->e_22->setEnabled(false);

    if (r)
    {
        ui->e_01->setEnabled(false);
        ui->e_03->setEnabled(false);
        ui->e_21->setEnabled(false);
        ui->e_23->setEnabled(false);
    }
    else
    {
        ui->e_01->setText("3 -1 0");
    }

    if (b)
    {
        ui->e_10->setEnabled(false);
        ui->e_12->setEnabled(false);
        ui->e_30->setEnabled(false);
        ui->e_32->setEnabled(false);
    }
    else
    {
        ui->e_10->setText("1 -3 0");
    }

    ui->e_11->setText("3 -3 0");
}


void ExtendDialog::setup_b(bool br, bool bl)
{
    ui->e_00->setEnabled(false);
    ui->e_02->setEnabled(false);
    ui->e_20->setEnabled(false);
    ui->e_22->setEnabled(false);

    ui->e_01->setEnabled(false);
    ui->e_03->setEnabled(false);
    ui->e_21->setEnabled(false);
    ui->e_23->setEnabled(false);

    if (br)
    {
        ui->e_11->setEnabled(false);
        ui->e_13->setEnabled(false);
        ui->e_31->setEnabled(false);
        ui->e_33->setEnabled(false);
    }
    else
    {
        ui->e_11->setText("1 -3 0");
    }

    if (bl)
    {
        ui->e_10->setEnabled(false);
        ui->e_12->setEnabled(false);
        ui->e_30->setEnabled(false);
        ui->e_32->setEnabled(false);
    }
    else
    {
        ui->e_10->setText("-1 -3 0");
    }
}

void ExtendDialog::setup_bl(bool b, bool l)
{
    ui->e_10->setEnabled(false);
    ui->e_12->setEnabled(false);
    ui->e_30->setEnabled(false);
    ui->e_32->setEnabled(false);

    if (b)
    {
        ui->e_10->setEnabled(false);
        ui->e_12->setEnabled(false);
        ui->e_30->setEnabled(false);
        ui->e_32->setEnabled(false);
    }
    else
    {
        ui->e_11->setText("-1 -3 0");
    }

    if (l)
    {
        ui->e_10->setEnabled(false);
        ui->e_12->setEnabled(false);
        ui->e_30->setEnabled(false);
        ui->e_32->setEnabled(false);
    }
    else
    {
        ui->e_00->setText("-3 -1 0");
    }

    ui->e_10->setText("-3 -3 0");
}

void ExtendDialog::setup_l(bool bl, bool tl)
{
    ui->e_01->setEnabled(false);
    ui->e_03->setEnabled(false);
    ui->e_21->setEnabled(false);
    ui->e_23->setEnabled(false);

    ui->e_11->setEnabled(false);
    ui->e_13->setEnabled(false);
    ui->e_31->setEnabled(false);
    ui->e_33->setEnabled(false);

    if (bl)
    {
        ui->e_10->setEnabled(false);
        ui->e_12->setEnabled(false);
        ui->e_30->setEnabled(false);
        ui->e_32->setEnabled(false);
    }
    else
    {
        ui->e_10->setText("-3 -1 0");
    }

    if (tl)
    {
        ui->e_00->setEnabled(false);
        ui->e_02->setEnabled(false);
        ui->e_20->setEnabled(false);
        ui->e_22->setEnabled(false);
    }
    else
    {
        ui->e_00->setText("-3 1 0");
    }
}
