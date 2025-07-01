#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/imgproc.hpp>
#define NOMINMAX
#include <Windows.h>
#include <map>
#include <thread>
#include <mutex>
#include <limits>
#include <fstream>



//terminal ekranýnda 2 defa ayný nesne konumu çýkmasý kameranýn ýþýklandýrmasý ile ilgili.
//kod bu hali ile iyi çalýþýyor sadece pid parametre ayarlamalarý yapýlmalý diðer kodda errorX double türünde yapmaya çalýþcam
//yani int object_center ve int nextX deðerleri double ile deðiþtircem! 

std::map<std::string, std::vector<std::pair<cv::Scalar, cv::Scalar>>> color_ranges = { {"red",{{cv::Scalar(0,120,70),cv::Scalar(10,255,255)},{cv::Scalar(170,120,70),cv::Scalar(180,255,255)}}},
	{"blue",{{cv::Scalar(100,150,0),cv::Scalar(140,255,255)}}},
	{"green",{{cv::Scalar(35,100,100),cv::Scalar(85,255,255)}}},
	{"yellow",{{cv::Scalar(20,100,100),cv::Scalar(30,255,255)}}},
	{"black",{{cv::Scalar(0,0,0),cv::Scalar(180,255,50)}}} };

std::string selected_color = "";
std::mutex color_mutex;

std::vector<std::vector<cv::Point>> filtered_contour;

std::vector<cv::Point> detected_center;

std::vector<double> previous_velocityX, previous_velocityY;

double startTime, currentTime, deltaTime;

double errorX = 0;
double nextX_arti_pid = 0;

bool pid_hesap = false;

std::vector<double> pid_outputs;


//gausianblur iþlemi hsv iþleminden önce yapýlmalýmý kontrol et !
// mask |= temp_mask yerine cv::bitwise_or kullanmak daha mantýklý olabilir bakarsýn!
void detectSelectColor(cv::Mat frame, std::string selected_color) {

	if (color_ranges.find(selected_color) == color_ranges.end()) {
		std::cout << "Bilinmeyen Tespit Edilemeyen Renk !";
		return;
	}
	filtered_contour.clear();
	cv::Mat hsv, mask, blured;
	cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
	cv::GaussianBlur(hsv, blured, cv::Size(5, 5), 0);
	mask = cv::Mat::zeros(frame.size(), CV_8U);

	for (const auto& range : color_ranges[selected_color]) {
		cv::Mat temp_mask;
		cv::inRange(hsv, range.first, range.second, temp_mask);
		mask |= temp_mask;
	}
	cv::threshold(mask, mask, 127, 255, cv::THRESH_BINARY); //bu gereksiz olabilir. inrange zaten 0-255 arasýnda deðer döndürüyor.
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (const auto& cnt : contours) {
		double area = cv::contourArea(cnt);
		if (area > 500) {
			filtered_contour.push_back(cnt);
		}
	}
	//çoklu nesne için moment hesaplamasasý
	int index = 1;
	for (const auto& cnt : filtered_contour) {
		cv::Moments M = cv::moments(cnt);
		if (M.m00 != 0) {
			double object_centerX = static_cast<int>(M.m10 / M.m00);
			double object_centerY = static_cast<int>(M.m01 / M.m00);
			cv::Point center(object_centerX, object_centerY);
			detected_center.push_back(center);

			//std::cout << index << ". Nesne" << "   " << "Merkezi: (" << center.x << ", " << center.y << ")" << std::endl;
			index++;
			cv::circle(frame, center, 5, cv::Scalar(0, 0, 255), -1);
		}
	}
	cv::drawContours(frame, filtered_contour, -1, cv::Scalar(0, 255, 0), 1);
	cv::imshow("renk", frame);
}

void inputThreat() {
	while (true) {
		std::string input;
		std::cout << "Yeni renk gir (red, blue, green, yellow, black) veya 'exit' yaz: ";
		std::cin >> input;

		std::lock_guard<std::mutex> lock(color_mutex);
		if (input == "exit") {
			selected_color = "exit";
			break;
		}
		else if (color_ranges.find(input) != color_ranges.end()) {
			selected_color = input;
		}
		else {
			std::cout << "Geçersiz renk!\n";
		}
	}
}

