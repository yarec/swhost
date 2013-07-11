//#include "log.h"
#include "hosteditor.h"
#include "ui_hosteditor.h"
#include <map>
HostEditor::HostEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HostEditor)
{
    ui->setupUi(this);
    init();

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    QPoint pos = frect.topLeft();
    pos.setY(160);
    move(pos);

}

HostEditor::~HostEditor()
{
    delete ui;
}

void HostEditor::init()
{
    initHostTable();
    initSwHostBtn();

    //list hostbak files
    QDir d(QCoreApplication::applicationDirPath ()+"/"BAKDIR);
    if(d.exists()){
        d.setFilter(QDir::Files | QDir::Hidden);
        d.setSorting(QDir::Time);
        ui->bakHostListWidget->addItems(d.entryList());
        ui->bakHostListWidget->sortItems(Qt::AscendingOrder);
    }

    loadTextFile();

}
void HostEditor::initHostTable(){

    read();

    map <string, string> mapIp = getMapIp();
    map <string, string> mapCIp = getMapCIp();

    for(int i=0; i<3; i++){
        ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
    }

    mapExistIps.clear();

    QMap<string, string> domainGroup;
    domainGroup = getDomainGroupMap(domainGroup, mapIp, false);
    domainGroup = getDomainGroupMap(domainGroup, mapCIp, true);

    QMap <string, string>::iterator qit;
    for ( qit=domainGroup.begin(); qit != domainGroup.end(); qit++ ){
        QStringList host_list = QString(qit.value().c_str()).split("&");
        foreach(QString host, host_list){
            buildIpUi(host.split("=")[0].toStdString(), host.split("=")[1].toStdString());
        }
    }

    ui->verticalLayoutWidget->adjustSize();
    ui->scrollAreaWidgetContents->setFixedSize(ui->verticalLayoutWidget->size()+QSize(1,1));
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();

}

QMap<string, string> HostEditor::getDomainGroupMap(QMap<string, string> dg, map<string, string> mapIp, bool comment)
{
    QMap<string, string> domainGroup = dg;

    map <string, string>::iterator it;
    for ( it=mapIp.begin(); it != mapIp.end(); it++ ){
        string ip = (*it).second;
        string domain = (*it).first;

        string domain_1st = get1stDomain(domain.c_str()).toStdString();
        string domain_list;
        if(domainGroup.contains(domain_1st)){
            domain_list = domainGroup.value(domain_1st);
        }

        if(!domain_list.empty()){
            domain_list += "&";
        }
        domain_list += domain+"="+ip;

        if(domain_list.substr(1) != "#" && comment){
            domain_list = ""+ domain_list;
        }

        domainGroup[domain_1st] = domain_list;
    }

    return domainGroup;
}

QString HostEditor::get1stDomain(QString domain)
{

    log(domain);

    QString ret;
    int dot_count = domain.count(".");
    for(int i=1; i<dot_count; i++){
        ret = domain.right(domain.count()-domain.indexOf(".")-1);
    }
    return ret;
}

void HostEditor::buildIpUi(string domain, string ip)
{

    QMap<string, string> cip(getMapCIp());

    int rowcount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowcount);

    QTableWidgetItem *d = new QTableWidgetItem(domain.c_str());
    ui->tableWidget->setItem(rowcount, 0, d);

    QTableWidgetItem *i= new QTableWidgetItem(ip.c_str());
    ui->tableWidget->setItem(rowcount, 1, i);


    QCheckBox *pBox =new QCheckBox();
    pBox->setObjectName("validIp");
    if(!cip.contains(domain)){
        pBox->setCheckState(Qt::Checked);
    }

    QTableWidgetItem *checkBox = new QTableWidgetItem();
    if(!cip.contains(domain)){
        checkBox->setCheckState(Qt::Checked);
    }
    else{
        checkBox->setCheckState(Qt::Unchecked);
    }

    ui->tableWidget->setItem(rowcount,2, checkBox);

    buildRadioBtn(ip);

}

void HostEditor::buildRadioBtn(string ip)
{
    if(! mapExistIps.contains(ip)){

        QRadioButton *radioButton;
        radioButton = new QRadioButton(ui->verticalLayoutWidget);

        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setText(QApplication::translate("HostEditor", ip.c_str(), 0, QApplication::UnicodeUTF8));
        ui->radioLayout->addWidget(radioButton);
        radioButton->show();

        QObject::connect(radioButton, SIGNAL(clicked()), this, SLOT(my_on_radioButton_clicked()));

        mapExistIps[ip] = ip;
    }
}

