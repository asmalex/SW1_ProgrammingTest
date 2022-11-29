# Programming Test
[![Build and Test](https://github.com/asmalex/SW1_ProgrammingTest/actions/workflows/msbuild.yml/badge.svg)](https://github.com/asmalex/SW1_ProgrammingTest/actions/workflows/msbuild.yml)
<!--[![Release](/github/v/release/asmalex/SW1_ProgrammingTest)]-->

This repo will automatically trigger an MSBuild and execute 8 unit tests against anything checked into the main branch. There are three sub-problems: Duplicate List, Sequence Buffer, and Reverse Words. A description of the sub-problems and their solution (as well as assumptions) is documented below:

## Reverse Words
Reverses the words of a sentence - preserving whitespace and punctuation.

##### How it Works
Uses two iterators to loop through the orignal string backwards (iterator j) and construct a new string (iterator i). This is similar to the 'sliding window' pointer technique.

##### Alternate Implementations
Using a stack datastructure, the FILO property (first-in-last-out) could be exploited to re-construct the words backwards. I like this implementation if we had multiple sentences in a string, or where punctuation wasn't preserved with the original word. The branch ReverseString_Stack contains coe for this implementation, but it was abandoned in favor of the simpler (and memory lean) implementation above.

## Duplicate List

##### How it Works
Places copies inline thereby preserving the reference pointer. For example: given the original list a=>b
a=>A'=>b=>B'

1. A' is a copy of a, and B' is a copy of B. A' and B' have reference pointers of null
2. Because we are guaranteed that the next node of the list is a copy of the original node, we can use this property to resolve the references by running:
3. Resolve the references with `newNode->reference = originalNode->reference->next`

##### Alternate Implementations
Using a dictionary, it would be possible to create a deep-copy in 2 passes:
1. Loop through the original list - copying the nodes and adding the reference into a dictionary. The key of the dictionary is the current node number, and the value is reference node.
2. Loop through the copied list - build the reference pointer using the relationships from the dictionary
The branch Reversestring_Loop contains code for this implementation, and while it was my original approach, it was abandoned in favor or the simpler implementation documented above.



## Sequence Buffer
Sequences are stored in a doubly-linked list.




## Installation
•	Clone contents into your project dev folder.
•	Build project (Cntr+Shift+B)
•	That's it!
•	To run a unit test locally on VS 2019 can be done from the menu select Window - > General -> Test Explorer
•	You should see the unit tests in the test explorer. You can run a unit test by double-clicking it

Note: this was tested on Visual Studio 2019 Version 16.11.5 on Windows 10 v10.0.19044. It'll likely work on other versions as well. 

## Unit Tests
The following unit tests are automatically executed and run against any new build:

Input String| Expected Result
------------ | -------------
"" |	""
"                   " | "                   "
"Supercalifragilisticexpialidocious" | "Supercalifragilisticexpialidocious"
"Hello world" |	"world Hello"
"   Hello  World " | " World  Hello   "
"The quick brown fox jumps over the lazy dog" | "dog lazy the over jumps fox brown quick The"
"Sierra Nevada Corporation. Or SNC." | "SNC. Or Corporation. Nevada Sierra"
