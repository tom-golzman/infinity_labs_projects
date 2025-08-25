#ifndef __ILRD_COMPLEX_NUMBERS_HPP__
#define __ILRD_COMPLEX_NUMBERS_HPP__

/*************************************** Includes ****************************************/
#include <iostream> /* std::ostream, std::istream */

/************************************ namespace ilrd *************************************/
namespace ilrd
{

Complex& operator+=(Complex& c1_, const Complex& c2_);
Complex& operator-=(Complex& c1_, const Complex& c2_);
Complex& operator*=(Complex& c1_, const Complex& c2_);
Complex& operator/=(Complex& c1_, const Complex& c2_);

Complex operator+(const Complex& c1_, const Complex& c2_);
Complex operator-(const Complex& c1_, const Complex& c2_);
Complex operator*(const Complex& c1_, const Complex& c2_);
Complex operator/(const Complex& c1_, const Complex& c2_);

bool operator==(const Complex& c1_, const Complex& c2_);
bool operator!=(const Complex& c1_, const Complex& c2_);

std::ostream& operator<<(std::ostream& os_, const Complex& c_);
std::istream& operator>>(std::istream& is_, Complex& c_);

class Complex
{
public:
    Complex(double real_ = 0.0, double imag_ = 0.0); //non explicit on purpose

    void SetReal(double val_);
    void SetImaginary(double val_);

    double GetReal();
    double GetImaginary();

private:
    double m_real;
    double m_imag;
}

}//namespace ilrd

#endif //__ILRD_COMPLEX_NUMBERS_HPP__