void HostEditor::initSwHostBtn()
{
    QDir d(QCoreApplication::applicationDirPath ()+"/"HOSTDIR);
    if(d.exists()){
        d.setFilter(QDir::Files | QDir::Hidden);
        d.setSorting(QDir::Time |QDir::Reversed );

        QStringList sl = d.entryList();
        foreach(QString host, sl){
            QPushButton *swhostbtn = new QPushButton(ui->horizontalLayoutWidget);
            swhostbtn->setObjectName(QString::fromUtf8(host.toStdString().c_str()));
            swhostbtn->setText(QApplication::translate("HostEditor", host.toStdString().c_str(), 0, QApplication::UnicodeUTF8));
            ui->btnLayout->addWidget(swhostbtn);
            swhostbtn->show();
            QObject::connect(swhostbtn, SIGNAL(clicked()), this, SLOT(my_on_swhostbtn_clicked()));

            ui->comboBox->addItem(host);
        }
        ui->horizontalLayoutWidget->adjustSize();
        ui->scrollAreaWidgetContents_2->setFixedSize(ui->horizontalLayoutWidget->size()+QSize(1,1));
    }
}

void HostEditor::setIp(QString ip){
    QList<QTableWidgetItem *> selectedItems  = ui->tableWidget->selectedItems ();

    QTableWidgetItem *item;
    foreach (item, selectedItems) {
        ui->tableWidget->item(item->row(), 1)->setText(ip);
    }
}

void HostEditor::loadTextFile()
{
    QFile inputFile(HOSTS);
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();

    ui->textEdit->setPlainText(line);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void HostEditor::bak(){

    QDir d(QCoreApplication::applicationDirPath ()+"/"BAKDIR);
    if(!d.exists()){
        QString appdirpath = QCoreApplication::applicationDirPath ()+"/"BAKDIR;
        d.mkdir(appdirpath);
    }

    QString bakfile = QDateTime::currentDateTime().toString("'hosts-'yyyy-MM-dd_hhmmss");
    cp(HOSTS, (QCoreApplication::applicationDirPath ()+"/"BAKDIR"/"+bakfile).toStdString());

    ui->bakHostListWidget->addItem(bakfile);

}

void HostEditor::save(string savefile){
    int rowcount = ui->tableWidget->rowCount();
    ofstream host(savefile.c_str());
    for(int i=0; i<rowcount; i++){

        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        if(item!=NULL){
            QString domain = item->text();
            QString ip = ui->tableWidget->item(i, 1)->text();
            if(ui->tableWidget->item(i, 2)->checkState()==Qt::Unchecked){
                host << "#";
            }

            host << ip.toStdString() << " " << domain.toStdString() <<endl;
        }
    }

    host.close();

    log("host save ok");
}

void HostEditor::cp(string src, string dest){

    ofstream hostbak(dest.c_str());

    ifstream fin(src.c_str());
    if(!fin) {
        cout<<"can not open the input file";
        return ;
    }

    string s;
    while(getline(fin,s)) {
        hostbak << s << endl;
    }
    hostbak.close();

    log("cp ok");
}
void HostEditor::enableHost(bool enable)
{
    QList<QTableWidgetItem *> selectedItems  = ui->tableWidget->selectedItems ();
    QTableWidgetItem *item;
    foreach (item, selectedItems) {
        QTableWidgetItem * checkbox = ui->tableWidget->item(item->row(), 2);
        if(enable){
            checkbox->setCheckState(Qt::Checked);
        }
        else{
            checkbox->setCheckState(Qt::Unchecked);
        }
    }
}

void HostEditor::refreshSwHostBtn(){
    while(ui->comboBox->count() > 0){
        ui->comboBox->removeItem(0);
    }
    while(ui->btnLayout->count() > 0){
        QWidget* w = ui->btnLayout->itemAt(0)->widget();
        ui->btnLayout->removeWidget(w);
        delete w;
    }
    initSwHostBtn();
}

void HostEditor::my_on_radioButton_clicked()
{
    QRadioButton *radioButton =  (QRadioButton *)QObject::sender();
    QString ip = radioButton->text();
    setIp(ip);
}

void HostEditor::my_on_swhostbtn_clicked()
{
    QPushButton *swhostbtn=  (QPushButton *)QObject::sender();
    QString swhost = swhostbtn->text();
    cp( (QCoreApplication::applicationDirPath ()+"/"HOSTDIR"/"+swhost).toStdString(), HOSTS);
    on_reload_clicked();
}

void HostEditor::on_save2host_clicked()
{
    bak();
    save(HOSTS);
    loadTextFile();
}

void HostEditor::on_restorebakhost_clicked()
{
    QListWidgetItem *item = ui->bakHostListWidget->currentItem();
    if(item !=NULL){
        QString bakhostfile = item->text();
        cp( (QCoreApplication::applicationDirPath ()+"/"BAKDIR"/"+bakhostfile).toStdString(), HOSTS);
        log("cp2"HOSTS" ok!");

        on_reload_clicked();
        loadTextFile();
    }
    else{
        log("bak file not selected!");
    }
}

void HostEditor::on_delbakhost_clicked()
{
    QListWidgetItem *item = ui->bakHostListWidget->currentItem();
    if(item !=NULL){
        QString bakhostfile = item->text();
        QFile f(QCoreApplication::applicationDirPath ()+"/"BAKDIR"/"+bakhostfile);
        f.remove();

        ui->bakHostListWidget->removeItemWidget(item);
        delete item;
    }
    else{
        log("bak file not selected!");
    }
}

void HostEditor::on_lineEdit_textChanged(const QString &arg1)
{
    int rowcount = ui->tableWidget->rowCount();
    for(int i=0; i<rowcount; i++){
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        if(item!=NULL){
            QString domain = item->text();

            if(!domain.contains(ui->lineEdit->text())){
                ui->tableWidget->setRowHidden(i,true);
            }
            else{
                ui->tableWidget->setRowHidden(i,false);
            }
        }
    }
}

void HostEditor::on_saveAs_clicked()
{
    bool isOK;
    QString name= QInputDialog::getText(this, "Input Dialog",
                                         "Please input name to save",
                                         QLineEdit::Normal, "", &isOK);
    if(isOK && !name.trimmed().isEmpty()){
        QDir d(QCoreApplication::applicationDirPath ()+"/"HOSTDIR);
        d.setFilter(QDir::Files | QDir::Hidden);
        if(!d.exists()){
            QString hostdir = QCoreApplication::applicationDirPath ()+"/"HOSTDIR;
            d.mkdir(hostdir);
        }

        if(d.count()>=5){
            log("Count not save swhost more then 5");
            QMessageBox::warning(this, tr("Save SwHost"),
                                            tr("count not save swhost more then 5!" ),
                                            QMessageBox::Ok ,
                                            QMessageBox::Ok);
        }
        else{
            save( (QCoreApplication::applicationDirPath ()+"/"HOSTDIR"/"+name).toStdString());
            refreshSwHostBtn();
        }
    }

}

void HostEditor::on_delswhost_clicked()
{
    const QString text = ui->comboBox->currentText();
    QFile f(QCoreApplication::applicationDirPath ()+"/"HOSTDIR"/"+text);
    f.remove();

    refreshSwHostBtn();
}

void HostEditor::on_enable_clicked()
{
    enableHost(true);
}

void HostEditor::on_disable_clicked()
{
    enableHost(false);
}

void HostEditor::on_add_clicked()
{
    int rowcount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowcount);

    QTableWidgetItem *checkBox = new QTableWidgetItem();
    checkBox->setCheckState(Qt::Checked);
    ui->tableWidget->setItem(rowcount,2, checkBox);

    ui->tableWidget->scrollToBottom();
}

