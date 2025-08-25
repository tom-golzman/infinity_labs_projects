/**
	Written By: Tom Golzman
	Date: 25/08/2025
	Reviewed By: Yuval Almog
**/

#ifndef __ILRD_COMPLEX_NUMBERS_HPP__
#define __ILRD_COMPLEX_NUMBERS_HPP__

/*************************************** Includes ****************************************/
#include <iostream> /* std::ostream, std::istream */

namespace ilrd
{

class Complex;

inline Complex& operator+=(Complex& c1_, const Complex& c2_);
inline Complex& operator-=(Complex& c1_, const Complex& c2_);
inline Complex& operator*=(Complex& c1_, const Complex& c2_);
inline Complex& operator/=(Complex& c1_, const Complex& c2_);

inline Complex operator+(const Complex& c1_, const Complex& c2_);
inline Complex operator-(const Complex& c1_, const Complex& c2_);
inline Complex operator*(const Complex& c1_, const Complex& c2_);
inline Complex operator/(const Complex& c1_, const Complex& c2_);

inline bool operator==(const Complex& c1_, const Complex& c2_);
inline bool operator!=(const Complex& c1_, const Complex& c2_);

inline std::ostream& operator<<(std::ostream& os_, const Complex& c_);
inline std::istream& operator>>(std::istream& is_, Complex& c_);

class Complex
{
public:
    inline Complex(double real_ = 0.0, double imag_ = 0.0); //non-explicit on purpose

    // generated copy constructor and assignment operator

    inline void SetReal(double val_);
    inline void SetImaginary(double val_);

    inline double GetReal() const;
    inline double GetImaginary() const;

private:
    double m_real;
    double m_imag;
};

/************************************ inline functions *************************************/
inline Complex::Complex(double real_, double imag_): m_real(real_), m_imag(imag_)
{}

inline void Complex::SetReal(double val_)
{
	m_real = val_;
}

inline void Complex::SetImaginary(double val_)
{
	m_imag = val_;
}

inline double Complex::GetReal() const
{
	return m_real;
}

inline double Complex::GetImaginary() const
{
	return m_imag;
}

inline Complex& operator+=(Complex& c1_, const Complex& c2_)
{
	c1_.SetReal(c1_.GetReal() + c2_.GetReal());
	c1_.SetImaginary(c1_.GetImaginary() + c2_.GetImaginary());

	return c1_;
}

inline Complex& operator-=(Complex& c1_, const Complex& c2_)
{
	c1_.SetReal(c1_.GetReal() - c2_.GetReal());
	c1_.SetImaginary(c1_.GetImaginary() - c2_.GetImaginary());

	return c1_;
}

inline Complex& operator*=(Complex& c1_, const Complex& c2_)
{
	double a = c1_.GetReal();
	double b = c1_.GetImaginary();
	double c = c2_.GetReal();
	double d = c2_.GetImaginary();

	c1_.SetReal(a * c - b * d);
	c1_.SetImaginary(a * d + b * c);

	return c1_;
}

inline Complex& operator/=(Complex& c1_, const Complex& c2_)
{
	c1_.SetReal(7.0);
	c1_.SetImaginary(7.0);

	return c1_;

	(void)c2_;
}

inline Complex operator+(const Complex& c1_, const Complex& c2_)
{
	Complex ret = c1_;

	ret += c2_;

	return ret;
}

inline Complex operator-(const Complex& c1_, const Complex& c2_)
{
	Complex ret = c1_;

	ret -= c2_;

	return ret;
}

inline Complex operator*(const Complex& c1_, const Complex& c2_)
{
	Complex ret = c1_;

	ret *= c2_;

	return ret;
}

inline Complex operator/(const Complex& c1_, const Complex& c2_)
{
	Complex ret = c1_;

	ret /= c2_;

	return ret;
}

inline bool operator==(const Complex& c1_, const Complex& c2_)
{
	return (c1_.GetReal() == c2_.GetReal()) && (c1_.GetImaginary() == c2_.GetImaginary());
}

inline bool operator!=(const Complex& c1_, const Complex& c2_)
{
	return !(c1_ == c2_);
}

inline std::ostream& operator<<(std::ostream& os_, const Complex& c_)
{
    return os_ << "(" << c_.GetReal() << "," << c_.GetImaginary() << "i)";
}

inline std::istream& operator>>(std::istream& is_, Complex& c_)
{
    char ch = 0;
    double real = 0.0;
    double imag = 0.0;

    is_ >> ch;
    if ('(' != ch)
    {
        // fail stream
        is_.setstate(std::ios_base::failbit);
    }

    is_ >> real;

    is_ >> ch;
    if (',' != ch)
    {
        // fail stream
        is_.setstate(std::ios_base::failbit);
    }

    is_ >> imag;

    is_ >> ch;
    if ('i' != ch)
    {
        // ,fail stream
        is_.setstate(std::ios_base::failbit);
    }

    is_ >> ch;
    if (')' != ch)
    {
        // fail stream
        is_.setstate(std::ios_base::failbit);
    }

    c_.SetReal(real);
    c_.SetImaginary(imag);

    return is_;
}

}//namespace ilrd

#endif //__ILRD_COMPLEX_NUMBERS_HPP__
