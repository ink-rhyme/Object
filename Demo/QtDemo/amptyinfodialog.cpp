#include "amptyinfodialog.h"
#include "ui_amptyinfodialog.h"

AmptyInfoDialog::AmptyInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AmptyInfoDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags()
           & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowMinMaxButtonsHint);
    this->setWindowTitle("文章内容");
}

AmptyInfoDialog::~AmptyInfoDialog()
{
    delete ui;
}

void AmptyInfoDialog::setTextTitle(const QString& title)
{
    ui->textTitle->setText(title);
}

void AmptyInfoDialog::setTextBrowser(const QString& browser)
{

    ui->textBrowser->setText("    " + browser);
}
