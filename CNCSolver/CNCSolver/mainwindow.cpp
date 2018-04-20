#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_checkScheduleForm(new CheckScheduleForm()),
    m_initialScheduleSize(0)
{
    ui->setupUi(this);

    connect(ui->pushButtonSolve, SIGNAL(released()), this, SLOT(slot_solvePressed()));
    connect(ui->pushButtonChooseFilename, SIGNAL(released()), this, SLOT(slot_chooseFilenamePressed()));
    connect(ui->lineEditFilename, SIGNAL(textChanged(QString)), this, SLOT(slot_filenameChanged(QString)));
    connect(ui->pushButtonCheckSchedule, SIGNAL(released()), this, SLOT(slot_checkSchedulePressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_checkScheduleForm;

    for(int i = 0; i < m_worker.size(); ++i)
    {
        SolverWorker *w = m_worker.value(i);

        if(!w->wait(5000)) //Wait until it actually has terminated (max. 5 sec)
        {
            w->terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
            w->wait(); //Note: We have to wait again here!
        }

        delete w;
    }

}

void MainWindow::slot_solvePressed()
{
    if(m_filename.size() == 0)
    {
        ui->lineEdit->setText("No filename specified!");
        return;
    }
    else
        ui->lineEdit->setText("");


    QFile file(m_filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << "Is Contiguous;Schedule (Tasknumber, processing time, size);Optimal Tasksequece;Solver result" << endl;
    }

    if(!generateSchedules())
        return;

    if(m_schedules.isEmpty())
        return;

    ui->progressBar->setValue(0);
    m_finishedCounter = 0;

    for(int i = 0; i < qMin(m_schedules.size(), 4); ++i)
    {
        Schedule schedule = m_schedules.takeFirst();
        SolverWorker *sw = new SolverWorker(i);
        sw->setup(schedule);
        connect(sw, SIGNAL(signal_solverResultReady(QVector<SolverResult>, int)), this, SLOT(slot_solved(QVector<SolverResult>, int)));
        m_worker.append(sw);

        sw->start(QThread::HighPriority);
    }

    ui->pushButtonSolve->setText("Solving...");
    ui->pushButtonSolve->setEnabled(false);
    ui->lineEditFilename->setReadOnly(true);

}

void MainWindow::slot_solved(QVector<SolverResult> result, int index)
{
    ++m_finishedCounter;

    static int cncCounter = 0;
    if(result.at(0).getSolverResult() != result.at(1).getSolverResult())
        ++cncCounter;

    ui->lineEdit->setText("Found " + QString::number(cncCounter) + " / " + QString::number(m_finishedCounter) + " c/nc difference");


    if(m_initialScheduleSize - m_finishedCounter == 0)
    {
        ui->pushButtonSolve->setText("Solve");
        ui->pushButtonSolve->setEnabled(true);
        ui->lineEditFilename->setReadOnly(false);
        cncCounter = 0; //Reset
    }
    else
    {
        if(m_schedules.size() != 0)
        {
            SolverWorker *w = m_worker.value(index);

            if(!w->wait(3000)) //Wait until it actually has terminated (max. 3 sec)
            {
                w->terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
                w->wait(3000); //Note: We have to wait again here!
            }

            Schedule schedule = m_schedules.takeFirst();
            w->setup(schedule);
            w->start(QThread::HighPriority);
        }
    }

    ui->progressBar->setValue(((double) m_finishedCounter / (double) m_initialScheduleSize) * 100.0);

    writeToFile(result.at(0));
    writeToFile(result.at(1));
}

void MainWindow::slot_chooseFilenamePressed()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ui->lineEditFilename->setText(dir + "/cnc_results.csv");
}

void MainWindow::slot_filenameChanged(QString newFilename)
{
    m_filename = newFilename;
}

void MainWindow::slot_checkSchedulePressed()
{
    m_checkScheduleForm->show();
}

bool MainWindow::generateSchedules()
{
    srand(time(NULL));  //Seed random

    int overallCount = ui->spinBoxOverallCount->value();
    int taskCount = ui->spinBoxTaskCount->value();
    int processorCount = ui->spinBoxProcessorCount->value();

    int processingTimeLower = ui->spinBoxProcessingTimeLower->value();
    int processingTimeUpper = ui->spinBoxProcessingTimeUpper->value();

    int taskSizeLower = ui->spinBoxTaskSizeLower->value();
    int taskSizeUpper = ui->spinBoxTaskSizeUpper->value();


    //check spinboxes
    if(processingTimeLower > processingTimeUpper)
    {
        ui->lineEdit->setText("Check processing times!");
        return false;
    }

    if(taskSizeLower > taskSizeUpper)
    {
        ui->lineEdit->setText("Check task sizes!");
        return false;
    }

    if(taskSizeUpper > processorCount)
    {
        ui->lineEdit->setText("Check upper task sizes and processor count!");
        return false;
    }

    ui->lineEdit->setText("");


    for(int c = 0; c < overallCount; ++c)
    {
        Schedule schedule;
        for(int i = 0; i < taskCount; ++i)
        {
            schedule.append(Task(i, getRandom(processingTimeLower, processingTimeUpper),
                                 getRandom(taskSizeLower, taskSizeUpper)));
        }
        schedule.setProcessorCount(processorCount);

        m_schedules.append(schedule);
    }

    m_initialScheduleSize = m_schedules.size();

    std::cout << "Generated: " << m_initialScheduleSize << std::endl;

    return true;
}

int MainWindow::getRandom(int lower, int upper)
{
    return (rand() % ((upper + 1) - lower)) + lower;
}

void MainWindow::writeToFile(SolverResult result)
{
    TaskSequence ts = result.getOptimalSequence();

    QString optimalTaskSequeceCSV;
    for(int i = 0; i < ts.size(); ++i)
    {
        optimalTaskSequeceCSV.append(QString::number(ts.at(i)));

        if(i < ts.size() - 1)
            optimalTaskSequeceCSV.append(",");
    }

    QFile file(m_filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << (result.isContiguous() ? "C" : "NC") << ";" << result.getSchedule().toCSVString() <<
                  ";" << optimalTaskSequeceCSV << ";" << QString::number(result.getSolverResult()) << endl;
    }
}
