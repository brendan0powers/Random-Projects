/****************************************************************************
** Meta object code from reading C++ file 'mainbot.h'
**
** Created: Tue Mar 3 23:47:39 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainbot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainbot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainBot[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,    9,    8,    8, 0x0a,
      57,   53,    8,    8, 0x0a,
      77,   73,    8,    8, 0x0a,
     108,   73,    8,    8, 0x0a,
     147,  139,    8,    8, 0x0a,
     190,  170,    8,    8, 0x0a,
     242,  226,    8,    8, 0x0a,
     290,  277,    8,    8, 0x0a,
     333,  319,    8,    8, 0x0a,
     363,   53,    8,    8, 0x0a,
     386,   53,    8,    8, 0x0a,
     412,    8,    8,    8, 0x0a,
     435,   53,    8,    8, 0x0a,
     463,   53,    8,    8, 0x0a,
     489,   53,    8,    8, 0x0a,
     511,   53,    8,    8, 0x0a,
     536,   53,    8,    8, 0x0a,
     560,   53,    8,    8, 0x0a,
     589,   53,    8,    8, 0x0a,
     614,    8,    8,    8, 0x0a,
     627,   53,    8,    8, 0x0a,
     649,   53,    8,    8, 0x0a,
     678,   53,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainBot[] = {
    "MainBot\0\0state\0stateChanged(QxtIrcConnection::State)\0"
    "msg\0notice(QString)\0a,b\0"
    "userJoined(QString,QxtIrcNick)\0"
    "userParted(QString,QxtIrcNick)\0channel\0"
    "joinedChannel(QString)\0channel,kicket,from\0"
    "partedChannel(QString,bool,QString)\0"
    "oldnick,newnick\0nickChanged(QxtIrcNick,QxtIrcNick)\0"
    "nick,message\0userQuit(QxtIrcNick,QString)\0"
    "channel,topic\0topicChanged(QString,QString)\0"
    "help(QxtIrcBotMessage)\0testcmd(QxtIrcBotMessage)\0"
    "info(QxtIrcBotMessage)\0"
    "publickey(QxtIrcBotMessage)\0"
    "profile(QxtIrcBotMessage)\0"
    "log(QxtIrcBotMessage)\0recent(QxtIrcBotMessage)\0"
    "whois(QxtIrcBotMessage)\0"
    "sendnotice(QxtIrcBotMessage)\0"
    "invite(QxtIrcBotMessage)\0checkForum()\0"
    "say(QxtIrcBotMessage)\0"
    "speechmode(QxtIrcBotMessage)\0"
    "ascii(QxtIrcBotMessage)\0"
};

const QMetaObject MainBot::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MainBot,
      qt_meta_data_MainBot, 0 }
};

const QMetaObject *MainBot::metaObject() const
{
    return &staticMetaObject;
}

void *MainBot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainBot))
        return static_cast<void*>(const_cast< MainBot*>(this));
    return QObject::qt_metacast(_clname);
}

int MainBot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stateChanged((*reinterpret_cast< QxtIrcConnection::State(*)>(_a[1]))); break;
        case 1: notice((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: userJoined((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QxtIrcNick(*)>(_a[2]))); break;
        case 3: userParted((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QxtIrcNick(*)>(_a[2]))); break;
        case 4: joinedChannel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: partedChannel((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 6: nickChanged((*reinterpret_cast< QxtIrcNick(*)>(_a[1])),(*reinterpret_cast< QxtIrcNick(*)>(_a[2]))); break;
        case 7: userQuit((*reinterpret_cast< QxtIrcNick(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: topicChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: help((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 10: testcmd((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 11: info((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 12: publickey((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 13: profile((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 14: log((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 15: recent((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 16: whois((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 17: sendnotice((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 18: invite((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 19: checkForum(); break;
        case 20: say((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 21: speechmode((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        case 22: ascii((*reinterpret_cast< QxtIrcBotMessage(*)>(_a[1]))); break;
        }
        _id -= 23;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
