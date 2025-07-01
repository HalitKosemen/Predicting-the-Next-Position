# Predicting-the-Next-Position

In this project, a specific color object is detected in real-time using a webcam feed with OpenCV, and the position of the object is calculated in each frame. Using the detected position, the object's velocity and acceleration are estimated, and based on classical physics equations, its next position is predicted. To minimize errors in this prediction, a PID control algorithm is applied, improving the accuracy of the estimation. Both the predicted and actual positions are saved to text files and later visualized using Python and Matplotlib for comparison. This allows for an analysis of the prediction accuracy. The project combines basic image processing, motion prediction, and control systems, serving as a learning-oriented application.

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