void HostEditor::on_tableWidget_cellChanged(int row, int column)
{
    if(column==1){
        QTableWidgetItem *item = ui->tableWidget->currentItem();
        if(item!=NULL && item->column()==1){
            QString ip = item->text();
            if(!ip.trimmed().isEmpty()){
                buildRadioBtn(ip.toStdString());
            }
        }
    }
}

void HostEditor::on_addip_clicked()
{

    bool isOK;
    QString ip= QInputDialog::getText(this, "Input Dialog",
                                         "Please input ip to add",
                                         QLineEdit::Normal, "", &isOK);
    if(isOK && !ip.trimmed().isEmpty()){
        buildRadioBtn(ip.toStdString());
        ui->verticalLayoutWidget->adjustSize();
        ui->scrollAreaWidgetContents->setFixedSize(ui->verticalLayoutWidget->size()+QSize(1,1));
    }
}

void HostEditor::on_reload_clicked()
{
    ui->tableWidget->removeColumn(2);
    ui->tableWidget->removeColumn(1);
    ui->tableWidget->removeColumn(0);

    int rowcount = ui->tableWidget->rowCount();
    for(int i=0; i<rowcount; i++){
        ui->tableWidget->removeRow(rowcount-i-1);
    }

    while(ui->radioLayout->count() > 0){
        QWidget* w = ui->radioLayout->itemAt(0)->widget();
        ui->radioLayout->removeWidget(w);
        delete w;
    }

    initHostTable();
}

void HostEditor::on_reloadtxt_clicked()
{
    loadTextFile();
}

void HostEditor::on_savetxt_clicked()
{
    bak();

    QString c = ui->textEdit->toPlainText();
    ofstream host(HOSTS);
    host << c.toStdString();
    host.close();

    on_reload_clicked();

    log("save txt ok");
}

void HostEditor::on_delhost_clicked()
{
    QList<QTableWidgetItem *> selectedItems  = ui->tableWidget->selectedItems ();
    log(selectedItems.size());
    QTableWidgetItem *item;
    foreach (item, selectedItems) {
        ui->tableWidget->removeRow(item->row());
    }
}