//ivme ve hýz kullanarak bir sonraki center_point tahmin etme 
void predictPosition() {

	currentTime = static_cast<double>(cv::getTickCount()) / cv::getTickFrequency();
	deltaTime = currentTime - startTime;
	startTime = currentTime;
	//std::cout << "fps: " << deltaTime << " saniye" << std::endl;

	static std::vector<cv::Point> previous_detected_center; //bunu static yapmak kodu hýzlandýrdý static yap
	static double previous_velocityX = 0;
	static double previous_velocityY = 0;
	static double previous_nextX = 0;
	double nextX = 0;
	double nextY = 0;

	if (previous_detected_center.empty()) {
		previous_detected_center = detected_center;
		return;
	}

	int x = 0;
	pid_hesap = false;
	for (size_t i = 0;i < detected_center.size();i++) {
		if (i < previous_detected_center.size()) {

			//std::cout << "i:" << i << "now" << detected_center[i] << "--" << "previous" << previous_detected_center[i] << "--";
			std::cout << "Nesne Konumu :" << detected_center[i] << "------";
			double deltaX = static_cast<double>(detected_center[i].x - previous_detected_center[i].x);
			double deltaY = static_cast<double>(detected_center[i].y - previous_detected_center[i].y);

			double velocityX = deltaX / deltaTime;
			double velocityY = deltaY / deltaTime;

			double old_velocityX = previous_velocityX;
			double old_velocityY = previous_velocityY;
			previous_velocityY = velocityY;
			previous_velocityX = velocityX;

			double accelerationX = (velocityX - old_velocityX) / deltaTime;
			double accelerationY = (velocityY - old_velocityY) / deltaTime;

			nextX = detected_center[i].x + static_cast<double>(velocityX * deltaTime) + 0.5 * accelerationX * deltaTime * deltaTime;
			nextY = detected_center[i].y + static_cast<double>(velocityY * deltaTime) + 0.5 * accelerationY * deltaTime * deltaTime;

			//std::cout << "TahminX =" << nextX << "-" << " TahminY = " << nextY << "-->";

			errorX = detected_center[i].x - previous_nextX;//hata þimdi oldu gibi orta nokta hesaplýo sonra bir sonraki nokta tahmin yapýyo bir sonraki nokta - onceki tahmin = error
			nextX_arti_pid = nextX;
			//std::cout << "errorX = " << errorX << "-";
			//std::cout << "previous_nextX = " << previous_nextX << "-";


			pid_hesap = true;

		}
	}
	previous_detected_center = detected_center;
	previous_nextX = nextX;
	detected_center.clear();
}

double calculatePid() { //pid_output neye uygulanmalý ? -> bir sonraki tahminime uygulanmalý yani nextX e uygulanmalý bence 

	double Kp = 1.2; //0.08 olarak ayarla
	double Kd = 0.015; //kd çok büyük biþe olcak çünkü türev çok büyük bir sayý
	double Ki = 0.003;
	static double previous_errorX = 0;
	static double integral = 0;
	double pid_output = 0;
	double turev = 0;

	if (pid_hesap == true) {

		integral += errorX * deltaTime;
		turev = (errorX - previous_errorX) / deltaTime;
		pid_output = (Kp * errorX) + (Ki * integral) + (Kd * turev);

		//std::cout << "Kp=" << Kp << " " << "Kp*error=" << Kp * errorX << "-";
		//std::cout << "kd=" << Kd << " " << "turev=" << turev << "-";
		//std::cout << "ki=" << Ki << " " << "integral=" << integral << "-";
		//std::cout << "pid_output" << pid_output << std::endl;
	}
	else {
		integral = 0;
		pid_output = 0;
		previous_errorX = 0;
	}

	previous_errorX = errorX;
	return pid_output;
}


//þimdi ne olmasý lazým 3 ana deðer var ilk nesnenin orta noktasýný buluyor sonra bir sonraki orta nokta için tahminde bulunuyor 
//bir sonraki noktaya geçince sonraki nokta - tahmin den = hatayý buluyoz bu hatada pid uyguluyoz bu pid çýktýsýný 
//bir sonraki tahmin yapmak için kullanýcaz yani pid_output + nextX þimdiki tahmin nesnenin bir sonra nereye gidiceðini tahmin etmeye çalýþýyo
//kod þimdiki hali ile iyi gibi sadece belki kp kd ki deðerleri üzerinde oynama yapýlabilir ...
int main() {

	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	

	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cerr << "Kamera acilmadi \n";
		return -1;
	}

	std::ofstream outfile("C:/Users/Halit/Desktop/pid_out.txt");
	if(!outfile.is_open()) {
		std::cerr << "Dosya acilamadi \n";
		return -1;
	}
	std::ofstream outfile1("C:/Users/Halit/Desktop/real_konum.txt");
	if(!outfile.is_open()) {
		std::cerr << "Dosya acilamadi \n";
		return -1;
	}

	startTime = static_cast<double>(cv::getTickCount()) / cv::getTickFrequency();

	std::thread input_thread(inputThreat);
	cv::Mat frame;

	while (true) {
		cap >> frame;
		if (frame.empty()) break;
		std::string current_color;
		{
			std::lock_guard<std::mutex> lock(color_mutex);
			current_color = selected_color;
		}

		if (current_color == "exit") break;
		if (!current_color.empty()) {
			detectSelectColor(frame, current_color);
			predictPosition();
			double pid_output_1 = calculatePid();
			double tahmin = nextX_arti_pid + pid_output_1;
			outfile << tahmin << '\n'; 
			outfile1 << detected_center[0] << '\n';
			outfile.flush();
			outfile1.flush();
			std::cout << "konum_tahmin = " << tahmin << std::endl;
		}
		if (cv::waitKey(1) == 27) break;
	}
	input_thread.join();
	cap.release();
	cv::destroyAllWindows();
	outfile.close();
	return 0;
}