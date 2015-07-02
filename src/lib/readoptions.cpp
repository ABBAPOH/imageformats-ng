#include "readoptions.h"

ReadOptions::ReadOptions()
{

}

ReadOptions::ReadOptions(const QString &mimeType)
{
    setMimeType(mimeType);
}

