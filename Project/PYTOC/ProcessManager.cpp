#include "ProcessManager.hpp"

namespace mawile {
	void __MAWILE_PIPE_INFO::clear() {
		(this->outText).clear();

		(this->isReportResultString) = false;
		(this->isUseInput) = false;
		(this->isUseWriteOutHandle) = false;

		(this->inputBuf) = nullptr;
		(this->WriteOutHandle) = nullptr;
	}

	void ProcessManager::CreateChildPipe() {
		SECURITY_ATTRIBUTES scrtAttr;
		RtlZeroMemory(&scrtAttr, sizeof(SECURITY_ATTRIBUTES));
		scrtAttr.bInheritHandle = TRUE;
		scrtAttr.lpSecurityDescriptor = NULL;
		scrtAttr.nLength = sizeof(SECURITY_ATTRIBUTES);

		wchar_t cbBuf[BUFSIZE];
		RtlZeroMemory(&cbBuf, BUFSIZE);
		wsprintfW(cbBuf, L"%smawile_temp_file.txt", std::filesystem::temp_directory_path().wstring().c_str());

		(this->tmpFile_Handle) = CreateFileW(
			cbBuf,
			GENERIC_READ,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			0,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			0);
		if ((this->tmpFile_Handle) == INVALID_HANDLE_VALUE) {
			throw std::runtime_error("CreateFileW : Failed");
		}

		if (CreatePipe(&(this->Out_Read), &(this->Out_Write), &scrtAttr, 0) == 0) {
			throw std::runtime_error("CreatePipe : Failed");
		}

		if (SetHandleInformation((this->Out_Read), HANDLE_FLAG_INHERIT, 0) == 0) {
			throw std::runtime_error("SetHandleInformation : Failed");
		}

		if (CreatePipe(&(this->In_Read), &(this->In_Write), &scrtAttr, 0) == 0) {
			throw std::runtime_error("CreatePipe : Failed");
		}

		if (SetHandleInformation((this->In_Write), HANDLE_FLAG_INHERIT, 0) == 0) {
			throw std::runtime_error("SetHandleInformation : Failed");
		}
	}

	void ProcessManager::CreateChildProcess(LPCWSTR childProcess) {
		STARTUPINFO strInfo;
		PROCESS_INFORMATION processInfo;

		RtlZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
		RtlZeroMemory(&strInfo, sizeof(STARTUPINFO));
		strInfo.hStdError = (this->Out_Write);
		strInfo.hStdOutput = (this->Out_Write);
		strInfo.hStdInput = (this->In_Read);
		strInfo.dwFlags |= STARTF_USESTDHANDLES;
		strInfo.cb = sizeof(STARTUPINFO);

		wchar_t buffer[0x200];
		RtlZeroMemory(&buffer, 0x200);

		wsprintfW(buffer, L"python \"%s\"", childProcess);

		int bSuccess = CreateProcessW(0, buffer, 0, 0, TRUE, 0, 0, 0, &strInfo, &processInfo);
		if (bSuccess == 0) {
			throw std::runtime_error("CreateProcessW : Failed");
		}
		else {
			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);

			CloseHandle((this->Out_Write));
			CloseHandle((this->In_Read));
		}
	}

	void ProcessManager::WritePipe(LPPER_PIPEINFO pipeInfo) {
		char cbBuf[BUFSIZE];
		RtlZeroMemory(cbBuf, BUFSIZE);

		DWORD dwRead, bSuccess;

		if (pipeInfo->isUseInput == true) {
			char cbBuf[BUFSIZE];
			RtlZeroMemory(&cbBuf, BUFSIZE);
			sprintf(cbBuf, "%smawile_temp_file.txt", std::filesystem::temp_directory_path().string().c_str());

			std::ofstream ofs(cbBuf);
			ofs << pipeInfo->inputBuf << '\n';
		}

		for (;;) {
			bSuccess = ReadFile((this->tmpFile_Handle), cbBuf, BUFSIZE, &dwRead, 0);
			if (bSuccess == FALSE || dwRead == 0) {
				break;
			}

			bSuccess = WriteFile((this->In_Write), cbBuf, dwRead, 0, 0);
			if (bSuccess == FALSE) {
				break;
			}
		}
	}

	void ProcessManager::ReadPipe(LPPER_PIPEINFO pipeInfo) {
		char cbBuf[BUFSIZE];

		DWORD dwRead, bSuccess;

		for (;;) {
			RtlZeroMemory(cbBuf, BUFSIZE);
			bSuccess = ReadFile((this->Out_Read), cbBuf, BUFSIZE, &dwRead, 0);
			if (bSuccess == FALSE || dwRead == 0) {
				break;
			}

			if (pipeInfo->isReportResultString == true) pipeInfo->outText.push_back(std::move(cbBuf));

			if (pipeInfo->isUseWriteOutHandle == true) {
				bSuccess = WriteFile(pipeInfo->WriteOutHandle, cbBuf, dwRead, 0, 0);
				if (bSuccess == FALSE) {
					break;
				}
			}
			else {
				bSuccess = WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), cbBuf, dwRead, 0, 0);
				if (bSuccess == FALSE) {
					break;
				}
			}
		}
	}

	void ProcessManager::Dispose() {
		CloseHandle((this->In_Write));
		CloseHandle((this->Out_Read));
		CloseHandle((this->tmpFile_Handle));
	}

	ProcessManager::ProcessManager() {

	}

	ProcessManager::~ProcessManager() {

	}
}