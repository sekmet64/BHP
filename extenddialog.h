#ifndef EXTENDDIALOG_H
#define EXTENDDIALOG_H

#include <QDialog>
#include "Core/DCoordinates3.h"
#include "Core/Matrices.h"

namespace Ui {
    class ExtendDialog;
}

class ExtendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtendDialog(
        cagd::DCoordinate3 d_00, cagd::DCoordinate3 d_01,
        cagd::DCoordinate3 d_02, cagd::DCoordinate3 d_03,

        cagd::DCoordinate3 d_10, cagd::DCoordinate3 d_11,
        cagd::DCoordinate3 d_12, cagd::DCoordinate3 d_13,

        cagd::DCoordinate3 d_20, cagd::DCoordinate3 d_21,
        cagd::DCoordinate3 d_22, cagd::DCoordinate3 d_23,

        cagd::DCoordinate3 d_30, cagd::DCoordinate3 d_31,
        cagd::DCoordinate3 d_32, cagd::DCoordinate3 d_33,

        QWidget *parent = 0);

    ~ExtendDialog();

    cagd::Matrix<cagd::DCoordinate3> getData();
    void setup_tl(bool l, bool t);
    void setup_t(bool tl, bool tr);
    void setup_tr(bool t, bool r);
    void setup_r(bool tr, bool br);

    void setup_br(bool r, bool b);
    void setup_b(bool br, bool bl);
    void setup_bl(bool b, bool l);
    void setup_l(bool bl, bool tl);
public slots:
    void accept();
private:
    Ui::ExtendDialog *ui;
    cagd::Matrix<cagd::DCoordinate3> _data;
};

#endif // EXTENDDIALOG_H
