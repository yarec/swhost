#ifndef HOSTEDITOR_H
#define HOSTEDITOR_H

#include "log.h"
#include "parsehost.h"
#include <QDir>
#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>
#include <QDesktopWidget>

#include <QInputDialog>
#include <QtGui/QRadioButton>
#include <QModelIndex>
#include <QtGui/QPushButton>

namespace Ui {
class HostEditor;
}

class HostEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit HostEditor(QWidget *parent = 0);
    void init();
    ~HostEditor();
    
    void setIp(QString ip);
    void bak();
    void save(string savefile);
    void cp(string src, string dest);
    void initHostTable();
    QString get1stDomain(QString domain);
    void initSwHostBtn();
    void refreshSwHostBtn();
    void buildIpUi(string domain, string ip);
    void enableHost(bool enable);
    void buildRadioBtn(string ip);
    void loadTextFile();
    QMap<string, string> getDomainGroupMap(QMap<string, string> dg, map<string, string> mapIp, bool comment);
private slots:
    void my_on_radioButton_clicked();

    void my_on_swhostbtn_clicked();

    void on_save2host_clicked();

    void on_restorebakhost_clicked();

    void on_delbakhost_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_saveAs_clicked();

    void on_delswhost_clicked();

    void on_enable_clicked();

    void on_disable_clicked();

    void on_add_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_reload_clicked();

    void on_addip_clicked();

    void on_reloadtxt_clicked();

    void on_savetxt_clicked();

    void on_delhost_clicked();

private:
    Ui::HostEditor *ui;

    QMap <string, string> mapExistIps;
};

#endif // HOSTEDITOR_H
