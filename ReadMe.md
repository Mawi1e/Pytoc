# **Pytoc**

+ Run Pythonscript and bring the result to c++.
+ パイソンスクリプトを実行してその結果値をc++に読み込んでください！。
+ 파이썬스크립트를 실행하고 그 결과값을 c++로 불러오세요.


<br></br>
## **Available References**

```cpp
/**
 * @brief CreateChildPipe
 * @exception std::runtime_error
*/
void CreateChildPipe();

/**
 * @brief CreateChildProcess
 * @exception std::runtime_error
*/
void CreateChildProcess(LPCWSTR);

/**
 * @brief WritePipe
 * @exception std::runtime_error
*/
void WritePipe(LPPER_PIPEINFO);

/**
 * @brief ReadPipe
 * @exception std::runtime_error
*/
void ReadPipe(LPPER_PIPEINFO);

/**
 * @brief Dispose
 * @exception std::runtime_error
*/
void Dispose();
		
```


<br></br>
# **Pictures**

![](https://github.com/Mawi1e/Pytoc/blob/main/Pictures/1_1.PNG)
![](https://github.com/Mawi1e/Pytoc/blob/main/Pictures/1_2.PNG)