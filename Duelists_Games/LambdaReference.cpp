//#include <iostream>
//#include <functional>
//
//int DoSomethingWithCallback (std::function<void()> callback)
//{
//	//Do stuff 
//	int x = 10;
//	int y = 20;
//
//	callback();
//	return x * y + 15;
//
//}
//
//void PrintToConsole()
//{
//
//	std::cout << "Lambda Callback" << std::endl;
//
//}
//
//int main ()
//{
//
//	DoSomethingWithCallback([](){std::cout << "Lambda Callback" << std::endl; });
//
//	return 0;
//}


//#include <iostream>
//
// 
// 
// 
//int main()
//{
//	// Capture by Value[=]:
//
//	int x = 5;
//	int y = 10;
//
//	auto lambda = [=]() {
//		// This captures x and y by value (makes copies)
//		std::cout << "x: " << x << " y: " << y << std::endl;
//		};
//
//	// Even if x or y changes outside the lambda, the lambda still sees the original values (5 and 10)
//	x = 20;
//	y = 30;
//
//	lambda(); // Output: x: 5 y: 10
//
//
//	//Capture by Reference[&]:
//
//	int x = 5;
//	int y = 10;
//
//	auto lambda = [x, &y]() {
//		// x is captured by value (copy), y is captured by reference
//		std::cout << "x: " << x << " y: " << y << std::endl;
//		};
//
//	// Changing y will affect what the lambda sees, but changing x won't
//	x = 20;
//	y = 30;
//
//	lambda(); // Output: x: 5 y: 30
//
//
//
//
//	//Mixed Capturing :
//
//	int x = 5;
//	int y = 10;
//
//	auto lambda = [x, &y]() {
//		// x is captured by value (copy), y is captured by reference
//		std::cout << "x: " << x << " y: " << y << std::endl;
//		};
//
//	// Changing y will affect what the lambda sees, but changing x won't
//	x = 20;
//	y = 30;
//
//	lambda(); // Output: x: 5 y: 30
//
//	return 0;
//}



//[]: No variables are captured.
//[=] : Captures all variables from the surrounding scope by value.
//[&] : Captures all variables from the surrounding scope by reference.
//[x] : Captures only the variable x by value.
//[&x] : Captures only the variable x by reference