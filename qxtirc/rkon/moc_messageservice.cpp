/****************************************************************************
** Meta object code from reading C++ file 'messageservice.h'
**
** Created: Sat Feb 28 23:51:49 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "messageservice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'messageservice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MessageService[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      32,   28,   16,   15, 0x0a,
      55,   48,   16,   15, 0x0a,
      93,   84,   16,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MessageService[] = {
    "MessageService\0\0ReturnValue\0msg\0"
    "notice(QString)\0to,msg\0"
    "sendMessage(QString,QString)\0revision\0"
    "svnCommit(int)\0"
};

const QMetaObject MessageService::staticMetaObject = {
    { &ServiceProxy::staticMetaObject, qt_meta_stringdata_MessageService,
      qt_meta_data_MessageService, 0 }
};

const QMetaObject *MessageService::metaObject() const
{
    return &staticMetaObject;
}

void *MessageService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MessageService))
        return static_cast<void*>(const_cast< MessageService*>(this));
    return ServiceProxy::qt_metacast(_clname);
}

int MessageService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ServiceProxy::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { ReturnValue _r = notice((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< ReturnValue*>(_a[0]) = _r; }  break;
        case 1: { ReturnValue _r = sendMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< ReturnValue*>(_a[0]) = _r; }  break;
        case 2: { ReturnValue _r = svnCommit((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< ReturnValue*>(_a[0]) = _r; }  break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
