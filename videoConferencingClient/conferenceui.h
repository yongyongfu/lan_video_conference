#ifndef CONFERENCEUI_H
#define CONFERENCEUI_H

#include <QObject>

#include "employee.h"
#include "videoconferencingclient.h"

class ConferenceUI:public QObject
{
    Q_OBJECT
//    Q_PROPERTY(Company *company READ company WRITE setCompany NOTIFY companyChanged)
    Q_PROPERTY(Employee *employee READ employee WRITE setEmployee NOTIFY employeeChanged)
signals:
//    void companyChanged();
    void employeeChanged();
public:
    ConferenceUI(QObject *parent = 0):QObject(parent){}
//    Company *company() const;
//    void setCompany(Company *company);
    Employee *employee() const;
    Q_INVOKABLE void setEmployee(Employee *employee);

    Q_INVOKABLE void getLoginInformation(QString account,QString password);
    Q_INVOKABLE void getRegisterInformation(QString avator,QString name,QString password,QString company,QString department,QString group,QString phone,QString email);
    Q_INVOKABLE void getLaunchMeetingMessage(QString speaker,QString date,QString time,QString category,QString subject,QString scale,QString dura,QString remark,QList<QString> attendees);
    Q_INVOKABLE void getReplyMeetingInvitation(QString result,QString meetingID,QString cause);
    Q_INVOKABLE void getExitMessage();
    Q_INVOKABLE void getStartMeetingMessage(QString meetingID);
    Q_INVOKABLE void getStopMeetingMessage(QString meetingID);
    Q_INVOKABLE void getAttendMeetingMessage(QString meetingID);
    Q_INVOKABLE void getStartVideoMessage(QString meetingID);
//    Q_INVOKABLE void getSpeakMessage(QString meetingID);

    VideoConferencingClient *getVideoConferencing() const;
    void setVideoConferencing(VideoConferencingClient *videoConferencing);

private:
//    Company *m_company;
    Employee *m_employee;
    VideoConferencingClient *m_videoConferencing;
};

#endif // CONFERENCEUI_H
