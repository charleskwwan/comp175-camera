#include <cmath>

#include "Camera.h"
#include "Algebra.h"

// ----------
//
// PROTECTED
//
// ----------

void Camera::setUVW(const Vector &look, const Vector &up) {
    // w first, since opp look
    wb = Vector(look);
    wb.negate();
    wb.normalize();

    // u next, cross up and w
    ub = cross(up, wb);
    ub.normalize();

    // v last, cross  w and u (or u and look + normalize)
    vb = cross(wb, ub); // no need to normalize
}

double Camera::widthAngle() {
    // width angle = aspect ratio * height angle
    return GetScreenWidthRatio() * heightAngle;
}

// ----------
// NORMALIZING
// ----------

Matrix Camera::unhingeNorm() {
    double c = -near / far;
    return Matrix(
        1, 0, 0,               0,
        0, 1, 0,               0,
        0, 0, -1.0f / (c + 1), c / (c + 1),
        0, 0, -1,              0
    );
}

Matrix Camera::scaleNorm() {
    return Matrix(
        1.0f / (tan(widthAngle() / 2) * far), 0, 0,          0,
        0, 1.0f / (tan(heightAngle / 2) * far),  0,          0,
        0, 0,                                    1.0f / far, 0,
        0, 0,                                    0,          1
    );
}

Matrix Camera::rotateNorm() {
    return Matrix(
        ub[0], ub[1], ub[2], 0,
        vb[0], vb[1], vb[2], 0,
        wb[0], wb[1], wb[2], 0,
        0,     0,     0,     1
    );
}

Matrix Camera::translateNorm() {
    return Matrix(
        1, 0, 0, -eye[0],
        0, 1, 0, -eye[1],
        0, 0, 1, -eye[2],
        0, 0, 0, 1
    );
}


// ----------
//
// PUBLIC
//
// ----------

Camera::Camera() {
    // set defaults, somewhat meaningless tho
    eye = Point(0, 0, 0);
    ub = Vector(1, 0, 0);
    vb = Vector(0, 1, 0);
    wb = Vector(0, 0, 1);

    heightAngle = 0;
    near = 0;
    far = 0;

    screenWidth = 0;
    screenHeight = 0;
}

Camera::~Camera() {
}


// ----------
// SETTERS
// ----------

void Camera::Orient(Point& eye, Point& focus, Vector& up) {
    // compute look from eye and focus
    Vector look = focus - eye;

    this->eye = Point(eye);
    setUVW(look, up);
}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
    this->eye = Point(eye);
    setUVW(look, up);
}

void Camera::SetViewAngle (double viewAngle) {
    heightAngle = viewAngle;
}

void Camera::SetNearPlane (double nearPlane) {
    near = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
    far = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}


// ----------
// GETTERS
// ----------

Point Camera::GetEyePoint() {
    return eye;
}

Vector Camera::GetLookVector() {
    return -wb;
}

Vector Camera::GetUpVector() {
    return vb; // no necessarily the same as what was passed in via Orient, but
              // direction is what matters
}

double Camera::GetViewAngle() {
    return heightAngle;
}

double Camera::GetNearPlane() {
    return near;
}

double Camera::GetFarPlane() {
    return far;
}

int Camera::GetScreenWidth() {
    return screenWidth;
}

int Camera::GetScreenHeight() {
    return screenHeight;
}

// dist from near to far
double Camera::GetFilmPlaneDepth() {
    return far - near;
}

// aspect ratio
double Camera::GetScreenWidthRatio() {
    return (double)screenWidth / (double)screenHeight;
}

// TODO: 4 matrices, most likely
// project world onto film plane
Matrix Camera::GetProjectionMatrix() {
    Matrix m;
    return m;
}

// TODO
// position camera relative to scene (or orient world relative to camera)
Matrix Camera::GetModelViewMatrix() {
    Matrix m;
    return m;
}


// ----------
// GEOMETRY
// ----------

// rotate around v, yaw
void Camera::RotateV(double angle) {
    Matrix rotV = rot_mat(vb, angle); // mtx to rotate around v
    ub = rotV * ub;
    wb = rotV * wb;
}

// rotate around u, pith
void Camera::RotateU(double angle) {
    Matrix rotU = rot_mat(ub, angle); // mtx to rotate around u
    vb = rotU * vb;
    wb = rotU * wb;
}

// rotate around w, roll
void Camera::RotateW(double angle) {
    Matrix rotW = rot_mat(wb, angle);
    ub = rotW * ub;
    vb = rotW * vb;
}

// translate eye
// TODO: currently in world space
//       maybe translate in camera space?
void Camera::Translate(const Vector &dir) {
    eye  = eye + dir;
}

// rotate u, v, w around arbitrary axis
void Camera::Rotate(Point p, Vector axis, double degrees) {
    Matrix rot = rot_mat(p, axis, degrees);
    ub = rot * ub;
    vb = rot * vb;
    wb = rot * wb;
}
