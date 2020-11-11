#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class OpenDialog; }
QT_END_NAMESPACE

class OpenDialog : public QDialog
{
    Q_OBJECT

public:
    OpenDialog(QWidget *parent = nullptr);
    ~OpenDialog();

private:
    Ui::OpenDialog *ui;
};
#endif // OPENDIALOG_H
