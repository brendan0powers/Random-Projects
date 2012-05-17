/****************************************************************************
** Meta object code from reading C++ file 'messageservice.h'
**
** Created: Sat Feb 28 23:51:51 2009
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

 // signals: signature, parameters, type, tag, flags
      32,   28,   16,   15, 0x05,
      55,   48,   16,   15, 0x05,
      93,   84,   16,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MessageService[] = {
    "MessageService\0\0ReturnValue\0msg\0"
    "notice(QString)\0to,msg\0"
    "sendMessage(QString,QString)\0revision\0"
    "svnCommit(int)\0"
};

const QMetaObject MessageService::staticMetaObject = {
    { &ClientProxy::staticMetaObject, qt_meta_stringdata_MessageService,
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
    return ClientProxy::qt_metacast(_clname);
}

int MessageService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ClientProxy::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
ReturnValue MessageService::notice(QString _t1)
{
    ReturnValue _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}

// SIGNAL 1
ReturnValue MessageService::sendMessage(QString _t1, QString _t2)
{
    ReturnValue _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
    return _t0;
}

// SIGNAL 2
ReturnValue MessageService::svnCommit(int _t1)
{
    ReturnValue _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
    return _t0;
}
QT_END_MOC_NAMESPACE
