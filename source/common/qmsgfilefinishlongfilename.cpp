#include "qmsgfilefinishlongfilename.h"
#include "qmessagefactory.h"

using namespace QRserver;
using namespace QProfile;
using namespace rserver;

QMessageSelfRegisteration<QMsgFileFinishLongFileName>
registerQMsgFileFinishLongFileName(eFileFinishEx);

QMsgFileFinishLongFileName::QMsgFileFinishLongFileName(QObject *parent)
    : QMsgFileBlockLongFileName(parent)
{
    setType(eFileFinishEx);
}

QMsgFileFinishLongFileName::QMsgFileFinishLongFileName(const
        QMsgFileFinishLongFileName
        &message)
    : QMsgFileBlockLongFileName(message)
{
    setType(eFileFinishEx);
}

QMsgFileFinishLongFileName::~QMsgFileFinishLongFileName()
{
}


