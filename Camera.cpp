#include "Camera.h"
#include "Algebra.h"

// ----------
//
// PROTECTED
//
// ----------

void Camera::setUVW(const Vector &look, const Vector &up) {
    // w first, since opp look
    w = Vector(look);
    w.negate();
    w.normalize();

    // u next, cross up and w
    u = cross(up, w);
    u.normalize();

    // v last, cross  w and u (or u and look + normalize)
    v = cross(w, u); // no need to normalize
}

// ----------
//
// PUBLIC
//
// ----------

Camera::Camera() {
    // set defaults, somewhat meaningless tho
    eye = Point(0, 0, 0);
    u = Vector(1, 0, 0);
    v = Vector(0, 1, 0);
    w = Vector(0, 0, 1);

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
    return -w;
}

Vector Camera::GetUpVector() {
    return v; // no necessarily the same as what was passed in via Orient, but
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

// TODO
// project world onto film plane
Matrix Camera::GetProjectionMatrix() {
    Matrix m;
    return m;
}

// TODO
// position camera relative to scene
Matrix Camera::GetModelViewMatrix() {
    Matrix m;
    return m;
}


// ----------
// GEOMETRY
// ----------

// rotate around v, yaw
void Camera::RotateV(double angle) {
    Matrix rotV = rot_mat(v, angle); // mtx to rotate around v
    u = rotV * u;
    w = rotV * w;
}

// rotate around u, pith
void Camera::RotateU(double angle) {
    Matrix rotU = rot_mat(u, angle); // mtx to rotate around u
    v = rotU * v;
    w = rotU * w;
}

// rotate around w, roll
void Camera::RotateW(double angle) {
    Matrix rotW = rot_mat(w, angle);
    u = rotW * u;
    v = rotW * v;
}

// translate eye
void Camera::Translate(const Vector &dir) {
    eye  = eye + dir;
}

// rotate u, v, w around arbitrary axis
void Camera::Rotate(Point p, Vector axis, double degrees) {
    Matrix rot = rot_mat(p, axis, degrees);
    u = rot * u;
    v = rot * v;
    w = rot * w;
}
