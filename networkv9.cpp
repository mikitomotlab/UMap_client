#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cassert>
#include <chrono>
#include <vector>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdio.h>
//#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
//#include <lsd.h>
#include "lsdvcpp.h"
#include <time.h>

#include <opencv2/opencv.hpp>
#include <C:\dev\opencv-3.2.0\include\opencv\highgui.h> 
//#include <iostream>
//#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <signal.h>
#include<iostream>
#include<fstream>



#pragma comment(lib, "opencv_highgui343.lib")
#if defined _DEBUG
#define CV_EXT "d.lib"
#else
#define CV_EXT ".lib"
#endif

//#pragma warning(disable : 4996)

#define PORT 20198 //クライアントプログラムとポート番号を合わせてください
#define port 12345

cv::Mat img;
int n_lines;
double* lines;

int imlength = 480;
int imwidth = 270;

//using namespace std;
using namespace cv;

//#ifdef __cplusplus
//extern "C"
//{
//#endif
//	cv::Mat image = cv::imread("image.png", 1);
//#ifdef __cplusplus
//}
//#endif

//cv::Mat change_th_lsd(int nfa, void* dummy)
//cv::Mat change_th_lsd(int nfa, cv::Mat img)
//{
//	cv::Mat result = img.clone();
//	for (int i = 0; i < n_lines; i++)
//	{
//		const double *line = &lines[i * 7];
//		if (nfa < line[6])
//		{
//			const cv::Point p1(line[0], line[1]);
//			const cv::Point p2(line[2], line[3]);
//			cv::line(result, p1, p2, cv::Scalar(0, 0, 255));
//		}
//	}
//	//cv::imshow("result_image", result);
//	//cv::waitKey(0);
//	return (result);
//
//}
//
//Mat from_string (char RECV_DATA[129600], size_t ImageSize)
//{
//	//boost::array<uchar, 10000> RECV_DATA;
//
//	//size_t ImageSize = image_recver.read_some(
//		//boost::asio::buffer(RECV_DATA), ignored_error);
//
//	std::vector<uchar> Img(ImageSize);
//
//	for (int i = 0; i < ImageSize; i++) {
//		Img[i] = RECV_DATA[i];
//		if (i == 129600)
//			break;
//	}
//
//	Mat img(1280, 720, CV_64, Img.data());
//
//	//imshow("1", img);
//	//waitKey(1);
//
//	return(img);
//	img.release();
//}

//std::string wide_to_utf8_winapi(std::wstring const& src)
//{
//	auto const dest_size = ::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
//	std::vector<char> dest(dest_size, '\0');
//	if (::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, dest.data(), dest.size(), nullptr, nullptr) == 0) {
//		throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
//	}
//	dest.resize(std::char_traits<char>::length(dest.data()));
//	dest.shrink_to_fit();
//	return std::string(dest.begin(), dest.end());
//}

//std::wstring utf8_to_wide_winapi(std::string const& src)
//{
//	auto const dest_size = ::MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, nullptr, 0U);
//	std::vector<wchar_t> dest(dest_size, L'\0');
//	if (::MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, dest.data(), dest.size()) == 0) {
//		throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
//	}
//	dest.resize(std::char_traits<wchar_t>::length(dest.data()));
//	dest.shrink_to_fit();
//	return std::wstring(dest.begin(), dest.end());
//}

//std::string wide_to_multi_winapi(std::wstring const& src)
//{
//	auto const dest_size = ::WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
//	std::vector<char> dest(dest_size, '\0');
//	if (::WideCharToMultiByte(CP_ACP, 0U, src.data(), -1, dest.data(), dest.size(), nullptr, nullptr) == 0) {
//		throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
//	}
//	dest.resize(std::char_traits<char>::length(dest.data()));
//	dest.shrink_to_fit();
//	return std::string(dest.begin(), dest.end());
//}

//std::string utf8_to_multi_winapi(std::string const& src)
//{
//	auto const wide = utf8_to_wide_winapi(src);
//	return wide_to_multi_winapi(wide);
//}

int lsd(cv::Mat image)
{
	int argc = 0;
	char** argv = 0;

	std::string in;
	//cv::CommandLineParser parser(argc, argv, "{@input|../data/building.jpg|input image}{help h||show help message}");
	cv::CommandLineParser parser(argc, argv, "{@input|C://Users//tmem//Desktop//Shirahama2018//Shirahama2018//x64//Release//181220160136raspi7.jpg|input image}{help h||show help message}");
	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}
	in = parser.get<std::string>("@input");
	//cv::Mat image = cv::imread(in, cv::IMREAD_GRAYSCALE);
#if 0
	Canny(image, image, 50, 200, 3); // Apply canny edge
#endif
	// Create and LSD detector with standard or no refinement.
#if 1
	cv::Ptr<cv::LineSegmentDetector> ls = cv::createLineSegmentDetector(cv::LSD_REFINE_STD);
