#ifndef CHECKSCHEDULEFORM_H
#define CHECKSCHEDULEFORM_H

#include <QWidget>
#include <QString>

#include <iostream>

#include "solverworker.h"
#include "tree.h"

namespace Ui {
class CheckScheduleForm;
}

class CheckScheduleForm : public QWidget
{
    Q_OBJECT

public:
    explicit CheckScheduleForm(QWidget *parent = 0);
    ~CheckScheduleForm();

private:
    Ui::CheckScheduleForm *ui;

private slots:
    void slot_solvePressed();

private:

};

#endif // CHECKSCHEDULEFORM_H
