#include "config.h"
#include "rom_metadata.h"

#include <QFile>

#include <mythcontext.h>

#include "zlib.h"
#include MINIZIP_UNZIP_H
#undef Z_NULL
#define Z_NULL nullptr

static int calcOffset(const QString& GameType, uLong filesize) {
    int result = 0;

    if (GameType == "NES")
    {
        result = 16;
    }
    else if (GameType == "SNES")
    {
         uLong rom_size = (filesize / 0x2000) * 0x2000;

         if (rom_size < filesize)
             result = filesize - rom_size;
    }
    else if (GameType == "PCE")
    {
         if (filesize & 0x0FFF)
             result = filesize & 0x0FFF;

    }

    return result;
}

static QString crcStr(uLong crc)
{
    QString tmpcrc("");

    tmpcrc = QString("%1").arg( crc, 0, 16 );
    if (tmpcrc == "0")
        tmpcrc = "";
    else
        tmpcrc = tmpcrc.rightJustified(8, '0');

    return tmpcrc;
}

// Return the crc32 info for this rom. (ripped mostly from the old neshandler.cpp source)
QString crcinfo(const QString& romname, const QString& GameType, QString *key, RomDBMap *romDB)
{
    // Get CRC of file
    std::array<char,32768> block {};
    uLong crc = crc32(0, Z_NULL, 0);
    QString crcRes;
    unz_file_info file_info {};

    int blocksize = 8192;
#if 0
    LOG(VB_GENERAL, LOG_DEBUG,
        QString("crcinfo : %1 : %2 :").arg(romname).arg(GameType));
#endif

    unzFile zf = unzOpen(qPrintable(romname));
    if (zf != nullptr)
    {
        for (int FoundFile = unzGoToFirstFile(zf); FoundFile == UNZ_OK;
             FoundFile = unzGoToNextFile(zf))
        {
            if (unzOpenCurrentFile(zf) == UNZ_OK)
            {
                std::string filename_inzip(256,'\0');
                unzGetCurrentFileInfo(zf,&file_info,filename_inzip.data(),filename_inzip.size(),nullptr,0,nullptr,0);

                int offset = calcOffset(GameType, file_info.uncompressed_size);

                if (offset > 0)
                    unzReadCurrentFile(zf, block.data(), offset);

                // Get CRC of rom data
                int count = 0;
                while ((count = unzReadCurrentFile(zf, block.data(), blocksize)) > 0)
                {
                    crc = crc32(crc, (Bytef *)block.data(), (uInt)count);
                }
                crcRes = crcStr(crc);
                *key = QString("%1:%2").arg(crcRes, filename_inzip.data());

                if (romDB->contains(*key))
                {
                    unzCloseCurrentFile(zf);
                    break;
                }

                unzCloseCurrentFile(zf);
            }
        }
        unzClose(zf);
    }
    else
    {
        QFile f(romname);

        if (f.open(QIODevice::ReadOnly))
        {
            int offset = calcOffset(GameType, f.size());

            if (offset > 0)
                f.read(block.data(), offset);

            // Get CRC of rom data
            qint64 count = 0;
            while ((count = f.read(block.data(), blocksize)) > 0)
            {
                crc = crc32(crc, (Bytef *)block.data(), (uInt)count);
            }

            crcRes = crcStr(crc);
            *key = QString("%1:").arg(crcRes);
            f.close();
        }
    }

    return crcRes;
}

