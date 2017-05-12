# EasyTest.Ue4

make Unreal Automation Test EASY

## How To Install

add **EasyTest.h** file to your games Source folder. 

## How To Use

### simple function test

you can add simple funcion test by this macro. test function must return bool type(true = pass, false = fail).

```C++
bool testFunc(int a, int b)
{
    return a + b == b + a;
}

//add simple automation test.
//SIMPLE_FUNCTION_TEST(testName, testFunction, function arguments...);
SIMPLE_FUNCTION_TEST(testCase1, testFunc, 1, 2); //this macro add "testCase1" test pass when testFunc(1,2) return true
```

### function test

you can add complex function test by this macro. test function must return bool type(true = pass, false = fail).

```C++
bool testFunc(int a, int b)
{
    return a + b == b + a;
}

FUNCTION_TEST(complexTest, testFunc) // add "complextTest" test "testFunc" function to unreal engine.
{
    ADD_TEST_CASE(test1, 1, 2); // test1 : is testFunc(1,2) return true
    ADD_TEST_CASE(test2, 2, 3); // test2 : is testFunc(2,3) return true
    ADD_TEST_CASE(test3, 5, 6); // test3 : is testFunc(5,6) return true
}
```