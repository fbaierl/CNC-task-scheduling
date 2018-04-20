#include "checkscheduleform.h"
#include "ui_checkscheduleform.h"

CheckScheduleForm::CheckScheduleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckScheduleForm)
{
    ui->setupUi(this);

    connect(ui->pushButtonSolve, SIGNAL(released()), this, SLOT(slot_solvePressed()));
}

CheckScheduleForm::~CheckScheduleForm()
{
    delete ui;
}

void CheckScheduleForm::slot_solvePressed()
{
    Schedule schedule;
    schedule.setProcessorCount(ui->spinBoxProcessorCount->value());

    QString text = ui->lineEditInput->text();
    text.remove(0, 1);
    text.remove(text.size() - 1, 1);
    QStringList list = text.split(")(");

    for(int i = 0; i < list.size(); ++i)
    {
        QString task = list.at(i);
        QStringList taskList = task.split(",");
        schedule.append(Task(taskList.at(0).toInt(), taskList.at(1).toInt(), taskList.at(2).toInt()));
    }


    CNCSolver *solverC = new CNCSolver();
    solverC->setSchedule(schedule);
    solverC->setVariant(CNCSolver::CONTIGUOUS);

    Tree *treeC = new Tree();
    treeC->setUpperBound(250);
    SolverResult resC = treeC->solve(schedule, solverC);

    solverC->solve(resC.getOptimalSequence());

    QString planTextC = solverC->getPlanString();
    ui->plainTextEditContiguous->setPlainText(planTextC);

    QString optSeqC;
    for(int i = 0; i < resC.getOptimalSequence().size(); ++i)
    {
        optSeqC.append(QString::number(resC.getOptimalSequence().at(i)) + " ");
    }
    ui->lineEditOptimalSequenceC->setText(optSeqC);

    ui->lineEditSolverResultC->setText(QString::number(resC.getSolverResult()));

    delete solverC;
    delete treeC;


    CNCSolver *solverNC = new CNCSolver();
    solverNC->setSchedule(schedule);
    solverNC->setVariant(CNCSolver::NONCONTIGUOUS);

    Tree *treeNC = new Tree();
    treeNC->setUpperBound(250);
    SolverResult resNC = treeNC->solve(schedule, solverNC);

    solverNC->solve(resNC.getOptimalSequence());

    QString planTextNC = solverNC->getPlanString();
    ui->plainTextEditNonContiguous->setPlainText(planTextNC);

    QString optSeqNC;
    for(int i = 0; i < resNC.getOptimalSequence().size(); ++i)
    {
        optSeqNC.append(QString::number(resNC.getOptimalSequence().at(i)) + " ");
    }
    ui->lineEditOptimalSequenceNC->setText(optSeqNC);

    ui->lineEditSolverResultNC->setText(QString::number(resNC.getSolverResult()));



    delete solverNC;
    delete treeNC;
}

