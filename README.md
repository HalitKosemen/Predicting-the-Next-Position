# Predicting-the-Next-Position
🎯 Real-Time Color-Based Object Tracking & Motion Prediction with PID Control<br><br>This project demonstrates a real-time object tracking system using OpenCV that detects a user-selected colored object, predicts its future position using velocity and acceleration, and refines the prediction using a PID controller.<br><br>The system is designed to be modular, extensible, and suitable for control systems experimentation, computer vision learning, and real-time tracking applications.<br><br><br>📌 Key Features<br><br>🔍 Contour filtering for noise reduction<br><br>🚀 Motion prediction using kinematic equations<br><br>🎯 PID control for prediction error correction<br><br>⌨️ Real-time color selection via multithreading<br><br>📄 Logging of real position and PID-corrected prediction<br><br>📷 Live camera visualization


## 🌐 Socials:
[![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?logo=linkedin&logoColor=white)](https://www.linkedin.com/in/halit-k%C3%B6semen-62b9992b4/)
[![Email](https://img.shields.io/badge/Email-D14836?logo=gmail&logoColor=white)](mailto:halit.kosemen@hotmail.com)

# 💻 Tech Stack:
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![OpenCV](https://img.shields.io/badge/opencv-%23white.svg?style=for-the-badge&logo=opencv&logoColor=white)
# 📊 Project Details And Video:

If we take the PID values ​​like this:

double Kp = 0.08; 

double Kd = 0.015; 

double Ki = 0.003;

The algorithm's next location prediction results like this


![image url](https://github.com/HalitKosemen/Predicting-the-Next-Position/blob/d8c1e508e2ef4da843997bd5a1bea3d3d5031e60/Pict2.JPG)
![image url](https://github.com/HalitKosemen/Predicting-the-Next-Position/blob/65e0e0ac685c2fe5c9efe7d4aae647cd2e4b782b/Pict1.JPG)



If we only increase the Kp coefficient of the system, we will start to see unwanted overshoots and steady-state errors in the system.

double Kp = 1.2; 

double Kd = 0.015; 

double Ki = 0.003;

![image url](https://github.com/HalitKosemen/Predicting-the-Next-Position/blob/f4ef7092d6b452efa20f67ead6250f17c37c440b/Pict11.JPG)
![image url](https://github.com/HalitKosemen/Predicting-the-Next-Position/blob/f4ef7092d6b452efa20f67ead6250f17c37c440b/Pict22.JPG)


If you want, you can add PID coefficients to the threat section, change the value of the coefficients in real time and observe the PID response graph.

## 🎥 Demo Video (YouTube)

[![Watch the video](https://img.youtube.com/vi/27HRKS_gyGA/0.jpg)](https://www.youtube.com/watch?v=27HRKS_gyGA)
