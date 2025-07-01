# Predicting-the-Next-Position

In this project, a specific color object is detected in real-time using a webcam feed with OpenCV, and the position of the object is calculated in each frame. Using the detected position, the object's velocity and acceleration are estimated, and based on classical physics equations, its next position is predicted. To minimize errors in this prediction, a PID control algorithm is applied, improving the accuracy of the estimation. Both the predicted and actual positions are saved to text files and later visualized using Python and Matplotlib for comparison. This allows for an analysis of the prediction accuracy. The project combines basic image processing, motion prediction, and control systems, serving as a learning-oriented application.

If we take the PID values ​​like this:
double Kp = 0.08; 
double Kd = 0.015; 
double Ki = 0.003;
The algorithm's next location prediction results like this
