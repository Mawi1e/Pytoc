#include "ProcessManager.hpp"

int main(int argc, CHAR* argv[]) {
	SetConsoleTitleW(L"<< PYTOC >> by Drakk");
	mawile::ProcessManager* prcManager = new mawile::ProcessManager;

	for (;;) {
		try {
			mawile::PER_PIPEINFO pipeInfo;
			char cbBuf[BUFSIZE];
			std::wstring tmpInput;
			bool flag = false;

			/* Create a Pipe and Initilization */
			prcManager->CreateChildPipe();
			RtlZeroMemory(cbBuf, BUFSIZE);
			pipeInfo.clear();

			/* Create a ChildProcess that Python Interpreter */
			std::cout << "Input the PythonScript FilePath: ";
			std::getline(std::wcin, tmpInput);
			std::cout << '\n';
			prcManager->CreateChildProcess(tmpInput.c_str());

			/* Set the Input Buffer */
			for (;;) {
				std::string inCout;
				std::cout << "Do you need input string?(y, n): ";
				std::getline(std::cin, inCout);
				std::cout << '\n';

				if (inCout == "y" || inCout == "n") {
					pipeInfo.isUseInput = ((inCout == "y") ? true : false);

					if (inCout == "y") {
						std::cout << "Input the value: ";
						int _ = scanf("%[^\n]s", &cbBuf);
						std::cout << '\n';

						pipeInfo.inputBuf = cbBuf;
						flag = true;
					}

					break;
				}
			}

			/* If you want to get the result as a std::vector<std::string> */
			pipeInfo.isReportResultString = true;

			/* Writing Pipe */
			prcManager->WritePipe(&pipeInfo);

			/* Reading Pipe */
			prcManager->ReadPipe(&pipeInfo);
			std::cout << '\n';

			/* If you want to get the result as a std::vector<std::string> */
			std::ofstream ofs("output.txt", std::ios::binary);
			for (auto& a : pipeInfo.outText) {
			//	std::cout << a;
				ofs << a;
			}
			ofs.close();

			/* Dispose an ProcessManager */
			prcManager->Dispose();

			/* Clear the input stream */
			if(flag) std::wcin.ignore();
		}
		catch (std::exception& e) {
			std::cout << e.what() << '\n';

			delete prcManager;
			return (-1);
		}
	}

	delete prcManager;
	return (0);
}