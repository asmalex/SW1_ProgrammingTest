#include "pch.h"
#include "stdio.h"
#include "CppUnitTest.h"
#include "../ReverseWords/ReverseWords.c"
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestReverseWords
{
	TEST_CLASS(UnitTestReverseWords)
	{
	public:
		
		TEST_METHOD(BaseTest)
		{
			char string[] = "The quick brown fox jumps over the lazy dog";
			char expectedResult[] = "dog lazy the over jumps fox brown quick The";
			printf("Original String: %s\n", string);

			ReverseWords(string);

			Assert::AreEqual(expectedResult, string);

			printf("Reversed String: %s\n\n\n", string);

		}
	};
}