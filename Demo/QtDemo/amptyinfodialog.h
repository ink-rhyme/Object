#ifndef AMPTYINFODIALOG_H
#define AMPTYINFODIALOG_H

#include <QDialog>

namespace Ui {
class AmptyInfoDialog;
}

class AmptyInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AmptyInfoDialog(QWidget *parent = 0);
    ~AmptyInfoDialog();

public:
    void setTextTitle(const QString&);
    void setTextBrowser(const QString&);

private:
    Ui::AmptyInfoDialog *ui;
};

#endif // AMPTYINFODIALOG_H
