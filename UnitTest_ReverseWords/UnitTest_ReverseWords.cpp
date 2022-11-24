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
		
		// Example test given
		TEST_METHOD(ReverseWords_FoxJumpNoChars_JumpFox)
		{
			char string[] = "The quick brown fox jumps over the lazy dog";
			char expectedResult[] = "dog lazy the over jumps fox brown quick The";
			printf("Original String: %s\n", string);

			ReverseWords(string);

			Assert::AreEqual(expectedResult, string);

			printf("Reversed String: %s\n\n\n", string);
		}

		//simple test
		TEST_METHOD(ReverseWords_SimpleCase_WorldHello)
		{
			char string[] = "Hello world";
			char expectedResult[] = "world Hello";
			printf("Original String: %s\n", string);

			ReverseWords(string);

			Assert::AreEqual(expectedResult, string);

			printf("Reversed String: %s\n\n\n", string);
		}

		//no whitespace
		TEST_METHOD(ReverseWords_NoWhiteSpace_SameResult)
		{
			char string[] = "Supercalifragilisticexpialidocious";
			char expectedResult[] = "Supercalifragilisticexpialidocious";
			printf("Original String: %s\n", string);

			ReverseWords(string);

			Assert::AreEqual(expectedResult, string);

			printf("Reversed String: %s\n\n\n", string);
		}

		//all whitespace
		TEST_METHOD(ReverseWords_AllWhiteSpace_SameResult)
		{
			char string[] = "                   ";
			char expectedResult[] = "                   ";
			printf("Original String: %s\n", string);

			ReverseWords(string);

			Assert::AreEqual(expectedResult, string);

			printf("Reversed String: %s\n\n\n", string);
		}

		//leading and end extra whitespace
		TEST_METHOD(ReverseWords_LeadingWhiteSpace_WhiteSpaceKept)
		{
			char string[] = "   Hello  World ";
			char expectedResult[] = " World  Hello   ";
			printf("Original String: %s\n", string);

			ReverseWords(string);

			Assert::AreEqual(expectedResult, string);

			printf("Reversed String: %s\n\n\n", string);
		}

		//punctuation
		TEST_METHOD(ReverseWords_HasPunctuation_ReverseOrder)
		{
			char string[] = "That's what she, said.";
			char expectedResult[] = "said. she, what That's";
			printf("Original String: %s\n", string);

			ReverseWords(string);

			Assert::AreEqual(expectedResult, string);

			printf("Reversed String: %s\n\n\n", string);
		}

		//multiple sentences
		TEST_METHOD(ReverseWords_MultipleSentences_ReverseOrder)
		{
			char string[] = "Sierra Nevada Corporation. Or SNC.";
			char expectedResult[] = "SNC. Or Corporation. Nevada Sierra";
			printf("Original String: %s\n", string);

			ReverseWords(string);

			Assert::AreEqual(expectedResult, string);

			printf("Reversed String: %s\n\n\n", string);
		}


		// With special characters
		TEST_METHOD(ReverseWords_FoxJumpWithChars_JumpFoxWithChars)
		{
			char string[] = "The quick brown, fox jumps over the lazy dog!";
			char expectedResult[] = "dog! lazy the over jumps fox brown, quick The";
			printf("Original String: %s\n", string);

			ReverseWords(string);

			Assert::AreEqual(expectedResult, string);

			printf("Reversed String: %s\n\n\n", string);
		}
	};
}
