#include "videoconferencingserver.h"
#include <iostream>
#include <QJsonDocument>


using std::cout;
using std::endl;
using std::string;
using namespace boost::asio;
using namespace boost::posix_time;

void VideoConferencingServer::accept()
{
    sock_ptr sock(new socket_type(m_io));
    m_acceptor.async_accept(*sock,boost::bind(&VideoConferencingServer::handle_accepter,this, boost::asio::placeholders::error, sock));
}
void VideoConferencingServer::handle_accepter(const boost::system::error_code &ec, VideoConferencingServer::sock_ptr sock)
{
    if(ec)
        return;
    _remote_endpoint = sock->remote_endpoint(); //这个东西必须要放在这里
    cout  << "Client: "<< sock->remote_endpoint() << "已连接"<<endl;

    sock->async_receive(buffer(m_tcpRecvBuf), boost::bind(&VideoConferencingServer::tcpHandleReceive, this, boost::asio::placeholders::error, sock, sock->remote_endpoint().address().to_string()));
    accept();
}
void VideoConferencingServer::run()
{
        m_io.run();
}

void VideoConferencingServer::tcpHandleReceive(const boost::system::error_code &ec, VideoConferencingServer::sock_ptr sock, std::string _remote_ip)
{
    std::string receive_message;

    for(int i = 0;i != BUFFER_LENGTH; i++)
    {
        if (m_tcpRecvBuf[i] != '\0')
            receive_message.push_back(m_tcpRecvBuf[i]);
    }
    cout << "来自客户端的数据：" << receive_message << endl;

    QJsonObject Data = stringToQJsonObject(receive_message);
    string type = Data["TYPE"].toString().toStdString();

    if(type == "#REGISTER") //处理注册请求
        handleRegister(Data, sock);
    else if(type == "#LOGIN") //处理登录请求
        handleLogin(Data, _remote_ip,sock);
    else if(type == "#EXIT")
        handleExit(Data, sock);
    else if(type == "#REQUEST_ACCOUNT_DETAIL")
        handleAccountDetail(Data, sock);
    else if(type == "#REQUEST_COLLEAGUE_LIST")
        handleColleagueList(Data, sock);

    sock->async_receive(buffer(m_tcpRecvBuf), boost::bind(&VideoConferencingServer::tcpHandleReceive,this, boost::asio::placeholders::error,sock,_remote_ip));
}

void VideoConferencingServer::tcpSendMessage(std::string msg, VideoConferencingServer::sock_ptr sock)
{
    for(int i = 0;i != BUFFER_LENGTH; i++)
    {
        if(i < msg.size())
            m_tcpSendBuf[i] = msg[i];
        else m_tcpSendBuf[i] = '\0';
    }
    cout << msg << endl;
    async_write(*sock,boost::asio::buffer(m_tcpSendBuf), boost::bind(&VideoConferencingServer::handleTcpSend, this, boost::asio::placeholders::error,sock));
}
void VideoConferencingServer::handleTcpSend(const boost::system::error_code &ec, VideoConferencingServer::sock_ptr sock)
{

}


//处理函数
void VideoConferencingServer::handleRegister(QJsonObject Data, VideoConferencingServer::sock_ptr sock)
{

    string email = Data.value("DATA")["EMAIL"].toString().toStdString();

    string tcpJson, id;
    int res;
    dc.jsonStrCreateRegisteredID(tcpJson, email, id, res);

    string realName = Data.value("DATA")["REALNAME"].toString().toStdString();
    string passwd = Data.value("DATA")["PASSWD"].toString().toStdString();
    string avatar = Data.value("DATA")["AVATAR"].toString().toStdString();
    string company = Data.value("DATA")["COMPANY"].toString().toStdString();
    string department = Data.value("DATA")["DEPARTMENT"].toString().toStdString();
    string group = Data.value("DATA")["GROUP"].toString().toStdString();
    string phone = Data.value("DATA")["PHONE"].toString().toStdString();

    if(res == 1)
        dc.getDb().insertIntoTableEmployees(id, passwd, realName, email, group, department, company, "");
    tcpSendMessage(tcpJson, sock);
}

void VideoConferencingServer::handleLogin(QJsonObject Data, string ip, VideoConferencingServer::sock_ptr sock)
{
    string tcpJson;
    string emailid = Data.value("DATA")["EMAILID"].toString().toStdString();
    string passwd = Data.value("DATA")["PASSWD"].toString().toStdString();

    string verifyRes;
    int result;
    dc.jsonStrVerifyAccountResult(emailid, passwd,verifyRes, result);

    if(result == 1)
    {
        dc.getDb().updateStateByEmaiID(emailid, 1, ip);
    }
    tcpSendMessage(tcpJson, sock);
}

void VideoConferencingServer::handleExit(QJsonObject Data, VideoConferencingServer::sock_ptr sock)
{
    string tcpJson;
    string emailid = Data.value("DATA")["FROM"].toString().toStdString();

    string verifyRes;
    dc.getDb().updateStateByEmaiID(emailid, 0, "");
}

void VideoConferencingServer::handleAccountDetail(QJsonObject Data, VideoConferencingServer::sock_ptr sock)
{

}

void VideoConferencingServer::handleColleagueList(QJsonObject Data, VideoConferencingServer::sock_ptr sock)
{

}



QJsonObject VideoConferencingServer::stringToQJsonObject(std::string string)
{
    QString qString = QString::fromStdString(string);
    QJsonObject DATA = QJsonDocument::fromJson(qString.toUtf8()).object();

    return DATA;
}