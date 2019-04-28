import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.12

Item {
    anchors.fill: parent
    signal exit
    property var newMessage: [false, false, false, false]
    Column {
        id: mainPage
        anchors.fill: parent
        Rectangle {
            height: mainWindow.height * 0.12
            width: mainWindow.width
            z: 1
            Row {
                anchors.fill: parent
                anchors.left: parent.left
                anchors.leftMargin: mainWindow.width * 0.03
                spacing: mainWindow.width * 0.03
                Rectangle {
                    id: img
                    width: mainWindow.height * 0.07
                    height: mainWindow.height * 0.07
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        id: _image
                        smooth: true
                        visible: false
                        anchors.fill: parent
                        source: "../resources/xly.png"
                        antialiasing: true
                    }
                    Rectangle {
                        id: _mask
                        color: "black"
                        anchors.fill: parent
                        radius: width / 2
                        visible: false
                        antialiasing: true
                        smooth: true
                    }
                    OpacityMask {
                        id: mask_image
                        anchors.fill: _image
                        source: _image
                        maskSource: _mask
                        visible: true
                        antialiasing: true
                    }
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: conferenceUI.employee.realName
                    //                    font.pixelSize: 25
                }
            }
            Button {
                anchors.verticalCenter: parent.verticalCenter
                text: "退出当前帐号"
                anchors.right: parent.right
                anchors.rightMargin: 20
                onClicked: {
                    conferenceUI.getExitMessage()
                    exit()
                }
            }
        }
        Rectangle {
            width: mainWindow.width
            height: 1
            color: "blue"
        }
        //        Loader {
        //            height: mainWindow.height * 0.88
        //            width: parent.width
        //            id: messageLoader
        //            sourceComponent: messageComponet
        //        }

        //        Component {
        //            id: messageComponet
        Meeting {
            id: meeting
            height: mainWindow.height * 0.88
            width: parent.width
            visible: false
            onMeetingBack: {
                employeeMessage.visible = true
                meeting.visible = false
            }
        }

        Row {
            id: employeeMessage
            visible: true
            Loader {
                id: buttonListLoader
                height: mainWindow.height * 0.88
                width: mainWindow.width * 0.15
                sourceComponent: buttonListComponent
            }
            Connections {
                target: conferenceUI.employee
                onLoginSucceeded: {
                    if (type === "MeetingListRefresh") {
                        home.newMessage[0] = true
                    } else if (type === "NotificationListRefresh") {
                        home.newMessage[2] = true
                    }
                    buttonListLoader.sourceComponent = null
                    buttonListLoader.sourceComponent = buttonListComponent
                }
            }

            Component {
                id: buttonListComponent
                Rectangle {
                    height: mainWindow.height * 0.88
                    width: mainWindow.width * 0.15
                    color: "white"
                    Column {
                        id: tabbarColumn
                        anchors.fill: parent
                        spacing: parent.height * 0.05
                        Repeater {
                            model: ["会议列表", "会议发布", "通知", "个人资料"]
                            Button {
                                width: tabbarColumn.width
                                height: tabbarColumn.height * 0.85 / 4
                                Text {
                                    text: qsTr(modelData)
                                    //                                font.pointSize: 50
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Rectangle {
                                    width: 10
                                    height: 10
                                    radius: width / 2
                                    color: "red"
                                    anchors.right: parent.right
                                    anchors.rightMargin: 10
                                    //                                anchors.top: parent.top
                                    //                                anchors.topMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    visible: newMessage[index]
                                }

                                onClicked: {
                                    if (index === 0) {
                                        meetingList.visible = true
                                        publishMeeting.visible = false
                                        notification.visible = false
                                        personalData.visible = false
                                    } else if (index === 1) {
                                        meetingList.visible = false
                                        publishMeeting.visible = true
                                        notification.visible = false
                                        personalData.visible = false
                                    } else if (index === 2) {
                                        meetingList.visible = false
                                        publishMeeting.visible = false
                                        notification.visible = true
                                        personalData.visible = false
                                    } else if (index === 3) {
                                        meetingList.visible = false
                                        publishMeeting.visible = false
                                        notification.visible = false
                                        personalData.visible = true
                                    }
                                    home.newMessage[index] = false
                                    buttonListLoader.sourceComponent = null
                                    buttonListLoader.sourceComponent = buttonListComponent
                                }
                            }
                        }
                    }
                }
            }
            Rectangle {
                width: 1
                height: mainWindow.width * 0.85
                border.color: "blue"
                border.width: 1
                color: "blue"
            }

            Rectangle {
                height: mainWindow.height * 0.88 - 1
                width: mainWindow.width * 0.85
                MeetingList {
                    id: meetingList
                    visible: true
                    onBeginMeeting: {
                        employeeMessage.visible = false
                        meeting.visible = true
                    }
                    onAttendMeeting: {
                        employeeMessage.visible = false
                        meeting.visible = true
                    }
                    onRecordMeeting: {

                    }
                }
                PublishMeeting {
                    id: publishMeeting
                    visible: false
                }
                Notification {
                    id: notification
                    visible: false
                }
                PersonalData {
                    id: personalData
                    visible: false
                }
            }
            //            }
        }
    }
}