#else
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_NONE);
#endif
	double start = double(cv::getTickCount());
	std::vector < cv::Vec4f > lines_std;
	// Detect the lines
	ls->detect(image, lines_std);
	double duration_ms = (double(cv::getTickCount()) - start) * 1000 / cv::getTickFrequency();
	std::cout << "It took " << duration_ms << " ms." << std::endl;
	// Show found lines
	cv::Mat drawnLines(image);
	ls->drawSegments(drawnLines, lines_std);
	cv::imshow("Standard refinement", drawnLines);
	cv::waitKey();
	return 0;
}

cv::Mat rotate(cv::Mat img)
{
	//cv::Mat srcImg = cv::imread("C:\Users\tmem\Desktop\Shirahama2018\Shirahama2018\1_30\sen\image.png");
	cv::Mat srcImg = img;
	/*if (srcImg.empty())
		return 1;*/

	cv::Point2f center = cv::Point2f(
		static_cast<float>(srcImg.cols / 2),
		static_cast<float>(srcImg.rows / 2));

	double degree = 45.0;  // 回転角度
	double scale = 1.0;    // 拡大率

	// アフィン変換行列
	cv::Mat affine;
	cv::getRotationMatrix2D(center, degree, scale).copyTo(affine);

	cv::warpAffine(srcImg, srcImg, affine, srcImg.size(), cv::INTER_CUBIC);

	cv::namedWindow("image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("image", srcImg);
	cv::waitKey();

	return (srcImg);
}



int main()
{
	while (1) {
		///////// TCP 通信 の準備 /////////////////////////////////////////////////////////////////////
		// ポート番号，ソケット
		int srcSocket;  // 自分
		int dstSocket;  // 相手
		// sockaddr_in 構造体
		struct sockaddr_in srcAddr;
		struct sockaddr_in dstAddr;
		int dstAddrSize = sizeof(dstAddr);
		int status;
		// 各種パラメータ
		int i;
		int numrcv = 0;
		int clientname = 0;
		int check = 0;
		//int buffer2[1024];
		//char buffer2[1024];
		std::vector<char> buffer2(1024, 'a');
		//std::vector<char> buffercheck(1024);
		//std::vector<char> *buffercheck;
		char buffer[1024];
		char buffer3[1024];
		char bufferangle[1024];
		//cv::Mat buf1(270, 480, CV_MAKETYPE(CV_8UC1, 1));
		cv::Mat buf1(270, 480, CV_MAKETYPE(CV_8U, 1));
		//cv::Mat* buf2( CV_MAKETYPE(CV_8U, 1));

		// Windows の場合
		WSADATA data;
		WSAStartup(MAKEWORD(2, 0), &data);

		// 接続の受付け
		printf("---Waiting at PORT %d---\n", PORT);

		// sockaddr_in 構造体のセット
		memset(&srcAddr, 0, sizeof(srcAddr));
		srcAddr.sin_port = htons(PORT);
		srcAddr.sin_family = AF_INET;
		srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);

		// ソケットの生成（ストリーム型）
		srcSocket = socket(AF_INET, SOCK_STREAM, 0);
		// ソケットのバインド
		bind(srcSocket, (struct sockaddr *) &srcAddr, sizeof(srcAddr)); 	// std::bind というものが存在している。winsock の bind とは全く別の機能。
		// 接続の許可
		listen(srcSocket, 5000);

		std::cout << "check01" << std::endl;


	//while (1) {
		///// input 画像の前処理 ///////////////////////////////////////////////////////		
		//cout << endl << "     Input a file name: ";
		//cin >> infile;

		// python TCP通信からの入力
		// 接続の受付け
		//std::cout << "---Waiting at PORT %d---\n" << std::endl;
		//printf("---Waiting at PORT %d---\n", PORT);
		dstSocket = accept(srcSocket, (struct sockaddr *) &dstAddr, &dstAddrSize);
		signal(dstSocket, SIG_IGN);
		printf("%s から接続を受けました\n", inet_ntoa(dstAddr.sin_addr));
		//printf("%s から接続を受けました\n", inet_ntoa(dstAddr.sin_addr));

		//パケットの受信
	    clientname = recv(dstSocket, buffer, sizeof(char) * 1024 , 0);
		if (clientname == 0 || clientname == -1) {/*受け取る変数を変えたときはこの変数も変える*/
			status = closesocket(dstSocket); break;
		}

		const char* returnmessage1 = "OK1";
		int len1 = sizeof(returnmessage1);
		//const char *m = (const char)*message;
		const char* m1 = returnmessage1;
		send(dstSocket, m1, len1, 0);
		std::cout << "alreadysended" << std::endl;

		std::cout << clientname << std::endl;


		//パケットの受信
		int angle = recv(dstSocket, bufferangle, sizeof(char) * 1024, 0);
		if (angle == 0 || angle == -1) {
			status = closesocket(dstSocket); break;
		}
		printf("Received packet:\n%s,\n", bufferangle);

		const char* returnmessageangle = "OK";
		int lenangle = sizeof(returnmessageangle);
		//const char *m = (const char)*message;
		const char* mangle = returnmessageangle;
		send(dstSocket, mangle, lenangle, 0);
		std::cout << "alreadysended" << std::endl;
		std::cout << angle << std::endl;
		//printf("Received packet:\n%s,", buffer2);



		//パケットの受信
		//int size = recv(dstSocket, buffer2, sizeof(char) * 1024, 0);
		int size = recv(dstSocket, buffer2.data(), buffer2.size(), 0);
		if (size == 0 || size == -1) {
			status = closesocket(dstSocket); break;
		}
		//printf("Received packet:%s,\n", buffer2);
		printf("Received packet:%p,\n", buffer2);

		const char* returnmessage3 = "OK";
		int len3 = sizeof(returnmessage3);

		//const char *m = (const char)*message;
		const char* m3 = returnmessage3;
		send(dstSocket, m3, len3, 0);
		//printf("Received size:%s,\n", size);
		std::cout << size << std::endl;

		
		//check = (unsigned char)buffer2 ;
		//std::cout << check << std::endl;
		//std::cout << buffer2 << std::endl;
		
		/*
		std::cout << buffer2[0] << std::endl;
		std::cout << buffer2[1] << std::endl;
		std::cout << buffer2[2] << std::endl;
		std::cout << buffer2[3] << std::endl;
		std::cout << buffer2[4] << std::endl;
		std::cout << buffer2[5] << std::endl;
		*/

		//char型で受け取った画像サイズをintに変換する
		int tmp = 0;
		for (int i = 0; i < size; i++) {
			tmp += std::pow(10, size-(i+1))*(buffer2[i] - '0');
		}
		std::cout << tmp << std::endl;

		int recvlen = 100;
		int checkrecv = 0;
		int checkrecv1 = 0;
		int stemp1;
		//char strage[480][270];
		std::string strage[480][270];
		//std::vector<char> buftemp(1024);
		std::vector<std::string> buftemp(129600);
		std::vector<char> buftemptry(129600);
		std::vector<byte> buftempcheck(129600);
		//std::vector<std::string> buftemptry;
		//cv::Mat buf1;
		//cv::Mat buf1 = cv::Mat(270,480,CV_32FC3);
		
		//cv::Mat buf1 = cv::Mat(270, 480, CV_8UC1);
		//cv::Mat buf1(270, 480, CV_MAKETYPE(CV_8UC1, 1));
		//cv::Mat buf1;
		std::vector<uchar> ibuff;
		std::vector<std::string> vbuf;

		Sleep(1);
	
		//getchar();
		std::vector<char> buffercheck(129600);
	    //char* buffercheck =0;
	
		//char* aaaa = 0;
		std::cout << "check1" << std::endl;

		int receivedstr;
		//画像の受け取り
		/*while (recvlen > 0) {
			//int receivedstr = recvfrom(dstSocket, buffer3, sizeof(char) * 1024, 0, NULL, NULL);
			for (int w = 0; w < 2; w++) {
				receivedstr = recv(dstSocket, buffercheck.data(), buffercheck.size(), 0);
				if (receivedstr == 0 || receivedstr == -1) {
					//if (buffer3 == 0 || buffer3 == -1) {
					perror("recv");
					status = closesocket(dstSocket);
					//WSACleanup();
					std::cout << "no image" << std::endl;
					break;
				}
				//try4
				int m = 0;
				int k = 0;
				char* it2 = buffercheck.data() ;
				for (m = 0; m < buffercheck.size(); ++m) {
					printf("%d,", *it2);
					//printf("%d,", receivedstr);
					//getchar();
					buftemptry[k] = *it2;
					//buftemptry.data() + k = *it2;
					//buftemptry.push_back(*it2);
					it2++;
				}
				getchar();
				if (receivedstr == 0 || receivedstr == -1) {
					//if (buffer3 == 0 || buffer3 == -1) {
					perror("recv");
					status = closesocket(dstSocket);
					//WSACleanup();
					std::cout << "no image" << std::endl;
					break;
				}
			}
			if (receivedstr == 0 || receivedstr == -1) {
				//if (buffer3 == 0 || buffer3 == -1) {
				perror("recv");
				status = closesocket(dstSocket);
				//WSACleanup();
				std::cout << "no image" << std::endl;
				break;
			}

			*/

			std::cout << buffercheck.size() << std::endl;

			//buffercheck.clear();
			//buftemptry.clear();
			//WSACleanup();

			//std::cout << buffercheck.size() << std::endl;
			//int receivedstr = recv(dstSocket, aaaa, buffercheck.size(), 0);
			//std::cout << aaaa << std::  endl;
			//int receivedstr = recv(dstSocket, buffercheck, sizeof(buffercheck), 0);
			//std::cout << receivedstr << std::endl;
			//std::cout << buffer3[0] << std::endl;
			int v = 0;
			char* it2 = buffercheck.data();
			int da = 0;

			while (recvlen > 0) {
				//int receivedstr = recvfrom(dstSocket, buffer3, sizeof(char) * 1024, 0, NULL, NULL);
					receivedstr = recv(dstSocket, buffercheck.data(), buffercheck.size(), 0);
					for (da = 0; da < buffercheck.size(); da++) {
						//buftemptry.push_back(buffercheck[da]);
						buftemptry[da] = buffercheck[da];
						/*std::cout << "ここが見たい1" << buffercheck[da] << std::endl;
						getchar();
						std::cout << "ここが見たい2" << buftemptry[da] << std::endl;
						getchar();
						std::cout << "ここが見たい3" << buftemptry.data() << std::endl;
						getchar();
						*it2++;*/
					}

					//buftemptry[v] = *it2;
					//int ga = 0;
					//for (ga = 0; ga < buftemptry.size(); ga++){
					//	std::ofstream outputFile("output_networkv8.txt");
					//    //std::ofstream outputFile.open("output_networkv8.txt");
					//    outputFile << buftemptry[ga]<<std::endl;
					//    //std::cout << "ここが見たい"<< buftemptry[ga] << std::endl;
					//	outputFile.close();
			  //       }

					//getchar();

					//int get[196000];
					//char kuuhaku[196000];
					//char kanma[196000];
					//FILE *fp;
					//errno_t error;

					//fopen_s(&fp, "output_networkv8.txt", "r");

					//fscanf_s(fp, "%d%[^,]%s" , get, kanma, kuuhaku);

					//fclose(fp);

					////fp = fopen("outputfile_networkv8.txt", ios::trunc);

					//it2++;
					if (receivedstr == 0 || receivedstr == -1) {
						perror("recv");
						status = closesocket(dstSocket);
						//WSACleanup();
						std::cout << "no image" << std::endl;
						break;
					}
				


			//if (receivedstr == 0 || receivedstr == -1) {
			//	//if (buffer3 == 0 || buffer3 == -1) {
			//	perror("recv");
			//	status = closesocket(dstSocket);
			//	//WSACleanup();
			//	std::cout << "no image" << std::endl;
			//	break;
			//}





			/*char* aaaa = 0;

			for (int r = 0; r < buftemptry.size(); r++) {
				char bbbb = buftemptry[r];
				sprintf(aaaa, "%s %s\n", aaaa, bbbb);

				std::cout << aaaa << std::endl;
			}
			getchar();
*/



			//sprintf(buftempcheck, "%02X", buffercheck);



			//buf1 = from_string(buffercheck.data(), size);











			recvlen = sizeof(receivedstr);
			//buf1 += receivedstr;
			//std::cout << buftemp.data() << std::endl;
			std::cout << "ここ？" << std::endl;

			std::cout << buffercheck.data() << std::endl;
			//getchar();

			std::cout << buftemptry.data() << std::endl;
			//getchar();
			//std::cout << *buffercheck.data() << std::endl;
			//getchar();
			std::cout << buftemptry.size() << std::endl;

			buffercheck.clear();
			//getchar();
			//std::cout << *buftemptry.begin() << std::endl;
			//getchar();
			//std::cout << *buftemptry.end() << std::endl;
			//getchar();


			//std::cout << buffercheck[2] << std::endl;

			//buftempに受け取った画像データを最後尾から入れる
			//for (int i = 0; i < buffercheck.size(); i++) {
			//	//std::cout << buffercheck[i] << std::endl;
			//	buftemp.push_back(buffercheck[i]);
			//}

			//utf-8からShift_JISへ変換
			//for (int i = 0; i < buffercheck.size(); i++) {
			//	//std::cout << buffercheck[i] << std::endl;
			//	std::string str(1, buffercheck[i]);
			//	utf8_to_multi_winapi(str);
			//	buftemp.push_back(str);
			//	
			//}

			/*std::cout << "v_shift_jis" << std::endl;
			std::cout << buftemp.data() << std::endl;*/
			////1次元から2次元に変換
			//std::vector <std::vector<int>> buftemp2(480, std::vector<int>(270, 0));
			//int a;
			//int b; 
			//int c =0;

			//for (a = 0; a <= 480; a++)
			//	for (b = 0; b <= 270; b++)
			//		buftemp2.at(a).at(b) = buftemp.at(c);
			//        c++;
			//char型からstring型へ変換
			//for (int i = 0; i < buffercheck.size(); i++) {
			//	//std::cout << buffercheck[i] << std::endl;
			//	//std::string str(1, buffercheck[i]);
			//	//utf8_to_multi_winapi(str);
			//	buftemp.push_back(buffercheck.data());

			//}

			//std::cout << buftemp.data() << std::endl;

			//for (int i = 0; i < buffercheck.size(); i++) {
			//	//std::cout << buffercheck[i] << std::endl;
			//	//std::string str(1, buffercheck[i]);
			//	//utf8_to_multi_winapi(str);

			//try1
			/*char* tryvec = buffercheck.data();
			for(int tryc =0; tryc<buffercheck.size(); tryc++)
			std::cout << "buffercheck.data" << *tryvec++ << std::endl;

			getchar();*/

			//try2
			/*buftemptry.assign(buffercheck.begin(), buffercheck.end());

			char* tryvec1 = buffercheck.data();
			for (int tryc1 = 0; tryc1 < buffercheck.size(); tryc1++)
				std::cout << "buffercheck.data" << *tryvec1++ << std::endl;
			
			getchar();
*/
			//try3
			//int k = 0;
			//for (auto it = buffercheck.begin(); it != buffercheck.end(); ++it) {
			//	printf("%d,", *it);
			//	//buftemptry[k] = *it;
			//	buftemptry.push_back(*it);
			//	k++;
			//    }
			//getchar();




			//try4
			//int m = 0;
			//char* it2 = buffercheck.data() + sizeof(char);
			//for (m =0; m<buffercheck.size(); ++m) {
			//	printf("%d,", *it2);
			//	//buftemptry[k] = *it;
			//	buftemptry.push_back(*it2);
			//	it2++;
			//}
			//getchar();




			//try5
			//buftemptry.push_back(*buffercheck);

			//buftemptry.assign(buffercheck.begin(), buffercheck.end());

			//try4
			/*for (int z = 0; z < buffercheck.size(); z++) {
				buftemptry.push_back(buffercheck[z]);
			}*/

			/*std::cout << "buftemptry.data" << buftemptry.data() << std::endl;
			std::cout << "buftemptry.size" << buftemptry.size() << std::endl;*/
			//}

			/*for (int r = 0; r < buftemptry.size(); r++) {
				std::cout << "buftemptry" << buftemptry[r] << std::endl;
			}
			getchar();*/
			

			////buffer内のデータをstrage配列に代入する
			//int i=0;
			//std::cout << "buffercheck.size()" << buffercheck.size() << std::endl;

			//for (int s = 0; s < 480; s++) {
			//	for (int u = 0; u < 270; u++) {
			//		std::cout<< "buffercheck.data" << buffercheck.data() << std::endl;
			//		strage[s][u] = buffercheck.data[i];
			//		//std::cout<< "strage[s][u]" << strage[s][u] << std::endl;
			//		i++;
			//		std::cout << "i" << i << std::endl;
			//		if (i == buffercheck.size())
			//			break;
			//	}
			//}
			//getchar();



			//buffercheck.clear;

			//for (int i = 0; i < buffercheck.size(); i++) {
			//	//std::cout << buffercheck[i] << std::endl;
			//	//std::string str(1, buffercheck[i]);
			//	//utf8_to_multi_winapi(str);
			//	strage = buffercheck.at(i);

			//}


			//受け取った画像のbyte数を追加
			for (i = 0; i < sizeof(buffer3); i++)
				ibuff.push_back((uchar)buffer3[i]);


			checkrecv += receivedstr;
			std::cout << checkrecv << std::endl;
			std::cout << "OK1" << std::endl;
			checkrecv1 = checkrecv + 0;

			//getchar();

			//unsigned char *data = buf1.data;
			//int checkdata = *data;//unsigned char* -> unsigned char -> int の流れでcheckの型と同じ型に変更したい
			printf("Received packet:%p,\n", tmp);
			
			
			//画像のbyte数が受け取った数字と一致したら画像を受け取ったこととみなす
			if (checkrecv1 == tmp) {
				std::cout << checkrecv1 << std::endl;
				std::cout << tmp << std::endl;
				break;
				/*if (sizeof(checkdata) == sizeof(check)) {
					std::cout << sizeof(checkdata) << std::endl;
					std::cout << sizeof(check) << std::endl;
					break;*/
			}
		}

		//buffer内のデータをstrage配列に代入する
		//i = 0;
		//std::cout << "buffercheck.size()" << buffercheck.size() << std::endl;
		////try1
		//for (int s = 0; s < 480; s++) {
		//	for (int u = 0; u < 270; u++) {
		//		std::cout << "buffercheck.data" << buffercheck.data() << std::endl;
		//		strage[s][u] = buftemptry[i];
		//		std::cout<< "strage[s][u]" << strage[s][u] << std::endl;
		//		i++;
		//		std::cout << "i" << i << std::endl;
		//		if (i == buftemptry.size())
		//			break;
		//	}
		//	if (i == buftemptry.size())
		//		break;
		//}

		//try2
		//int k = 0;
		//int j = 0;
		//for (int s = 0; s < 480; s++) {
		//	for (int u = 0; u < 270; u++) {
		//		//for (auto it1 = buftemptry.begin(); it1 != buftemptry.end(); ++it1) {
		//		auto it1 = buftemptry.begin() + k ;
		//		//printf("%d,", *it1);
		//		strage[s][u] = *it1;
		//		//printf("strage", strage[s][u]);
		//		std::cout << strage[s][u] << " ";
		//		k++;
		//		j++;
		//		if (j == buftemptry.size())
		//			break;
		//	}
		//	std::cout << std::endl;
		//	if (j == buftemptry.size())
		//		break;
		//	//buftemptry[k] = *it;
		//	//buftemptry.push_back(*it);
		//}
		for (int ww = 0; ww < buftemptry.size(); ww++)
			std::cout  << buftemptry[ww] << ' ';
		getchar();
		


		//try3(ここは必要だった)
		//std::vector<std::string> tmp(1);
		std::string st;
		//char st[3];
		int k = 0;
		int j = 0;
		int l = 0;
		for (int s = 0; s < 480; s++) {
			for (int u = 0; u < 270; u++) {
				for (k = 0; k < buftemptry.size(); k++) {
					std::cout << "buftemptry[k]" << buftemptry[k] << std::endl;
					std::string tmp(1,buftemptry[k]);
					//getchar();
					if (tmp[0] == ' ') {
						//for(l = 0; l < k; l++)
						    buftemptry.erase(buftemptry.begin() + k);
						//break;
					}
					if (tmp[0] == ',') {
						//for (l = 0; l < k; l++)
							buftemptry.erase(buftemptry.begin()+k);
						break;
					}
					//std::string str(1,buftemptry[k]);
					st = st + tmp[0];
					std::cout << "st" << st << std::endl;

				}
				//std::memcpy(&strage[s][u], &st, sizeof st);
				strage[s][u] = st;
				//printf("strage1", strage[s][u]);
				std::cout << "strage" <<strage[s][u] << std::endl;
				getchar();
				//k++;
				//j++;
				st.clear();
				/*if (j == buftemptry.size())
					break;*/
			}
			//std::cout << std::endl;
			/*if (j == buftemptry.size())
				break;*/
				//buftemptry[k] = *it;
				//buftemptry.push_back(*it);
		}
		getchar();
















		//try3(ここは必要だった)
		//int k = 1;
		//int j = 0;
		//for (int s = 0; s < 480; s++) {
		//	for (int u = 0; u < 270; u++) {
		//		//for (auto it1 = buftemptry.begin(); it1 != buftemptry.end(); ++it1) {
		//		//char* it1 = buftemptry.data() + k;
		//		//char it1 = buftemptry[k];
		//		//printf("%d,", *it1);
		//		//printf("aaaaaaa");
		//		//std::cout << buftemptry[k] << std::endl;
		//		//getchar();
		//		strage[s][u] = buftemptry[k];
		//		//printf("strage", strage[s][u]);
		//		std::cout << strage[s][u] << std::endl;
		//		k++;
		//		j++;
		//		/*if (j == buftemptry.size())
		//			break;*/
		//	}
		//	//std::cout << std::endl;
		//	/*if (j == buftemptry.size())
		//		break;*/
		//	//buftemptry[k] = *it;
		//	//buftemptry.push_back(*it);
		//}
		//getchar();

		std::cout << "buftemptry.data" << buftemptry.data() << std::endl;
		//getchar();
		
		//std::cout << buf1[0] << std::endl;
		std::cout << "get image data"<< std::endl;
		//std::cout << buftemp[1] << std::endl;

		//vectorからMatへ画像の型を変換する
		//int k;
		//int l;
		//int m = 0;
		////std::cout << buftemp[2] << std::endl;
		//for (k = 0; k <= 480; k++) {
		//	for (l = 0; l <= 270; l++) {
		//		//buf1.data[k][l] = buftemp[m];
		//		buf1.at<int>(k, l) = buftemp[m];
		//		//std::cout << buf1 << std::endl;
		//		//std::cout << "aaaaaaaatt" << std::endl;
		//		m++;
		//	}
		//}

		std::cout << "縦dims:" << buftemptry.size() << std::endl;

		//1次元から2次元に変換
		//std::vector <std::vector<int>> buftemp2(480, std::vector<int>(270, 0));
		//int a;
		//int b;
		//int c = 0;
		//std::cout << buftemp.size() << std::endl;

		//if (imlength * imwidth != buftemp.size()) {
		//	std::cout << "buftemp:"<< buftemp.size() << std::endl;
		//	std::cout << "image size:" << imlength * imwidth << std::endl;
		//	std::cout << "buftempと画像サイズが一致してない！！" << std::endl;
		//	//c = imlength * imwidth - buftemp.size();
		//	//break;
		//}

		//for (a = 0; a <= imlength; a++) {
		//	for (b = 0; b <= imwidth; b++) {
		//		std::cout << "buftemp data:" << buftemp.data() << std::endl;
		//		std::cout << "before buftemp:" << buftemp.at(c) << std::endl;
		//		int num = atoi(buftemp.at(c).c_str());
		//		std::cout << "after buftemp:" << num << std::endl;
		//		buftemp2.at(a).at(b) = num;
		//		c++;
		//		std::cout << buftemp2.at(a).at(b) << std::endl;
		//		if (c == imlength * imwidth - 1)
		//			break;
		//	}
		//}
		//
		std::cout << "縦dims2:" << buftemptry.size() << std::endl;
		std::cout << "changed to 2dim data" << std::endl;

		std::cout << "次にMat形式に変換" << buftemptry.data() << std::endl;

		//memcpy(buf1.data, buftemptry.data(), buftemptry.size() * sizeof(float));

		/*int iml;
		int imw;
		for (iml = 0; iml < buftemp.size(); iml++) {
			for (imw = 0; imw < buftemp.at(0).size(); imw++) {
				buf1.at<cv::Vec2b>(iml,imw) = buftemptry.at(iml).at(imw);
				std::cout << buf1.at<cv::Vec2b>(iml, imw) << std::endl;
			}
		}*/

		/*int iml;
		int imw;
		int count =0;
		for (iml = 0; iml < imlength; iml++) {
			for (imw = 0; imw < imwidth; imw++) {
				buf1.at<cv::Vec2b>(iml, imw) = buftemptry.at(iml).at(imw);
				std::cout << buf1.at<cv::Vec2b>(iml, imw) << std::endl;
				count++;
				std::cout << count << std::endl;
				if ( count == 480*270)
					break;
			}
		}*/

		std::cout <<"要素数"<< sizeof(strage)/sizeof(int) << std::endl;
		//int iml;
		//int imw;
		//int count = 0;
		//for (iml = 0; iml < imwidth; iml++) {
		//	for (imw = 0; imw < imlength; imw++) {
		//		/*buf1.at<cv::Vec2b>(iml, imw) = strage[iml][imw];*/
		//		buf1.data[iml+imw] = strage[iml][imw];
		//		/*std::cout << buf1 << std::endl;*/
		//		count++;
		//		//std::cout << count << std::endl;
		//		if (count == 480 * 270)
		//			break;
		//	}
		//	if (count == 480 * 270)
		//		break;
		//}



		//ここは必要だった
		int iml;
		int imw;
		int count = 0;
		for (iml = 0; iml < imwidth; iml++) {
			for (imw = 0; imw < imlength; imw++) {
				/*buf1.at<cv::Vec2b>(iml, imw) = strage[iml][imw];*/
				
				//buf1.at<char>(iml, imw) = strage[iml][imw];
				//buf1.at<int>(iml, imw) = buf1.at<char>(iml, imw) - '0';
				buf1.at<std::string>(iml, imw) = strage[iml][imw];
				//std::cout <<"buf1"<< buf1.at<int>(iml, imw) << std::endl;
				//std::cout << "strage"<<strage[iml][imw] << std::endl;
				count++;
				//std::cout << count << std::endl;
				/*if (count == 480 * 270)
					break;*/
			}
			/*if (count == 480 * 270)
				break;*/
		}
		std::cout << buf1 << std::endl;
		getchar();
		std::cout << "changed to mat data" << std::endl;

		//int iml1;
		//int imw1;
		//int count = 0;
		//for (iml1 = 0; iml1 < imwidth; iml1++) {
		//	for (imw1 = 0; imw1 < imlength; imw1++) {
		//		/*buf1.at<cv::Vec2b>(iml, imw) = strage[iml][imw];*/
		//		buf2 = buf1.data;
		//		//std::cout << buf1.at<int>(iml, imw) << std::endl;
		//		count++;
		//		//std::cout << count << std::endl;
		//		/*if (count == 480 * 270)
		//			break;*/
		//	}
		//	/*if (count == 480 * 270)
		//		break;*/
		//}

		std::cout << buf1.size() << std::endl;
		std::cout << buf1.rows << std::endl;
		std::cout << buf1.cols << std::endl;
		std::cout << buf1.data << std::endl;
		std::cout << *buf1.data << std::endl;
		//Sleep(3000);
		std::cout << "dims:" << buf1.dims << std::endl;
		//std::cout << (buf1.empty()?"true":"false") << std::endl;
		//std::cout << buftemp.size() << std::endl;

		
		//encodeされた画像データをdecodeする
		
		//cv::Mat imdecode(buf1, CV_LOAD_IMAGE_COLOR);

		//cv::Mat imdecode(buf1, CV_LOAD_IMAGE_GRAYSCALE);
		std::cout << "decode" << std::endl;
		
		//cv::Mat imdecode(buf1, CV_LOAD_IMAGE_GRAYSCALE,buf2);
		//cv::Mat imdecode(buf1, 0, buf2);
		//Sleep(3000);

		//cv::Mat imencode(buf1, CV_LOAD_IMAGE_GRAYSCALE);

		std::cout << "show received image" << std::endl;

		//cv::Mat img = cv::imread("181220160136raspi7.jpg", 1);
		//cv::Mat image(480, 270, CV_32F, img);

		//受け取った画像の表示
		cv::imshow("sensingimage", buf1);
		//cv::imshow("sensingimage", *buf2);
		cv::waitKey(5000);
		cv::imshow("sensingimage", buf1);
		cv::waitKey(5000);
		//cv::imwrite("D:\\dev\\workspace\\UMap\\SEN\\PiCA\\1.img\\'+ rcvmsg+'.jpg", image);

		////LSD用画像に変換＞＜
		//double *dat = new double[img.rows * img.cols];
		//for (int y = 0; y < img.rows; y++)
		//	for (int x = 0; x < img.cols; x++)
		//		dat[y * img.cols + x] = img.at<unsigned char>(y, x);

		//LSD処理
	    //	lines = lsd(&n_lines, dat, img.cols, img.rows);
	    //	cv::imshow("result_image", img);
	    //	cv::waitKey(5000);

	    //	//しきい値の最大値と最小値をもってくる
	   //	int max_NFA = 0;
	   //	/*for (int i = 0; i < n_lines; i++)
	   //		max_NFA = std::max(max_NFA, static_cast<int>(lines[i * 7 + 6]));
	   //	cv::imshow("result_image", img);
	   //	cv::waitKey(5000);
	   //*/
	    ////結果描画用画像
	    ////cv::cvtColor(img, img, CV_GRAY2RGB);

	    ////結果表示用ウィンドウ
	    ////cv::namedWindow("result_image");
	    ////cv::createTrackbar("NFA", "result_image", NULL, max_NFA, change_th_lsd);
	    ////cv::setTrackbarPos("NFA", "result_image", max_NFA);
	    //	img = change_th_lsd(max_NFA, img);
	    //	//結果表示
	    //	cv::imshow("result_image", img);
	    //	cv::waitKey(5000);
		std::cout << "check get imgage" << std::endl;

		//画像を線分検出する
		img = lsd(buf1);
		
		//画像の回転
		img = rotate(img);
		
		//結果表示
		cv::imshow("result_image", img);
		cv::waitKey(5000);

		//白黒反転


		//画像の保存場所の送信
		const char* PATH = "D:\\dev\\workspace\\UMap\\SEN\\PiCA\\4.query\\'+ rcvmsg +'.png";
		cv::imwrite(PATH, img);

		//Matchingprogramへ送信
		struct sockaddr_in server;
		int sock;
		char buf[32];
		int n;

		/* ソケットの作成 */
		sock = socket(AF_INET, SOCK_STREAM, 0);

		/*Matchingprogramへ送信*/

		/* 接続先指定用構造体の準備 */
		server.sin_family = AF_INET;
		server.sin_port = htons(20001);
		//server.sin_addr.s_addr = inet_addr("127.0.0.1");
		InetPton(server.sin_family, "127.0.0.1", &server.sin_addr.S_un.S_addr);

		/* サーバに接続 */
		connect(sock, (struct sockaddr *)&server, sizeof(server));

		const char* returnmessage4 = PATH;
		int len4 = sizeof(returnmessage4);
		//const char *m = (const char)*message;
		const char* m4 = returnmessage4;
		send(sock, m4, len4, 0);
		std::cout << "alreadysended" << std::endl;

		/* サーバからデータを受信 */
		memset(buf, 0, sizeof(buf));
		//n = recv(sock, buffer2, sizeof(size) * 1024, 0);
		n = recv(sock, buffer2.data(), buffer2.size(), 0);

		closesocket(sock);
		WSACleanup();
		/*n = read(sock, buf, sizeof(buf));*/

		//close(sock);

		/*Matchingprogramとの接続を閉じる*/



		//char c_inputpath[64], c_infile[32], c_dummy[8];
		//sscanf(buffer2, "%s %s %s", c_inputpath, c_infile, c_dummy);

		//printf("%s\n", c_inputpath);
		//printf("%s\n", c_infile);

		//inputpath = c_inputpath;
		//infile = c_infile;

		//cout << inputpath << endl;
		//cout << infile << endl;

		//// resize
		//double ratio = dbV[0].cols / (double)src.cols;
		//Mat rsrc = Mat::ones(src.rows * ratio, src.cols * ratio, CV_8UC1);
		//resize(src, rsrc, rsrc.size(), INTER_LINEAR);
		//rsrc = ~rsrc;

		//cout << " size[]: " << rsrc.size().width << "," << rsrc.size().height << endl;
		//
		//
		/*clientへ位置座標を返す*/

		int len5 = sizeof(n);
		//const char *m = (const char)*message;

		std::string message = std::to_string(n);
		const char* m5 = message.c_str();
		send(dstSocket, m5, len5, 0);
		std::cout << "alreadysended" << std::endl;

		closesocket(dstSocket);
		WSACleanup();



	}
}



