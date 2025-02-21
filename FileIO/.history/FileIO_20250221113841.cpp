CFileIO::~CFileIO()
{
    if (fileStream.is_open())
    {
        fileStream.close();
    }
}
