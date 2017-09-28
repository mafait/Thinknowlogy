Thinknowlogy (Thinking - Knowledge - Technology)

----------------------------------------------------------------------------
How to start:
----------------------------------------------------------------------------
• Unpack the .zip file in a folder of your choice;
• The application comes in two interface types: Java GUI and textual console interface (C++);
• The source code of both versions (Java as well as C++) is included;
• Both applications have the same functionality, but the Java GUI comes with	handy demo buttons;
• The example files can be found in the "data/examples" subfolder.

----------------------------------------------------------------------------
Thinknowlogy2017r2.jar:
----------------------------------------------------------------------------
• This is a Java application. It can be started by double-clicking on the file;
• The GUI (Graphical User Interface) has handy demo buttons to guide you;
• This software uses Java. In order to install or upgrade the Java Runtime
	Environment, go to http://java.com/ and follow the instructions;
• See the included screenshots for extra guidance about the implemented grammar.

----------------------------------------------------------------------------
For experts only - please use the Java version.exe:
----------------------------------------------------------------------------
• First of all, the C++ version is just a bonus. It has a textual console interface.
	So, you'll have to type the sentences by yourself. See the included
	screenshots for guidance about the implemented grammar;
• I have only used standard string functions in C++ instead of wide-character
	string functions. As a consequence, the C++ executable fails to read Spanish
	and French files, because of special characters 'ñ' and 'ç' in the file name.
	However, a Linux Native C++ executable has no problems to read such files;
• But an native Linux C++ executable still has problems to read Spanish and
	French files containing characters '¿' and 'É'. In the regression test files, I
	have put "#Java#" in front of each test file line, in order to skip the test files
	containing these characters. In this way, my native Linux C++ executable
	runs all four languages under Linux, except for the out-commented test files
	in Spanish and French.

----------------------------------------------------------------------------
How to create a Linux Native C++ executable:
----------------------------------------------------------------------------
In subfolder "source" you will find a text file that describes how a Linux Native
C++ executable can be compiled under Linux from the C++ sources.

----------------------------------------------------------------------------
Known issue:
----------------------------------------------------------------------------
I don't speak Spanish, nor French. I've tried to implement these languages the best I could.
But I am aware that the implementation will contain mistranslations and misinterpretations.
Please contact me if you find any problem with the Spanish or French language.

Enjoy playing around with this application. Be brave enough to read the design paper
and the fundamental paper. Have a peek in the source code. And above all: Have fun.

Menno Mafait.
http://mafait.org/contact/
https://nl.linkedin.com/in/mennomafait