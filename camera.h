#ifndef CAMERA_H
#define CAMERA_H


#include<QMatrix4x4>

#include <vector>
#include <math.h>

//Move Direction
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:
    // camera Attributes
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = QVector3D(posX, posY, posZ);
        WorldUp = QVector3D(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    QMatrix4x4 GetViewMatrix()
    {
        QMatrix4x4 theMatrix;
        theMatrix.lookAt(Position, Position + Front, Up);
        return theMatrix;
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // 确保当投球超出边界时，屏幕不会翻转
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // 使用更新的Euler角度更新前、右和上矢量
        updateCameraVectors();
    }

    // 处理从鼠标滚轮事件接收的输入。仅需要在垂直车轮轴上输入
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 75.0f)
            Zoom = 75.0f;
    }

private:
    // 根据相机的（更新的）Euler角度计算前矢量
    void updateCameraVectors()
    {
        // calculate the new Front vector
        float PI=3.1415926;
        QVector3D front;
        front.setX(cos(Yaw*PI/180.0) * cos(Pitch*PI/180.0));
        front.setY( sin(Pitch*PI/180.0));
        front.setZ(sin(Yaw*PI/180.0) * cos(Pitch*PI/180.0));
        front.normalize();
        Front = front;
        // also re-calculate the Right and Up vector
        Right = QVector3D::crossProduct(Front, WorldUp);
        // 标准化向量，因为向上或向下看得越多，向量的长度就越接近0，这会导致移动速度变慢。
        Right.normalize();
        Up    = QVector3D::crossProduct(Right, Front);
        Up.normalize();
    }
};
#endif
