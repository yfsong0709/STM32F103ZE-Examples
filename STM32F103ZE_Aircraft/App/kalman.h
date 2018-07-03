
extern float Angle_X_Kalman;
extern float Angle_Y_Kalman;

void CalAngle_Kalman(float Gyro_x, float Gyro_y, float Gyro_z, float Accel_x, float Accel_y, float Accel_z);
void Kalman_Filter_X(float Accel,float Gyro);
void Kalman_Filter_Y(float Accel,float Gyro);

