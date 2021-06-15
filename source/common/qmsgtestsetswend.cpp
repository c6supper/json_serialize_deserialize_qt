#include "qmessagefactory.h"
#include "qmsgtestsetswend.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgTestsetSwEnd> registerQMsgTestsetSwEnd(
    rserver::eTestsetSWEND);

QMsgTestsetSwEnd::QMsgTestsetSwEnd(QObject *parent)
    : QMsgTestsetSwBlk(parent)
{
    setType(rserver::eTestsetSWEND);
}

QMsgTestsetSwEnd::QMsgTestsetSwEnd(const QMsgTestsetSwEnd &message)
    : QMsgTestsetSwBlk(message)
{
    setType(rserver::eTestsetSWEND);
}

QMsgTestsetSwEnd::~QMsgTestsetSwEnd()
{
}

