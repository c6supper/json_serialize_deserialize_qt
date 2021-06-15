#include "qmsgupdatetechnameresult.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMsgUpdateTechNameResult::QMsgUpdateTechNameResult(QObject *parent)
    : QMsgUpdateTechName(parent)
{
}

QMsgUpdateTechNameResult::QMsgUpdateTechNameResult(const
        QMsgUpdateTechNameResult
        &message)
    : QMsgUpdateTechName(message)
{
}

QMsgUpdateTechNameResult::~QMsgUpdateTechNameResult()
{
}
