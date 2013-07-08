#include "mAVMMonitor.h"
#include "ui_mAVMMonitor.h"
#include "mAMainWindow.h"

const float VMMONITOR_REFRESH_RATE = 20; // Hz

mAVMMonitor::mAVMMonitor(QWidget *parent, miniAudicle * _ma) :
    QMainWindow(parent),
    ui(new Ui::mAVMMonitor),
    ma(_ma)
{
    ui->setupUi(this);

    timerId = -1;
    vm_stall_count = 0;

    m_docid = ma->allocate_document_id();

    ui->removeLastButton->setEnabled(false);
    ui->removeAllButton->setEnabled(false);

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("shred"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("name"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("time"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("-"));
    ui->tableWidget->setColumnWidth(0, 48);
    ui->tableWidget->setColumnWidth(1, 156);
    ui->tableWidget->setColumnWidth(2, 48);
    ui->tableWidget->setColumnWidth(3, 24);
}

mAVMMonitor::~mAVMMonitor()
{
    ma->free_document_id(m_docid);

    delete ui;
}

void mAVMMonitor::vmChangedToState(bool vmOn)
{
    if(vmOn)
    {
        ui->toggleVMButton->setText("Stop Virtual Machine");

        timerId = startTimer((int)(1000.0/VMMONITOR_REFRESH_RATE));
    }
    else
    {
        ui->toggleVMButton->setText("Start Virtual Machine");

        if(timerId != -1)
        {
            killTimer(timerId);
            timerId = -1;
        }

        ui->runningTimeLabel->setText("");
        ui->shredCountLabel->setText("");

        ui->tableWidget->setRowCount(0);
    }

    ui->removeLastButton->setEnabled(vmOn);
    ui->removeAllButton->setEnabled(vmOn);
}

void mAVMMonitor::toggleVM()
{
    mAMainWindow * mainWindow = (mAMainWindow *) this->parent();
    mainWindow->toggleVM();
}

void mAVMMonitor::removeAll()
{
    mAMainWindow * mainWindow = (mAMainWindow *) this->parent();
    string result;
    ma->removeall(m_docid, result);
}

void mAVMMonitor::removeLast()
{
    mAMainWindow * mainWindow = (mAMainWindow *) this->parent();
    string result;
    ma->removelast(m_docid, result);
}

void mAVMMonitor::timerEvent(QTimerEvent *event)
{
    static time_t last_now = 0 - 1;
    static t_CKTIME last_now_system = 0;
    static int last_num_shreds = -1;

//    fprintf(stderr, "timer\n");

    ma->status( &status );
    QString temp;
    time_t age, now;
    int num_rows = ui->tableWidget->rowCount();
    int num_shreds = status.list.size();

    if( /*status.now_system > status.srate &&*/( status.now_system - last_now_system ) < 0.5 )
    {
        vm_stall_count++;

//        if( vm_stall_count >= vm_max_stalls && !wxGetApp().IsInLockdown() )
//        {
//            wxGetApp().SetLockdown( TRUE );
//        }
    }

    else
    {
//        if( wxGetApp().IsInLockdown() )
//            wxGetApp().SetLockdown( FALSE );
        vm_stall_count = 0;
    }

    last_now_system = status.now_system;

    ui->tableWidget->setRowCount(num_shreds);

    now = ( time_t ) ( status.now_system / status.srate );
//    if( now != last_now )
    {
        t_CKUINT samps = fmod( status.now_system, status.srate );
        ui->runningTimeLabel->setText(QString("%1:%2.%3")
                                      .arg((uint)(now/60))
                                      .arg((uint)(now%60), 2, 10, QLatin1Char('0'))
                                      .arg((uint)samps, 5, 10, QLatin1Char('0')));
        last_now = now;
    }

    if( num_shreds != last_num_shreds )
    {
        ui->shredCountLabel->setText(QString("%1").arg(num_shreds));
        last_num_shreds = num_shreds;
    }

    for( int i = 0; i < num_shreds; i++ )
    {
        Chuck_VM_Shred_Status * shred = status.list[i];

        ui->tableWidget->setRowHeight(i, 18);

        // set shred id column
        QString shredId = QString("%1").arg(shred->xid);
        if(ui->tableWidget->item(i, 0) == NULL)
        {
            QTableWidgetItem * item = new QTableWidgetItem(shredId);
            item->font().setPointSize(8);
            item->setFlags(Qt::ItemIsSelectable);
            ui->tableWidget->setItem(i, 0, item);
        }
        else
            ui->tableWidget->item(i, 0)->setText(shredId);

        // set shred name column
        QString name = QString("%1").arg(shred->name.c_str());
        if(ui->tableWidget->item(i, 1) == NULL)
        {
            QTableWidgetItem * item = new QTableWidgetItem(name);
            item->font().setPointSize(8);
            item->setFlags(Qt::ItemIsSelectable);
            ui->tableWidget->setItem(i, 1, item);
        }
        else
            ui->tableWidget->item(i, 1)->setText(name);

        // set shred time column
        age = ( time_t ) ( ( status.now_system - shred->start ) / status.srate );
        QString time = QString("%1:%2")
                .arg((uint)(age/60))
                .arg((uint)(age%60), 2, 10, QLatin1Char('0'));
        if(ui->tableWidget->item(i, 2) == NULL)
        {
            QTableWidgetItem * item = new QTableWidgetItem(time);
            item->font().setPointSize(8);
            item->setFlags(Qt::ItemIsSelectable);
            ui->tableWidget->setItem(i, 2, item);
        }
        else
            ui->tableWidget->item(i, 2)->setText(time);

//        grid->SetCellValue( i, 3, _T( "-" ) );

        // set remove button
        if(ui->tableWidget->cellWidget(i, 3) == NULL)
        {
            QPushButton * button = new QPushButton(ui->tableWidget);
            button->setText("-");
            button->resize(20, 16);
            button->setProperty("shred_id", QVariant((int)shred->xid));
            connect(button, SIGNAL(clicked()), SLOT(removeShred()));
            ui->tableWidget->setCellWidget(i, 3, button);
        }
        else
            ui->tableWidget->cellWidget(i, 3)->setProperty("shred_id", QVariant((int)shred->xid));
    }

//    grid->ClearSelection();
//    grid->Refresh();
}

void mAVMMonitor::removeShred()
{
    t_CKINT shred_id = this->sender()->property("shred_id").toInt();
    string result;
    ma->remove_shred(m_docid, shred_id, result);
}

