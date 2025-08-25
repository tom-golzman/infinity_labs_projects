/************************************ Includes *************************************/
#include <iostream> // std

#include "test_utils.hpp" /* RUN_TEST, colors, titles, status */
#include "complex_numbers.hpp"

/************************************* Defines *************************************/

/************************** Functions Forward Decleration **************************/
static void TestCtorAndGetters();
static void TestSetters();
static void TestOperatorPlusEqual();
static void TestOperatorMinusEqual();
static void TestOperatorMulEqual();
static void TestOperatorDivEqual();
static void TestBinaryOperators();
static void TestEquality();
static void TestOutputOperator();
static void TestInputOperator();

/************************************** main ***************************************/
int main(void)
{
	using namespace test;
	
	TestCtorAndGetters();
	TestSetters();
    TestOperatorPlusEqual();
    TestOperatorMinusEqual();
    TestOperatorMulEqual();
    TestOperatorDivEqual();
    TestBinaryOperators();
    TestEquality();
    TestOutputOperator();
    TestInputOperator();
	
	std::cout << "\n";

	return 0;
}

/************************************ Functions ************************************/
using namespace ilrd;

static void TestCtorAndGetters()
{
    TITLE("Constructor and Getters");

    Complex c(3.5, -2.1);
	
    RUN_TEST("Real part is 3.5", c.GetReal() == 3.5);
    RUN_TEST("Imag part is -2.1", c.GetImaginary() == -2.1);
}

static void TestSetters()
{
    TITLE("Setters");

    Complex c;

    c.SetReal(1.1);
    c.SetImaginary(2.2);

    RUN_TEST("SetReal = 1.1", c.GetReal() == 1.1);
    RUN_TEST("SetImaginary = 2.2", c.GetImaginary() == 2.2);
}

static void TestOperatorPlusEqual()
{
    TITLE("operator+=");

    Complex c1(1.0, 2.0), c2(3.0, 4.0);

    c1 += c2;

    RUN_TEST("Real += ", c1.GetReal() == 4.0);
    RUN_TEST("Imag += ", c1.GetImaginary() == 6.0);
}

static void TestOperatorMinusEqual()
{
    TITLE("operator-=");

    Complex c1(5.0, 7.0), c2(2.0, 3.0);

    c1 -= c2;

    RUN_TEST("Real -= ", c1.GetReal() == 3.0);
    RUN_TEST("Imag -= ", c1.GetImaginary() == 4.0);
}

static void TestOperatorMulEqual()
{
    TITLE("operator*=");

    Complex c1(1.0, 2.0), c2(3.0, 4.0);

    c1 *= c2;

    RUN_TEST("Real *= ", c1.GetReal() == -5.0);
    RUN_TEST("Imag *= ", c1.GetImaginary() == 10.0);
}

static void TestOperatorDivEqual()
{
    TITLE("operator/=");

    Complex c1(7.0, 7.0), c2(1.0, -1.0);
    
	c1 /= c2;

    RUN_TEST("Real /=", c1.GetReal() == 7.0);
    RUN_TEST("Imag /=", c1.GetImaginary() == 7.0);
}

static void TestBinaryOperators()
{
    TITLE("Binary operators +, -, *, /");

    Complex c1(2.0, 3.0), c2(1.0, -1.0);

    Complex plus = c1 + c2;
    Complex minus = c1 - c2;
    Complex mul = c1 * c2;
    Complex div = c1 / c2;

    RUN_TEST("Plus", plus.GetReal() == 3.0 && plus.GetImaginary() == 2.0);
    RUN_TEST("Minus", minus.GetReal() == 1.0 && minus.GetImaginary() == 4.0);
    RUN_TEST("Mul", mul.GetReal() == 5.0 && mul.GetImaginary() == 1.0);
    RUN_TEST("Div", div.GetReal() == 7.0 && div.GetImaginary() == 7.0);
}

static void TestEquality()
{
    TITLE("operator== and !=");

    Complex c1(1.0, 2.0), c2(1.0, 2.0), c3(2.0, 1.0);

    RUN_TEST("c1 == c2", c1 == c2);
    RUN_TEST("c1 != c3", c1 != c3);
}

static void TestOutputOperator()
{
    TITLE("operator<<");

	Complex c(3.5, -1.5);

	std::cout << "Expected output: (3.5,-1.5i)\n";
    std::cout << "Actual output  : " << c << std::endl;
}

static void TestInputOperator()
{
	TITLE("operator>>");

    Complex c;

    std::cout << "Enter a complex number in format: (real,imaginary)\n";
    std::cout << "Example: (4.2,3.3i)\n> ";

    std::cin >> c;

    if (std::cin.fail())
    {
        RUN_TEST("Input failed", false);
    }
    else
    {
        RUN_TEST("Your complex number: " << c, true);
    }
}