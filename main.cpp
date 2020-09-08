#include <QCoreApplication>
#include <QCryptographicHash>
#include <QProcess>
#include <QFile>

QString getKey(QByteArray arr)
{
    QString str=arr.data();
    QStringList tmp=str.simplified().split(" ");
    if(tmp.count()==2){
        return tmp[1];
    }
    return "E";
}

QByteArray runPro(QString cmd,QStringList arg)
{
    QProcess process;
    process.setProgram(cmd);
    process.setArguments(arg);
    process.start();

    process.closeWriteChannel();
    if(process.waitForFinished()){
        return  process.readAll();
    }

    process.close();
    return "";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QByteArray key;

    key.append(getKey(runPro("wmic",QStringList()<<"CPU"<<"get"<<"ProcessorID")));/* CPU */
    //key.append(getKey(runPro("wmic",QStringList()<<"diskdrive"<<"get"<<"serialnumber")));/* 硬盘 */
    key.append(getKey(runPro("wmic",QStringList()<<"baseboard"<<"get"<<"serialnumber")));/* 主板 */
    key.append(getKey(runPro("wmic",QStringList()<<"csproduct"<<"get"<<"UUID")));/* UUID */


    if(key!="EE"){
        QFile file("key");
        file.open(QIODevice::WriteOnly);
        file.write(QCryptographicHash::hash(key.insert(3,"Cheng870888"),QCryptographicHash::Md5).toHex());
        file.close();
    }

    return a.exec();
}

