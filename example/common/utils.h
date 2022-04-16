#ifndef _utilities_h_included
#define _utilities_h_included

#include <direct.h>
#include <io.h>
#include <fstream>

namespace utilities
{

	namespace fs
	{


		/**
		 * \brief ���Ŀ¼�����ڣ� ���½�Ŀ¼
		 * \param pszPath Ŀ¼ȫ·��
		 */
		void create_folder(const char* pszPath)
		{
			if (_access(pszPath, 0) == -1)
			{
				//����ļ��в�����
				_mkdir(pszPath);
			}
		}


		struct FileInfo
		{
			std::string strPath{ "" };
			std::string strFileName{ "" };
			FileInfo(const char* pszRoot, const char* pszName) : strPath(pszRoot), strFileName(pszName) {}

			bool operator<(const FileInfo& rhs)const
			{
				return this->strFileName < rhs.strFileName;
			}
		};


		/**
		 * \brief �ݹ�����ָ���ļ��� Ĭ�������ļ�
		 * \param vecFiles [out] �ļ���Ϣ
		 * \param pszDir ��Ŀ¼
		 * \param pszSuffix ָ����׺
		 * \return �����ļ�����
		 */
		size_t list_files(std::vector<FileInfo>& vecFiles, const char* pszDir, const char* pszSuffix="*.*")
		{
			char szDirPath[260] = { 0 };
			strcpy_s(szDirPath, pszDir);
			strcat_s(szDirPath, "/");
			strcat_s(szDirPath, pszSuffix);

			intptr_t handle;
			_finddata_t findData;

			handle = _findfirst(szDirPath, &findData);
			if (handle == -1) // ����Ƿ�ɹ�
				return 0;

			do
			{
				if (findData.attrib & _A_SUBDIR)
				{
					if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
						continue;

					strcpy_s(szDirPath, pszDir);
					strcat_s(szDirPath, "/");
					strcat_s(szDirPath, findData.name);

					list_files(vecFiles, szDirPath,  pszSuffix);
				}
				else
				{
					// strcpy_s(dirNew, pszDir);
					// strcat_s(dirNew, "/");
					// strcat_s(dirNew, findData.name);
					vecFiles.emplace_back(pszDir, findData.name);
				}
			} while (_findnext(handle, &findData) == 0);

			_findclose(handle); // �ر��������
			return vecFiles.size();
		}



		/**
		 * \brief �����ļ�
		 * \param pszSrc Դ�ļ�
		 * \param pszNewFile Ŀ���ļ�
		 * \return �Ƿ񿽱��ɹ�
		 */
		bool copyfile(const char* pszSrc, const char* pszNewFile)
		{
			try
			{
				std::ifstream in;
				in.open(pszSrc, std::ios::binary);
				if (in.fail())
				{
					std::cout << "Error 1: Fail to open the source file." << std::endl;
					in.close();
					return false;
				}

				std::ofstream out;
				out.open(pszNewFile, std::ios::binary);
				if (out.fail())
				{
					std::cout << "Error 2: Fail to create the new file." << std::endl;
					out.close();
					in.close();
					return false;
				}

				out << in.rdbuf();
				out.close();
				in.close();

			}
			catch (std::exception e)
			{
			}
			return true;
		}
	}

	
}


#endif // _utilities_h_included