#include "../../Extend/ResourceLoader.h"
#include <vector>

struct PackagedFile
{
	LPSMXFILE file;
	string name;
};

typedef shared_ptr<PackagedFile> LPPACKAGE;

vector<LPPACKAGE> packagedFileList;

void MountPackageFile(LPCSTR filename)
{
	LPSMXFILE fp = OpenSmxFile(filename);
	LPPACKAGE msf(new PackagedFile);
	msf->file = fp;
	msf->name = filename;
	packagedFileList.emplace_back(msf);
}

void UnmountPackageFile(LPCSTR filename)
{
	for (vector<LPPACKAGE>::iterator it = packagedFileList.begin(); it != packagedFileList.end();)
	{
		if ((*it)->name == filename)
			it = packagedFileList.erase(it);
		else
			it++;
	}
}

RCInfo GetResourceInfo(LPCSTR filename)
{
	RCInfo info;
	info.fileName = filename;
	info.exist = false;
	if (_access(filename, 0) == -1)
	{
		LPSRCINFO srcinfo = 0;
		LPSMXFILE pkg = 0;
		for (vector<LPPACKAGE>::iterator it = packagedFileList.begin(); it != packagedFileList.end(); it++)
		{
			srcinfo = LoadSourceFileByPath((*it)->file, filename);
			if (srcinfo)
			{
				pkg = (*it)->file;
				break;
			}
		}
		if (srcinfo)
		{
			info.isPackaged = true;
			info.exist = true;
			info.isZipped = srcinfo->zipped;
			info.nbByteUnzip = srcinfo->bytesunzip;
			info.nbByteZipped = srcinfo->byteszipped;
			info.source = srcinfo;
			info.package = pkg;
		}
	}
	else
	{
		FileStream fs(filename, FileStream::Mode::Open);
		info.isPackaged = false;
		info.exist = true;
		info.isZipped = false;
		info.nbByteUnzip = fs.GetLength();
		info.nbByteZipped = fs.GetLength();
		info.source = 0;
		info.package = 0;
		fs.Close();
	}
	return info;
}

void LoadResourceByInfo(const RCInfo& info, BYTE* pOut, void(*progressReport)(float percent))
{
	if (!info.exist)
		return;
	BYTE buffer[4096];
	if (!info.isPackaged)
	{
		LONG64 offset = 0;
		int readbyte;
		FileStream fs(info.fileName.c_str(), FileStream::Mode::Open);
		while (fs.GetPosition() < fs.GetLength())
		{
			if (fs.GetLength() - fs.GetPosition() < 4096)
				readbyte = fs.GetLength() - fs.GetPosition();
			else
				readbyte = 4096;
			fs.Read(buffer, 0, readbyte);
			memcpy(pOut + offset, buffer, readbyte);
			offset += readbyte;
			if (progressReport)
				progressReport((float)(fs.GetPosition() * 100 / fs.GetLength()));
		}
		fs.Close();
	}
	else
	{
		info.package->fs->SetPosition(info.source->startpos);
		if (info.isZipped)
		{
			LONG64 offset = 0;
			LONG64 length = info.nbByteUnzip;
			LONG64 position = 0;
			int readbyte;
			GZipStream gz(info.package->fs, GZipStream::CompressionMode::Decompress);
			while (position < length)
			{
				if (length - position < 4096)
					readbyte = length - position;
				else
					readbyte = 4096;
				gz.Read(buffer, 0, readbyte);
				position += readbyte;
				memcpy(pOut + offset, buffer, readbyte);
				offset += readbyte;
				if (progressReport)
					progressReport((float)(position * 100 / length));
			}
			gz.Close();
		}
		else
		{
			LONG64 offset = 0;
			LONG64 length = info.nbByteUnzip;
			LONG64 position = 0;
			int readbyte;
			while (position < length)
			{
				if (length - position < 4096)
					readbyte = length - position;
				else
					readbyte = 4096;
				info.package->fs->Read(buffer, 0, readbyte);
				position += readbyte;
				memcpy(pOut + offset, buffer, readbyte);
				offset += readbyte;
				if (progressReport)
					progressReport((float)(position * 100 / length));
			}
		}
	}
}

void LoadResourceFromFile(LPCSTR filename, BYTE* pOut, RCInfo* pInfoOut, void(*progressReport)(float percent))
{
	RCInfo info = GetResourceInfo(filename);
	if (pInfoOut)
		*pInfoOut = info;
	if (!info.exist)
		return;
	LoadResourceByInfo(info, pOut, progressReport);
}

void LoadResourceFromFile(LPCSTR filename, vector<BYTE>& pOut, RCInfo* pInfoOut, void(*progressReport)(float percent))
{
	RCInfo info = GetResourceInfo(filename);
	if (pInfoOut)
		*pInfoOut = info;
	if (!info.exist)
		return;
	pOut.resize(info.nbByteUnzip);
	BYTE buffer[4096];
	if (!info.isPackaged)
	{
		LONG64 offset = 0;
		int readbyte;
		FileStream fs(info.fileName.c_str(), FileStream::Mode::Open);
		while (fs.GetPosition() < fs.GetLength())
		{
			if (fs.GetLength() - fs.GetPosition() < 4096)
				readbyte = fs.GetLength() - fs.GetPosition();
			else
				readbyte = 4096;
			fs.Read(buffer, 0, readbyte);
			memcpy(pOut.data() + offset, buffer, readbyte);
			offset += readbyte;
			if (progressReport)
				progressReport((float)(fs.GetPosition() * 100 / fs.GetLength()));
		}
		fs.Close();
	}
	else
	{
		info.package->fs->SetPosition(info.source->startpos);
		if (info.isZipped)
		{
			LONG64 offset = 0;
			LONG64 length = info.nbByteUnzip;
			LONG64 position = 0;
			int readbyte;
			GZipStream gz(info.package->fs, GZipStream::CompressionMode::Decompress);
			while (position < length)
			{
				if (length - position < 4096)
					readbyte = length - position;
				else
					readbyte = 4096;
				gz.Read(buffer, 0, readbyte);
				position += readbyte;
				memcpy(pOut.data() + offset, buffer, readbyte);
				offset += readbyte;
				if (progressReport)
					progressReport((float)(position * 100 / length));
			}
			gz.Close();
		}
		else
		{
			LONG64 offset = 0;
			LONG64 length = info.nbByteUnzip;
			LONG64 position = 0;
			int readbyte;
			while (position < length)
			{
				if (length - position < 4096)
					readbyte = length - position;
				else
					readbyte = 4096;
				info.package->fs->Read(buffer, 0, readbyte);
				position += readbyte;
				memcpy(pOut.data() + offset, buffer, readbyte);
				offset += readbyte;
				if (progressReport)
					progressReport((float)(position * 100 / length));
			}
		}
	}
}