#include "quaternion.h"
#include <math.h>

#define Kp 0.8f		// 比例系数
#define Ki 0.001f	// 积分系数
#define T 0.008f	// 采样周期

float Angle_X_Quaternion;
float Angle_Y_Quaternion;
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;			// 四元数
float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f; 			// 积分误差

void CalAngle_Quaternion(float gx, float gy, float gz, float ax, float ay, float az){
	float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    // 测量正常化
    norm = sqrt(ax*ax + ay*ay + az*az);      
    ax /= norm;
    ay /= norm;
    az /= norm;      

    // 估计方向的重力
    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3; 

    // 错误的领域和方向传感器测量参考方向之间的交叉乘积的总和
    ex = ay*vz - az*vy;
    ey = az*vx - ax*vz;
    ez = ax*vy - ay*vx;

    // 积分误差比例积分增益
    exInt += ex*Ki;
    eyInt += ey*Ki;
    ezInt += ez*Ki; 

    // 调整后的陀螺仪测量
    gx += Kp*ex + exInt;
    gy += Kp*ey + eyInt;
    gz += Kp*ez + ezInt; 

    // 整合四元数率和正常化
	q0 += (-q1*gx - q2*gy - q3*gz)*T*0.5f;
	q1 += (q0*gx + q2*gz - q3*gy)*T*0.5f;
	q2 += (q0*gy - q1*gz + q3*gx)*T*0.5f;
	q3 += (q0*gz + q1*gy - q2*gx)*T*0.5f;   

    // 正常化四元
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 /= norm;
    q1 /= norm;
    q2 /= norm;
    q3 /= norm;

    Angle_X_Quaternion = asin(2*(q0*q2 - q1*q3)) * 57.3f;
	Angle_Y_Quaternion = atan2(2*q2*q3 + 2*q0*q1, 1 - 2*q1*q1 - 2*q2*q2) * 57.3;
}
