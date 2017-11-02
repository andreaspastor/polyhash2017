#include "FileHandling.hpp"

//Si WIN32 pas défini, Arbitre->propriétés->C++->Préprocesseur et ajouter WIN32; dans les définitions du préprocesseur
#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <string.h>

using namespace std;

struct FileHandling::DirListStruct
{
#ifdef WIN32
	HANDLE hFile;
	WIN32_FIND_DATAA finddata;
	bool firstElmt;
#else
	struct dirent **namelist;
	int cur;
	int n;
#endif
	string curDir;
};

FileHandling::FileHandling(const char *baseDir)
{
	hDir = new DirListStruct;
	hDir->curDir = baseDir;
	if (hDir->curDir.size() && ((hDir->curDir.c_str()[hDir->curDir.size() - 1] != '/') || (hDir->curDir.c_str()[hDir->curDir.size() - 1] != '\\')))
		hDir->curDir += '/';

#ifdef WIN32
	hDir->firstElmt = true;
	hDir->curDir += "*.*";
	if ((hDir->hFile = FindFirstFileA(hDir->curDir.c_str(), &hDir->finddata)) != INVALID_HANDLE_VALUE);
#else
	if ((hDir->n = scandir(baseDir, &hDir->namelist, 0, alphasort)) >= 0)
	{
		hDir->cur = 0;
	}
#endif
	else
	{
		delete hDir;
		hDir = nullptr;
	}
}

FileHandling::~FileHandling()
{
#ifdef WIN32
	FindClose(hDir->hFile);
#else
	for (int i = 0; i<hDir->n; i++) free(hDir->namelist[i]);
	free(hDir->namelist);
#endif

	delete hDir;
}

bool FileHandling::GetNextElmt(string &str, ETypeResult kind)
{
	bool find = false;
	bool ended = false;

#ifdef WIN32
	if (hDir->firstElmt)
	{
		hDir->firstElmt = false;
		find = true;
	}
	else find = (FindNextFileA(hDir->hFile, &hDir->finddata) != 0);
	while (find && !ended)
	{
		if ((ended = (kind == ETypeResult::FILE_DIR))
			|| (ended = ((kind == ETypeResult::ONLY_FILE ? !(hDir->finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) : hDir->finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)))
			str = hDir->finddata.cFileName;
		else
			find = (FindNextFileA(hDir->hFile, &hDir->finddata) != 0);
	}
#else
	string curFile;
	while (!ended && (find = (hDir->cur<hDir->n)))
	{
		struct stat buf;

		curFile = hDir->curDir;
		curFile += hDir->namelist[hDir->cur]->d_name;
		if (stat(curFile.c_str(), &buf)) find = false;
		else
		{
			if ((ended = (kind == ETypeResult::FILE_DIR))
				|| (ended = (kind == ETypeResult::ONLY_FILE ? !S_ISDIR(buf.st_mode) : S_ISDIR(buf.st_mode))))
				str = hDir->namelist[hDir->cur]->d_name;
		}
		hDir->cur++;
	}
#endif

	return(find);
}

bool FileHandling::isFile(const char *file)
{
#ifdef WIN32
	DWORD attr;

	if ((attr = GetFileAttributesA(file)) == INVALID_FILE_ATTRIBUTES) return(false);
	return((attr & ~FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
	struct stat buf;
	if (stat(file, &buf)) return(false);
	return(!S_ISDIR(buf.st_mode));
#endif
}

bool FileHandling::isDir(const char *file)
{
#ifdef WIN32
	return((GetFileAttributesA(file) & FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
	struct stat buf;
	if (stat(file, &buf)) return(false);
	return(S_ISDIR(buf.st_mode));
#endif
}

void FileHandling::ParseFileName(const char * const file, string &dir, string &fileName, string &ext)
{
	int len = strlen(file);
	bool dot = false;

	if (len == 0)
	{
		ext = "";
		fileName = "";
		dir = "";
	}
	else
	{
		char *str;
		int i;

		str = const_cast<char*>(&file[len - 1]);
		i = 0;
		while ((str != file) && (*str != '\\') && (*str != '/'))
		{
			if ((!dot) && (*str == '.'))
			{
				dot = true;
				ext = str + 1;
				i = 0;
			}
			else i++;
			str--;
		}
		if ((*str != '\\') && (*str != '/'))
		{
			fileName.append(str, i + 1);
			dir = "";
		}
		else
		{
			fileName.append(str + 1, i);
			dir.append(file, len - fileName.size() - ext.size() + (dot ? -1 : 0));
		}
	}
}

size_t FileHandling::GetFileSize(const char * const file)
{
#ifdef WIN32
	HANDLE hFile = CreateFileA(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	size_t size = ::GetFileSize(hFile, NULL);
	CloseHandle(hFile);
	return(size);
#else
	struct stat st;
	stat(file, &st);
	return(st.st_size);
#endif
}
