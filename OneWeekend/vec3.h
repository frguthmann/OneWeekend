#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3
{
public:
	vec3() { e[0] = 0; e[1] = 0; e[2] = 0;}
    vec3(float e0, float e1, float e2){ e[0] = e0; e[1] = e1; e[2] = e2;}
	vec3(float a) { e[0] = a; e[1] = a; e[2] = a; }
	inline float x() const { return e[0];}
    inline float y() const { return e[1];}
    inline float z() const { return e[2];}
    inline float r() const { return e[0];}
    inline float g() const { return e[1];}
    inline float b() const { return e[2];}

    inline const vec3& operator+() const {return *this;}
    inline vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
    inline float operator[](int i) const { return e[i];}
    inline float& operator[](int i) { return e[i];}

    inline vec3& operator+=(const vec3 &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);

    inline float length() const{ return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);}
    inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];}
    inline void make_unit_vector();

    float e[3];
};

inline std::istream& operator>>(std::istream &is, vec3 &v){
    is >> v.e[0] >> v.e[1] >> v.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &v){
    os << v.e[0] << " " << v.e[1] << " " << v.e[2];
    return os;
}

inline void vec3::make_unit_vector(){
    float inv_length = 1.0 / this->length();
    e[0] *= inv_length;
    e[1] *= inv_length;
    e[2] *= inv_length;
}

inline vec3 operator+(const vec3 & v1, const vec3 &v2){
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 & v1, const vec3 &v2){
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 & v1, const vec3 &v2){
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 & v1, const vec3 &v2){
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(float t, const vec3 &v){
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator/(const vec3 & v, float t){
    return vec3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

inline vec3 operator+(const vec3 & v, float t) {
	return vec3(v.e[0] + t, v.e[1] + t, v.e[2] + t);
}

inline vec3 operator-(const vec3 & v, float t) {
	return vec3(v.e[0] - t, v.e[1] - t, v.e[2] - t);
}

inline vec3 operator*(const vec3 & v, float t){
    return vec3(v.e[0]*t, v.e[1]*t, v.e[2]*t);
}

inline float dot(const vec3 & v1, const vec3 &v2){
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

inline vec3 cross(const vec3 & v1, const vec3 &v2){
    return vec3(
        v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1],
        v2.e[0]*v1.e[3] - v2.e[2]*v1.e[0],
        v1.e[0]*v2.e[1] - v1.e[1]-v2.e[0]);
}

inline vec3& vec3::operator+=(const vec3 &v){
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const vec3 &v){
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3 &v){
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline vec3& vec3::operator-=(const vec3 &v){
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const float t){
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline vec3& vec3::operator/=(const float t){
    float k = 1.0 / t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

inline vec3 unit_vector(vec3 v){
    return v / v.length();
}

inline vec3 sqrt(vec3 v) {
	return vec3(sqrt(v.e[0]), sqrt(v.e[1]), sqrt(v.e[2]));
}
