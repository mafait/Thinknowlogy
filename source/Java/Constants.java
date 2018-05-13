/*	Class:		Constants
 *	Purpose:	To define constants
 *	Version:	Thinknowlogy 2018r1 (ShangDi 上帝)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at http://mafait.org/contact/
 *************************************************************************/
/*	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this program; if not, write to the Free Software Foundation, Inc.,
 *	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *************************************************************************/

class Constants
	{
	// Version constants
	protected static final String PRODUCT_NAME =												"Thinknowlogy";
	protected static final String VERSION_NAME =												"2018r1 (ShangDi 上帝)";

	// Console constants
	protected static final short CONSOLE_SLEEP_TIME =											10;		// in milliseconds

	protected static final short CONSOLE_BORDER_SIZE =											2;		// in pixels
	protected static final short CONSOLE_BUTTON_PANE_HEIGHT =									30;		// in pixels
	protected static final short CONSOLE_ERROR_PANE_HEIGHT =									400;	// in pixels
	protected static final short CONSOLE_ERROR_PANE_WIDTH =										600;	// in pixels
	protected static final short CONSOLE_MAXIMUM_FRAME_WIDTH_AT_STARTUP =						1280;	// in pixels
	protected static final short CONSOLE_TOP_BOTTOM_SPACE =										75;		// in pixels (space between top of screen and top of GUI and between bottom of GUI and bottom of screen)
	protected static final short CONSOLE_MAX_NUMBER_OF_SUBMENU_BUTTONS =						50;

	protected static final short CONSOLE_SUBMENU_INIT =											0;
	protected static final short CONSOLE_SUBMENU_PROGRAMMING =									1;
	protected static final short CONSOLE_SUBMENU_PROGRAMMING_CONNECT4 =							2;
	protected static final short CONSOLE_SUBMENU_PROGRAMMING_TOWER_OF_HANOI =					3;
	protected static final short CONSOLE_SUBMENU_REASONING =									4;
	protected static final short CONSOLE_SUBMENU_REASONING_FAMILY_DEFINITIONS =					5;
	protected static final short CONSOLE_SUBMENU_REASONING_FAMILY_CONFLICTS =					6;
	protected static final short CONSOLE_SUBMENU_REASONING_FAMILY_JUSTIFICATION_REPORT =		7;
	protected static final short CONSOLE_SUBMENU_REASONING_FAMILY_QUESTIONS =					8;
	protected static final short CONSOLE_SUBMENU_REASONING_FAMILY_DISPLAY_INFORMATION =			9;
	protected static final short CONSOLE_SUBMENU_CHANGE_LANGUAGE =								10;
	protected static final short CONSOLE_SUBMENU_CHANGE_FONT =									11;
	protected static final short CONSOLE_SUBMENU_HELP =											12;

	protected static final short CONSOLE_DEFAULT_FONT_SIZE =									13;
	protected static final short CONSOLE_MINIMUM_FONT_SIZE =									8;
	protected static final short CONSOLE_MAXIMUM_FONT_SIZE =									64;

	// File constants
	protected static final String FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING =					"data/examples/";
	protected static final String FILE_DATA_GRAMMAR_DIRECTORY_NAME_STRING =						"data/grammar/";
	protected static final String FILE_DATA_INFO_DIRECTORY_NAME_STRING =						"data/info/";
	protected static final String FILE_DATA_INTERFACE_DIRECTORY_NAME_STRING =					"data/interface/";
	protected static final String FILE_DATA_STARTUP_DIRECTORY_NAME_STRING =						"data/startup/";
	protected static final String FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING =		"data/regression/output/";
	protected static final String FILE_DATA_REGRESSION_TEST_REFERENCE_DIRECTORY_NAME_STRING =	"data/regression/reference/";
	protected static final String FILE_SOURCE_DIRECTORY_NAME_STRING =							"source/Java/";

	protected static final String FILE_STARTUP_NAME_STRING =									"startup";
	protected static final String FILE_EXTENSION_STRING =										".txt";
	protected static final String FILE_UTF_8_ENCODING_STRING =									"UTF-8";
	protected static final String FILE_UTF_8_BOM_STRING =										"\uFEFF";
	protected static final char FILE_UTF_8_BOM_CHAR =											0x3F;

	// Initialization constants
	protected static final short NO_ASSIGNMENT_LEVEL =											0;
	protected static final short NO_ASSUMPTION_LEVEL =											0;
	protected static final short NO_GRAMMAR_LEVEL =												0;
	protected static final short NO_SELECTION_LEVEL =											0;
	protected static final short NO_SOLVE_LEVEL =												0;
	protected static final short NO_WRITE_LEVEL =												0;
	protected static final short NO_LANGUAGE_NR =												0;
	protected static final short NO_ORDER_NR =													0;
	protected static final short NO_USER_NR =													0;

	protected static final short NO_ADJECTIVE_PARAMETER =										0;
	protected static final short NO_ANSWER_PARAMETER =											0;
	protected static final short NO_ASSIGNMENT_PARAMETER =										0;
	protected static final short NO_DEFINITE_ARTICLE_PARAMETER =								0;
	protected static final short NO_GRAMMAR_PARAMETER =											0;
	protected static final short NO_IMPERATIVE_PARAMETER =										0;
	protected static final short NO_INDEFINITE_ARTICLE_PARAMETER =								0;
	protected static final short NO_INTERFACE_PARAMETER =										0;
	protected static final short NO_PREPOSITION_PARAMETER =										0;
	protected static final short NO_QUESTION_PARAMETER =										0;
	protected static final short NO_SOLVE_STRATEGY_PARAMETER =									0;
	protected static final short NO_WORD_PARAMETER =											0;

	protected static final short NO_LIST_NR =													Short.MAX_VALUE;
	protected static final short MAX_LEVEL =													Short.MAX_VALUE;
	protected static final short MAX_ORDER_NR =													Short.MAX_VALUE;
	protected static final short MAX_POSITION =													Short.MAX_VALUE;

	protected static final int NO_CENTER_WIDTH =												0;
	protected static final int NO_COLLECTION_NR =												0;
	protected static final int NO_CONTEXT_NR =													0;
	protected static final int NO_ITEM_NR =														0;
	protected static final int NO_SENTENCE_NR =													0;
	protected static final int NO_SCORE =														0;

	protected static final int MAX_COLLECTION_NR =												Integer.MAX_VALUE;
	protected static final int MAX_CONTEXT_NR =													Integer.MAX_VALUE;
	protected static final int MAX_ITEM_NR =													Integer.MAX_VALUE;
	protected static final int MAX_NUMBER =														Integer.MAX_VALUE;
	protected static final int MAX_PROGRESS =													Integer.MAX_VALUE;
	protected static final int MAX_QUERY_PARAMETER =											Integer.MAX_VALUE;
	protected static final int MAX_SENTENCE_NR =												Integer.MAX_VALUE;
	protected static final int MAX_SCORE =														Integer.MAX_VALUE;
	protected static final int WINNING_SCORE =													Integer.MAX_VALUE;

	// Error string constants

	protected static final String NO_INTERFACE_STRING_FOUND_STRING =							"{no interface string found}";
	protected static final String NO_LANGUAGE_NAME_FOUND_STRING =								"{no language name found}";
	protected static final String NO_LANGUAGE_WORD_FOUND_STRING =								"{no language word found}";
	protected static final String NO_USER_NAME_FOUND_STRING =									"{no user name found}";

	// Input-output error constants
	protected static final String INPUT_OUTPUT_ERROR_INTERNAL_TITLE_STRING =					"Internal error";
	protected static final String INPUT_OUTPUT_ERROR_CURRENT_ID_STRING =						" with current item id: ";

	protected static final String INPUT_OUTPUT_ERROR_CLASS_STRING =								"Class: ";
	protected static final String INPUT_OUTPUT_ERROR_PARENT_CLASS_STRING =						" / parent class ";
	protected static final String INPUT_OUTPUT_ERROR_METHOD_STRING =							".\nMethod: ";
	protected static final String INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME =					"<init>";
	protected static final String INPUT_OUTPUT_ERROR_METHOD_WORD_START_STRING =					" of word \"";
	protected static final String INPUT_OUTPUT_ERROR_METHOD_WORD_END_STRING =					"\"";
	protected static final String INPUT_OUTPUT_ERROR_METHOD_LIST_START_STRING =					" in list <";
	protected static final String INPUT_OUTPUT_ERROR_METHOD_LIST_END_STRING =					">";
	protected static final String INPUT_OUTPUT_ERROR_MESSAGE_START_STRING =						".\nError: ";
	protected static final String INPUT_OUTPUT_ERROR_MESSAGE_END_STRING =						".\n\n";

	// Input-output prompt constants
	protected static final short INPUT_OUTPUT_PROMPT_READ =										0;
	protected static final short INPUT_OUTPUT_PROMPT_WRITE =									1;
	protected static final short INPUT_OUTPUT_PROMPT_WRITE_INDENTED =							2;
	protected static final short INPUT_OUTPUT_PROMPT_INFO =										3;
	protected static final short INPUT_OUTPUT_PROMPT_NOTIFICATION =								4;
	protected static final short INPUT_OUTPUT_PROMPT_WARNING =									5;
	protected static final short INPUT_OUTPUT_PROMPT_WARNING_INDENTED =							6;
	protected static final short INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY =						7;
	protected static final short INPUT_OUTPUT_PROMPT_QUERY =									8;

	protected static final String INPUT_OUTPUT_PROMPT_READ_STRING =								"> ";
	protected static final String INPUT_OUTPUT_PROMPT_WRITE_STRING =							"< ";
	protected static final String INPUT_OUTPUT_PROMPT_WRITE_INDENTED_STRING =					"<	- ";
	protected static final String INPUT_OUTPUT_PROMPT_INFO_STRING =								"";
	protected static final String INPUT_OUTPUT_PROMPT_NOTIFICATION_STRING =						"* ";
	protected static final String INPUT_OUTPUT_PROMPT_WARNING_STRING =							"! ";
	protected static final String INPUT_OUTPUT_PROMPT_WARNING_INDENTED_STRING =					"!	- ";
	protected static final String INPUT_OUTPUT_PROMPT_QUERY_STRING =							"? ";

	protected static final String INPUT_OUTPUT_SKIP_COMMENT_STRING =							"#C++#";
	protected static final String INPUT_OUTPUT_STRIP_COMMENT_STRING =							"#Java#";

	// Character constants
	protected static final char SPACE_CHAR =													' ';
	protected static final char BACK_SPACE_CHAR =												'\b';
	protected static final char BELL_CHAR =														'\u0007';
	protected static final char NEW_LINE_CHAR =													'\n';
	protected static final char CARRIAGE_RETURN_CHAR =											'\r';
	protected static final char TAB_CHAR =														'\t';

	// Symbol constants
	private static final char 	SYMBOL_HASH =													'#';
	private static final char	SYMBOL_SWUNG_DASH =												'~';
	private static final char 	SYMBOL_QUOTE =													'\'';
	private static final char 	SYMBOL_BACK_QUOTE =												'`';

	protected static final char SYMBOL_COMMA =													',';
	protected static final char SYMBOL_COLON =													'.';
	protected static final char SYMBOL_SEMI_COLON =												';';
	protected static final char SYMBOL_DOUBLE_COLON =											':';
	protected static final char SYMBOL_EXCLAMATION_MARK =										'!';
	protected static final char SYMBOL_SPANISH_INVERTED_EXCLAMATION_MARK =						'¡';
	protected static final char SYMBOL_QUESTION_MARK =											'?';
	protected static final char SYMBOL_SPANISH_INVERTED_QUESTION_MARK =							'¿';
	protected static final char SYMBOL_PLUS =													'+';
	protected static final char SYMBOL_MINUS =													'-';
	protected static final char SYMBOL_UNDERSCORE =												'_';
	protected static final char SYMBOL_APOSTROPHE =												'\'';
	protected static final char SYMBOL_EQUALITY =												'=';
	protected static final char SYMBOL_PIPE =													'|';
	protected static final char SYMBOL_AMPERSAND =												'&';
	protected static final char SYMBOL_AT_SIGN =												'@';
	protected static final char SYMBOL_ASTERISK =												'*';
	protected static final char SYMBOL_PERCENT =												'%';
	protected static final char SYMBOL_DOLLAR =													'$';
	protected static final char SYMBOL_SLASH =													'/';
	protected static final char SYMBOL_BACK_SLASH =												'\\';
	protected static final char SYMBOL_DOUBLE_QUOTE =											'"';
	protected static final char SYMBOL_OPEN_ROUNDED_BRACKET =									'(';
	protected static final char SYMBOL_CLOSE_ROUNDED_BRACKET =									')';
	protected static final char SYMBOL_OPEN_CURVED_BRACKET =									'{';
	protected static final char SYMBOL_CLOSE_CURVED_BRACKET =									'}';
	protected static final char SYMBOL_OPEN_HOOKED_BRACKET =									'<';
	protected static final char SYMBOL_CLOSE_HOOKED_BRACKET =									'>';
	protected static final char SYMBOL_OPEN_SQUARE_BRACKET =									'[';
	protected static final char SYMBOL_CLOSE_SQUARE_BRACKET =									']';

	// Grammar constants
	protected static final char GRAMMAR_CHOICE_START =											SYMBOL_OPEN_CURVED_BRACKET;
	protected static final char GRAMMAR_CHOICE_END =											SYMBOL_CLOSE_CURVED_BRACKET;
	protected static final char GRAMMAR_OPTION_START =											SYMBOL_OPEN_SQUARE_BRACKET;
	protected static final char GRAMMAR_OPTION_END =											SYMBOL_CLOSE_SQUARE_BRACKET;
	protected static final char GRAMMAR_OPTION_READ_NOT_WRITE_START =							SYMBOL_OPEN_ROUNDED_BRACKET;
	protected static final char GRAMMAR_OPTION_READ_NOT_WRITE_END =								SYMBOL_CLOSE_ROUNDED_BRACKET;

	// Query constants
	protected static final char COMMENT_CHAR =													SYMBOL_HASH;
	protected static final char GRAMMAR_WORD_DEFINITION_CHAR =									SYMBOL_DOLLAR;

	protected static final char QUERY_CHAR =													SYMBOL_SWUNG_DASH;
	protected static final char QUERY_COUNT_CHAR =												SYMBOL_HASH;
	protected static final char QUERY_SELECTING_ATTACHED_JUSTIFICATIONS_CHAR =					SYMBOL_ASTERISK;
	protected static final char QUERY_NO_LIST_CHAR =											SYMBOL_QUESTION_MARK;
	protected static final char QUERY_WORD_TYPE_CHAR =											SYMBOL_AT_SIGN;
	protected static final char QUERY_PARAMETER_CHAR =											SYMBOL_DOUBLE_COLON;
	protected static final char QUERY_ACTIVE_CHAR =												SYMBOL_PLUS;
	protected static final char QUERY_INACTIVE_CHAR	=											SYMBOL_EQUALITY;
	protected static final char QUERY_ARCHIVED_CHAR =											SYMBOL_DOLLAR;
	protected static final char QUERY_REPLACED_CHAR =											SYMBOL_PERCENT;
	protected static final char QUERY_DELETED_CHAR =											SYMBOL_AMPERSAND;
	protected static final char QUERY_SEPARATOR_CHAR =											SYMBOL_COMMA;
	protected static final char QUERY_ITEM_START_CHAR =											SYMBOL_OPEN_ROUNDED_BRACKET;
	protected static final char QUERY_ITEM_END_CHAR =											SYMBOL_CLOSE_ROUNDED_BRACKET;
	protected static final char QUERY_ITEM_SENTENCE_NR_START_CHAR =								SYMBOL_OPEN_CURVED_BRACKET;
	protected static final char QUERY_ITEM_SENTENCE_NR_END_CHAR =								SYMBOL_CLOSE_CURVED_BRACKET;
	protected static final char QUERY_REF_ITEM_START_CHAR =										SYMBOL_OPEN_SQUARE_BRACKET;
	protected static final char QUERY_REF_ITEM_END_CHAR =										SYMBOL_CLOSE_SQUARE_BRACKET;
	protected static final char QUERY_LIST_START_CHAR =											SYMBOL_OPEN_HOOKED_BRACKET;
	protected static final char QUERY_LIST_END_CHAR =											SYMBOL_CLOSE_HOOKED_BRACKET;
	protected static final char QUERY_WORD_START_CHAR =											SYMBOL_QUOTE;
	protected static final char QUERY_WORD_END_CHAR =											SYMBOL_QUOTE;
	protected static final char QUERY_WORD_REFERENCE_START_CHAR =								SYMBOL_BACK_QUOTE;
	protected static final char QUERY_WORD_REFERENCE_END_CHAR =									SYMBOL_BACK_QUOTE;
	protected static final char QUERY_STRING_START_CHAR =										SYMBOL_DOUBLE_QUOTE;
	protected static final char QUERY_STRING_END_CHAR =											SYMBOL_DOUBLE_QUOTE;

	protected static final char TEXT_BELL_CHAR =												'a';
	protected static final char TEXT_BACK_SPACE_CHAR =											'b';
	protected static final char TEXT_NEW_LINE_CHAR =											'n';
	protected static final char TEXT_TAB_CHAR =													't';

	// String constants
	protected static final String BACK_SLASH_STRING =											"\\";
	protected static final String CARRIAGE_RETURN_NEW_LINE_STRING =								"\r\n";
	protected static final String CHANGE_LANGUAGE_STRING =										"~";
	protected static final String DOUBLE_COLON_STRING =											":";
	protected static final String EMPTY_STRING =												"";
	protected static final String NEW_LINE_STRING =												"\n";
	protected static final String SLASH_STRING =												"/";
	protected static final String SPACE_STRING =												" ";

	// Query constants
	protected static final String QUERY_ITEM_START_STRING =										"(";
	protected static final String QUERY_LIST_START_STRING =										"<";
	protected static final String QUERY_SEPARATOR_SPACE_STRING =								", ";
	protected static final String QUERY_SEPARATOR_STRING =										",";
	protected static final String QUERY_WORD_TYPE_STRING =										EMPTY_STRING +	QUERY_WORD_TYPE_CHAR;

	// Result constants
	protected static final byte RESULT_OK =														1;
	protected static final byte RESULT_ERROR =													0;
	protected static final byte RESULT_SYSTEM_ERROR =											-1;

	// Screen constants
	protected static final short TAB_LENGTH =													8;


	// Administrator initialization constants
	protected static final String ADMIN_USER_NAME_STRING =										"Admin";

	protected static final short ADMIN_FILE_LIST =												0;
	protected static final short ADMIN_READ_LIST =												1;
	protected static final short ADMIN_SCORE_LIST =												2;
	protected static final short ADMIN_WORD_LIST =												3;
	protected static final short ADMIN_CONDITION_LIST =											4;		// SelectionList
	protected static final short ADMIN_ACTION_LIST =											5;		// SelectionList
	protected static final short ADMIN_ALTERNATIVE_LIST =										6;		// SelectionList
	protected static final short NUMBER_OF_ADMIN_LISTS =										7;

	protected static final char ADMIN_FILE_LIST_SYMBOL =										'F';
	protected static final char ADMIN_READ_LIST_SYMBOL =										'R';
	protected static final char ADMIN_SCORE_LIST_SYMBOL =										'S';
	protected static final char ADMIN_WORD_LIST_SYMBOL =										'W';
	protected static final char ADMIN_CONDITION_LIST_SYMBOL =									'X';	// SelectionList
	protected static final char ADMIN_ACTION_LIST_SYMBOL =										'Y';	// SelectionList
	protected static final char ADMIN_ALTERNATIVE_LIST_SYMBOL =									'Z';	// SelectionList

	// Word initialization constants
	protected static final short WORD_ASSIGNMENT_LIST =											0;		// SpecificationList
	protected static final short WORD_COLLECTION_LIST =											1;
	protected static final short WORD_GENERALIZATION_LIST =										2;
	protected static final short WORD_INTERFACE_LIST =											3;
	protected static final short WORD_JUSTIFICATION_LIST =										4;
	protected static final short WORD_GRAMMAR_LIST =											5;
	protected static final short WORD_MULTIPLE_WORD_LIST =										6;
	protected static final short WORD_WRITE_LIST =												7;
	protected static final short WORD_SPECIFICATION_LIST =										8;
	protected static final short WORD_TYPE_LIST =												9;
	protected static final short WORD_CONTEXT_LIST = 											10;
	protected static final short NUMBER_OF_WORD_LISTS =											11;

	protected static final char WORD_ASSIGNMENT_LIST_SYMBOL =									'a';	// SpecificationList
	protected static final char WORD_COLLECTION_LIST_SYMBOL =									'c';
	protected static final char WORD_GENERALIZATION_LIST_SYMBOL =								'g';
	protected static final char WORD_INTERFACE_LIST_SYMBOL =									'i';
	protected static final char WORD_JUSTIFICATION_LIST_SYMBOL =								'j';
	protected static final char WORD_GRAMMAR_LIST_SYMBOL =										'l';
	protected static final char WORD_MULTIPLE_WORD_LIST_SYMBOL =								'm';
	protected static final char WORD_WRITE_LIST_SYMBOL =										'r';
	protected static final char WORD_SPECIFICATION_LIST_SYMBOL =								's';
	protected static final char WORD_TYPE_LIST_SYMBOL =											'w';
	protected static final char WORD_CONTEXT_LIST_SYMBOL =										'x';

	// Other initialization constants
	protected static final short MAX_GRAMMAR_LEVEL =											50;		// Maximum depth of parsing grammar calls
	protected static final short MAX_REASONING_ITERATIONS =										50;		// Maximum number of reasoning iterations
	protected static final short MAX_SELECTION_ITERATIONS =										250;	// Maximum number of selection iterations

	protected static final short NUMBER_OF_ASSUMPTION_LEVELS =									3;		// Probably, possibly and maybe
	protected static final short NUMBER_OF_PHONETIC_VOWELS = 									4;

	protected static final char PHONETIC_VOWEL[] = { 'a',
													'e',
													'i',
													'o' };

	// Justification types
	protected static final short JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION =									0;
	protected static final short JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION =	1;
	protected static final short JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION =			2;
	protected static final short JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION =					3;
	protected static final short JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION =							4;

	protected static final short JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION =								5;
	protected static final short JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION =							6;
	protected static final short JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION =								7;
	protected static final short JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION =										8;
	protected static final short JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION =		9;
	protected static final short JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION =						10;
	protected static final short JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION =				11;
	protected static final short JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION =								12;

	protected static final short JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION =								13;
	protected static final short JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION =							14;
	protected static final short JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION =								15;
	protected static final short JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION =										16;
	protected static final short JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION =		17;
	protected static final short JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION =						18;
	protected static final short JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION =				19;
	protected static final short JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION =								20;

	protected static final short JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION =						21;


	//	Word types
	protected static final short NO_WORD_TYPE_NR =										0;
	protected static final short WORD_TYPE_SYMBOL =										1;
	protected static final short WORD_TYPE_NUMERAL =									2;
	protected static final short WORD_TYPE_LETTER_SMALL =								3;
	protected static final short WORD_TYPE_LETTER_CAPITAL =								4;
	protected static final short WORD_TYPE_PROPER_NOUN =								5;
	protected static final short WORD_TYPE_ADJECTIVE =									6;
	protected static final short WORD_TYPE_ADVERB =										7;
	protected static final short WORD_TYPE_ANSWER =										8;
	protected static final short WORD_TYPE_ARTICLE =									9;
	protected static final short WORD_TYPE_CONJUNCTION =								10;
	protected static final short WORD_TYPE_NOUN_SINGULAR =								11;
	protected static final short WORD_TYPE_NOUN_PLURAL =								12;
	protected static final short WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_SUBJECTIVE =		13;
	protected static final short WORD_TYPE_PERSONAL_PRONOUN_SINGULAR_OBJECTIVE =		14;
	protected static final short WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR =				15;
	protected static final short WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR =				16;
	protected static final short WORD_TYPE_PERSONAL_PRONOUN_PLURAL_SUBJECTIVE =			17;
	protected static final short WORD_TYPE_PERSONAL_PRONOUN_PLURAL_OBJECTIVE =			18;
	protected static final short WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL =				19;
	protected static final short WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL =					20;
	protected static final short WORD_TYPE_PREPOSITION =								21;
	protected static final short WORD_TYPE_VERB_SINGULAR =								22;
	protected static final short WORD_TYPE_VERB_PLURAL =								23;
	protected static final short WORD_TYPE_SELECTION_WORD =								24;
	protected static final short WORD_TYPE_TEXT =										25;
	protected static final short NUMBER_OF_WORD_TYPES =									26;


	//	Word Parameters

	//	Symbols
	protected static final short WORD_PARAMETER_SYMBOL_COMMA =												100;
	protected static final short WORD_PARAMETER_SYMBOL_COLON =												101;
	protected static final short WORD_PARAMETER_SYMBOL_EXCLAMATION_MARK =									102;
	protected static final short WORD_PARAMETER_SYMBOL_QUESTION_MARK =										103;
	protected static final short WORD_PARAMETER_SYMBOL_CHINESE_NEGATIVE_SEPARATOR =							104;
	protected static final short WORD_PARAMETER_SYMBOL_CHINESE_QUESTION_SEPARATOR =							105;
	protected static final short WORD_PARAMETER_SYMBOL_SPANISH_INVERTED_EXCLAMATION_MARK =					106;
	protected static final short WORD_PARAMETER_SYMBOL_SPANISH_INVERTED_QUESTION_MARK =						107;

	//	Numerals
	protected static final short WORD_PARAMETER_NUMERAL_BOTH =												200;
	protected static final short WORD_PARAMETER_NUMERAL_CHINESE_ALL =										201;

	//	Small letters

	//	Capital letters

	// Proper nouns

	//	Adjectives
	protected static final short WORD_PARAMETER_ADJECTIVE_ASSIGNED =										600;
	protected static final short WORD_PARAMETER_ADJECTIVE_BUSY =											601;
	protected static final short WORD_PARAMETER_ADJECTIVE_DEFENSIVE =										602;
	protected static final short WORD_PARAMETER_ADJECTIVE_DONE =											603;
	protected static final short WORD_PARAMETER_ADJECTIVE_EMPTY =											604;
	protected static final short WORD_PARAMETER_ADJECTIVE_EXCLUSIVE =										605;
	protected static final short WORD_PARAMETER_ADJECTIVE_FEMININE =										606;
	protected static final short WORD_PARAMETER_ADJECTIVE_INVERTED =										607;
	protected static final short WORD_PARAMETER_ADJECTIVE_MASCULINE =										608;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_BIGGER =								609;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_EARLIER =								610;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_EQUAL =								611;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_HIGHER =								612;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_LARGER =								613;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_LATER =								614;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_LESS =									615;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_LOWER =								616;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_MORE =									617;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_SAME =									618;
	protected static final short WORD_PARAMETER_ADJECTIVE_COMPARISON_SMALLER =								619;
	protected static final short WORD_PARAMETER_ADJECTIVE_CALLED_NEUTRAL =									620;
	protected static final short WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_FEMININE =						621;
	protected static final short WORD_PARAMETER_ADJECTIVE_CALLED_SINGULAR_MASCULINE =						622;
	protected static final short WORD_PARAMETER_ADJECTIVE_CHARACTERISTIC =									623;
	protected static final short WORD_PARAMETER_ADJECTIVE_CURRENT_NEUTRAL =									624;
	protected static final short WORD_PARAMETER_ADJECTIVE_CURRENT_FEMININE_MASCULINE =						625;
	protected static final short WORD_PARAMETER_ADJECTIVE_EVEN =											626;
	protected static final short WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL =									627;
	protected static final short WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE =						628;
	protected static final short WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL =										629;
	protected static final short WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE =							630;
//	protected static final short WORD_PARAMETER_ADJECTIVE_NEXT_NEUTRAL =									631;
//	protected static final short WORD_PARAMETER_ADJECTIVE_NEXT_FEMININE_MASCULINE =							632;
	protected static final short WORD_PARAMETER_ADJECTIVE_NO =												633;
	protected static final short WORD_PARAMETER_ADJECTIVE_ODD =												634;
	protected static final short WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL =								635;
	protected static final short WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE =						636;
	protected static final short WORD_PARAMETER_ADJECTIVE_SPECIFIC =										637;
	protected static final short WORD_PARAMETER_ADJECTIVE_CHINESE_DRAW =									638;

	//	Adverbs
	protected static final short WORD_PARAMETER_ADVERB_ASSUMPTION_MAYBE =									700;
	protected static final short WORD_PARAMETER_ADVERB_ASSUMPTION_POSSIBLY =								701;
	protected static final short WORD_PARAMETER_ADVERB_ASSUMPTION_PROBABLY =								702;
	protected static final short WORD_PARAMETER_ADVERB_ANYMORE =											703;
	protected static final short WORD_PARAMETER_ADVERB_AS =													704;
	protected static final short WORD_PARAMETER_ADVERB_NOT =												705;
	protected static final short WORD_PARAMETER_ADVERB_CHINESE_NEVER_AGAIN =								706;
	protected static final short WORD_PARAMETER_ADVERB_FRENCH_PAS =											707;

	// Interjections
	protected static final short WORD_PARAMETER_INTERJECTION_YES =											800;
	protected static final short WORD_PARAMETER_INTERJECTION_NO =											801;

	//	Articles
	protected static final short WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE =			900;
	protected static final short WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE =		901;
	protected static final short WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE =		902;
	protected static final short WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE =		903;
	protected static final short WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE =						904;
	protected static final short WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE =						905;
	protected static final short WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE =						906;
	protected static final short WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE =						907;
	protected static final short WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_NEUTRAL =							908;
	protected static final short WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE =							909;
	protected static final short WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE =							910;
	protected static final short WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_NEUTRAL =							911;
	protected static final short WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE =						912;
	protected static final short WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE =						913;

	//	Conjunctions
	protected static final short WORD_PARAMETER_CONJUNCTION_AND =											1000;
	protected static final short WORD_PARAMETER_CONJUNCTION_OR =											1001;
//	protected static final short WORD_PARAMETER_CONJUNCTION_THAN =											1002;
//	protected static final short WORD_PARAMETER_CONJUNCTION_THAN =											1003;
	protected static final short WORD_PARAMETER_CONJUNCTION_DUTCH_ZOWEL =									1004;
	protected static final short WORD_PARAMETER_CONJUNCTION_DUTCH_ALS =										1005;

	//	Singular / plural nouns
	protected static final short WORD_PARAMETER_NOUN_DEVELOPER =											1100;
	protected static final short WORD_PARAMETER_NOUN_EXPERT =												1101;
	protected static final short WORD_PARAMETER_NOUN_FILE =													1102;
	protected static final short WORD_PARAMETER_NOUN_HEAD =													1103;
	protected static final short WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT =									1104;
	protected static final short WORD_PARAMETER_NOUN_LANGUAGE =												1105;
	protected static final short WORD_PARAMETER_NOUN_MIND =													1106;
	protected static final short WORD_PARAMETER_NOUN_NUMBER =												1107;
	protected static final short WORD_PARAMETER_NOUN_PASSWORD =												1108;
	protected static final short WORD_PARAMETER_NOUN_SOLVE_LEVEL =											1109;
	protected static final short WORD_PARAMETER_NOUN_SOLVE_METHOD =											1110;
	protected static final short WORD_PARAMETER_NOUN_SOLVE_STRATEGY =										1111;
	protected static final short WORD_PARAMETER_NOUN_STARTUP_LANGUAGE =										1112;
	protected static final short WORD_PARAMETER_NOUN_TAIL =													1113;
	protected static final short WORD_PARAMETER_NOUN_TEST_FILE =											1114;
	protected static final short WORD_PARAMETER_NOUN_USER =													1115;
	protected static final short WORD_PARAMETER_NOUN_INFORMATION =											1116;
	protected static final short WORD_PARAMETER_NOUN_PART =													1117;
	protected static final short WORD_PARAMETER_NOUN_VALUE =												1118;
	protected static final short WORD_PARAMETER_NOUN_CHINESE_IS_IT_TRUE =									1119;
	protected static final short WORD_PARAMETER_NOUN_CHINESE_MARSUPIAL =									1120;
	protected static final short WORD_PARAMETER_NOUN_CHINESE_SAILBOAT =										1121;
	protected static final short WORD_PARAMETER_NOUN_CHINESE_TURN =											1122;

	// Singular pronouns
	protected static final short WORD_PARAMETER_SINGULAR_PRONOUN_I_ME_MY_MINE =								1300;
	protected static final short WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_INFORMAL =					1301;
	protected static final short WORD_PARAMETER_SINGULAR_PRONOUN_YOU_YOUR_YOURS_FORMAL =					1302;
	protected static final short WORD_PARAMETER_SINGULAR_PRONOUN_HE_HIM_HIS_HIS =							1303;
	protected static final short WORD_PARAMETER_SINGULAR_PRONOUN_SHE_HER_HER_HERS =							1304;
	protected static final short WORD_PARAMETER_SINGULAR_PRONOUN_IT_ITS_ITS_ITS =							1305;

	// Plural pronouns
	protected static final short WORD_PARAMETER_PLURAL_PRONOUN_WE_US_OUR_OURS_FEMININE =					1700;
	protected static final short WORD_PARAMETER_PLURAL_PRONOUN_WE_US_OUR_OURS_MASCULINE =					1701;
	protected static final short WORD_PARAMETER_PLURAL_PRONOUN_YOU_YOUR_YOURS_FEMININE =					1702;
	protected static final short WORD_PARAMETER_PLURAL_PRONOUN_YOU_YOUR_YOURS_MASCULINE =					1703;
	protected static final short WORD_PARAMETER_PLURAL_PRONOUN_YOU_ALL =									1704;
	protected static final short WORD_PARAMETER_PLURAL_PRONOUN_THEY_THEM_THEIR_THEIRS_FEMININE =			1705;
	protected static final short WORD_PARAMETER_PLURAL_PRONOUN_THEY_THEM_THEIR_THEIRS_MASCULINE =			1706;

	//	Prepositions
	protected static final short WORD_PARAMETER_PREPOSITION_ABOUT =											2100;
	protected static final short WORD_PARAMETER_PREPOSITION_FROM =											2101;
	protected static final short WORD_PARAMETER_PREPOSITION_IN =											2102;
	protected static final short WORD_PARAMETER_PREPOSITION_TO =											2103;
	protected static final short WORD_PARAMETER_PREPOSITION_FOR =											2104;
	protected static final short WORD_PARAMETER_PREPOSITION_OF =											2105;
	protected static final short WORD_PARAMETER_PREPOSITION_CHINESE_VERB_ADD =								2106;
	protected static final short WORD_PARAMETER_PREPOSITION_CHINESE_VERB_MOVE =								2107;
	protected static final short WORD_PARAMETER_PREPOSITION_CHINESE_VERB_REMOVE =							2108;
	protected static final short WORD_PARAMETER_PREPOSITION_FRENCH_A =										2109;

	//	Singular verbs
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IS =											2200;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_WAS =											2201;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_CAN_BE =										2202;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_HAS =											2203;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_HAD =											2204;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD =								2205;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CLEAR =							2206;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY =							2207;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_HELP =								2208;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN =							2209;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE =								2210;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ =								2211;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO =								2212;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE =							2213;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_RESTART =							2214;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SOLVE =							2215;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO =								2216;
	protected static final short WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CHINESE_PUT =						2217;

	//	Plural verbs
	protected static final short WORD_PARAMETER_PLURAL_VERB_ARE =											2300;
	protected static final short WORD_PARAMETER_PLURAL_VERB_WERE =											2301;
	protected static final short WORD_PARAMETER_PLURAL_VERB_CAN_BE =										2302;
	protected static final short WORD_PARAMETER_PLURAL_VERB_HAVE =											2303;
	protected static final short WORD_PARAMETER_PLURAL_VERB_HAD =											2304;

	//	Selection words
	protected static final short WORD_PARAMETER_SELECTION_IF =												2400;
	protected static final short WORD_PARAMETER_SELECTION_THEN =											2401;
	protected static final short WORD_PARAMETER_SELECTION_ELSE =											2402;

	//	Text

	// Feminine proper name ending
	protected static final short WORD_FEMININE_PROPER_NOUN_ENDING =											3000;

	// Masculine proper name ending
	protected static final short WORD_MASCULINE_PROPER_NOUN_ENDING =										3001;

	// Feminine singular noun ending
	protected static final short WORD_FEMININE_SINGULAR_NOUN_ENDING =										3002;

	// Masculine singular noun ending
	protected static final short WORD_MASCULINE_SINGULAR_NOUN_ENDING =										3003;

	//	Plural noun ending
	protected static final short WORD_PLURAL_NOUN_ENDING =													3004;

	// Merged words
	protected static final short WORD_MERGED_WORD =															3005;

	// Assignment nouns
	protected static final short WORD_CHINESE_EXCLUSIVE_NOUN =												3006;


	//	Grammar parameters
	protected static final short GRAMMAR_SENTENCE =															4000;
	protected static final short GRAMMAR_STATEMENT =														4010;
	protected static final short GRAMMAR_STATEMENT_ANSWER =													4011;
	protected static final short GRAMMAR_STATEMENT_ASSIGNMENT =												4012;
	protected static final short GRAMMAR_STATEMENT_SPECIFICATION =											4013;
	protected static final short GRAMMAR_STATEMENT_SPECIFICATION_GENERALIZATION =							4014;
	protected static final short GRAMMAR_QUESTION =															4020;
	protected static final short GRAMMAR_QUESTION_SPECIFICATION =											4021;
	protected static final short GRAMMAR_QUESTION_SPECIFICATION_GENERALIZATION =							4022;
	protected static final short GRAMMAR_SELECTION =														4030;
	protected static final short GRAMMAR_IMPERATIVE =														4040;
	protected static final short GRAMMAR_GENERALIZATION_SPECIFICATION =										5000;
	protected static final short GRAMMAR_GENERALIZATION_PART =												5010;
	protected static final short GRAMMAR_GENERALIZATION_WORD =												5011;
	protected static final short GRAMMAR_LINKED_GENERALIZATION_CONJUNCTION =								5012;
	protected static final short GRAMMAR_SPECIFICATION_PART =												5020;
	protected static final short GRAMMAR_ASSIGNMENT_PART =													5021;
	protected static final short GRAMMAR_SPECIFICATION_WORD =												5022;
	protected static final short GRAMMAR_EXCLUSIVE_SPECIFICATION_CONJUNCTION =								5023;
	protected static final short GRAMMAR_RELATION_PART =													5030;
	protected static final short GRAMMAR_RELATION_WORD =													5031;
	protected static final short GRAMMAR_TEXT =																5040;
	protected static final short GRAMMAR_GENERALIZATION_ASSIGNMENT =										5100;
	protected static final short GRAMMAR_SPECIFICATION_ASSIGNMENT =											5101;
	protected static final short GRAMMAR_RELATION_ASSIGNMENT =												5102;
	protected static final short GRAMMAR_SENTENCE_CONJUNCTION =												5110;
	protected static final short GRAMMAR_VERB =																5120;
	protected static final short GRAMMAR_QUESTION_VERB =													5121;
	protected static final short GRAMMAR_SPECIFICATION_GENERALIZATION_VERB =								5122;
	protected static final short GRAMMAR_SPECIFICATION_GENERALIZATION_QUESTION_VERB =						5123;

	// Console (upper menu)
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_CLEAR_YOUR_MIND =												100;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_RESTART =														101;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_UNDO =														102;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_REDO =														103;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_LOGIN_AS_EXPERT =												104;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_LOGIN_AS_DEVELOPER =											105;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_REGRESSION_TEST =												106;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_STOP_TESTING =												107;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_MORE_EXAMPLES =												108;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_READ_FILE_START =												109;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_READ_THE_TEST_FILE =											110;
	protected static final short INTERFACE_CONSOLE_UPPER_MENU_READ_FILE_END =												111;

	// Console (main menu)
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_CONNECT4 =							200;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_PROGRAMMING_TOWER_OF_HANOI =						201;
	protected static final short INTERFACE_CONSOLE_REASONING_READ_THE_FILE_SCIENTIFIC_CHALLENGE =							202;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_READ_THE_FILE_REASONING_FAMILY =								203;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_PROGRAMMING_SUBMENU =											204;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_REASONING_SUBMENU =											205;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_FAMILY_SUBMENU =												206;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_FAMILY_CONFLICT_SUBMENU =										207;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_FAMILY_JUSTIFICATION_REPORT_SUBMENU =							208;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_FAMILY_QUESTION_SUBMENU =										209;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_FAMILY_DISPLAY_INFORMATION_SUBMENU =							210;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_BACK =															211;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_HELP =															212;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_CHANGE_LANGUAGE =												213;
	protected static final short INTERFACE_CONSOLE_MAIN_MENU_CHANGE_FONT =													214;

	// Console (programming/connect4 sub-menu)
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_A =										300;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_B =										301;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_C =										302;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_D =										303;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_E =										304;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_F =										305;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_MY_SET_IS_G =										306;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_DISPLAY_INFO_ABOUT_THE_SET =						307;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_THE_SOLVE_LEVEL_IS_LOW =							308;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_CONNECT4_THE_SOLVE_LEVEL_IS_HIGH =							309;

	// Console (programming/tower of Hanoi sub-menu)
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_TOWER_OF_HANOI_EVEN_NUMBER_OF_DISCS =						310;
	protected static final short INTERFACE_CONSOLE_PROGRAMMING_TOWER_OF_HANOI_ODD_NUMBER_OF_DISCS =							311;

	// Console (reasoning/family definition sub-menu)
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_THE_FATHER_AND_ANN_IS_THE_MOTHER =				400;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_THE_FATHER_OF_PAUL_JOE_AND_LAURA =				401;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_THE_MOTHER_OF_PAUL_JOE_AND_LAURA =				402;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_SON_OF_JOHN_AND_ANN =							403;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_SON_OF_JOHN_AND_ANN =							404;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_DAUGHTER_OF_JOHN_AND_ANN =					405;

	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_A_FATHER =										406;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_A_MOTHER =										407;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_SON =											408;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_SON =											409;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_DAUGHTER =									410;

	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_HAS_A_FATHER_CALLED_JOHN =							411;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_HAS_A_MOTHER_CALLED_ANN =							412;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOE_HAS_A_FATHER_CALLED_JOHN =							413;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOE_HAS_A_MOTHER_CALLED_ANN =							414;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_HAS_A_FATHER_CALLED_JOHN =						415;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_HAS_A_MOTHER_CALLED_ANN =							416;

	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_A_PARENT =										417;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_A_PARENT =										418;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_A_PARENT_OF_PAUL_JOE_AND_LAURA =				419;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_A_PARENT_OF_PAUL_JOE_AND_LAURA =					420;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_HAS_2_PARENT_CALLED_JOHN_AND_ANN =					421;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOE_HAS_2_PARENT_CALLED_JOHN_AND_ANN =					422;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_HAS_2_PARENT_CALLED_JOHN_AND_ANN =				423;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_CHILD_OF_JOHN_AND_ANN =						424;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_CHILD_OF_JOHN_AND_ANN =						425;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_CHILD_OF_JOHN_AND_ANN =						426;

	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOHN_IS_A_MAN =											427;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_ANN_IS_A_WOMAN =										428;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_MAN =											429;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_MAN =											430;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_WOMAN =										431;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_CHILD =										432;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_CHILD =										433;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_CHILD =										434;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_PAUL_IS_A_BOY =											435;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_JOE_IS_A_BOY =											436;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_LAURA_IS_A_GIRL =										437;

	// Console (reasoning/family conflict sub-menu)
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_JOHN_IS_A_WOMAN =								440;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_JOHN_IS_THE_MOTHER_PETE =						441;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_ANN_IS_A_SON =									442;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_PAUL_IS_A_DAUGHTER =							443;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_CONFLICT_JOE_IS_A_MOTHER =								444;

	// Console (reasoning/family justification sub-menu)
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_FAMILY =				450;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_PARENT =				451;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_CHILD =				452;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_FATHER =				453;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_MOTHER =				454;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_SON =					455;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_DAUGHTER =				456;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_JOHN =					457;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_ANN =					458;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_PAUL =					459;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_JOE =					460;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_JUSTIFICATION_REPORT_FOR_LAURA =				461;

	// Console (reasoning/family question sub-menu)
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_JOHN_A_FATHER =								470;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_JOHN_A_MOTHER =								471;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_JOHN_THE_FATHER_OF_PAUL =					472;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_JOHN_THE_MOTHER_OF_PAUL =					473;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_MAN =								474;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_WOMAN =								475;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_MAN_OR_A_WOMAN =						476;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_SON =								477;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_DAUGHTER =							478;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_QUESTION_IS_PAUL_A_SON_OR_A_DAUGHTER =					479;

	// Console (reasoning/family display information sub-menu)
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_FAMILY =						480;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_PARENT =						481;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_CHILD =						482;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_FATHER =						483;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_MOTHER =						484;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_SON =							485;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_DAUGHTER =					486;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_JOHN =						487;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_ANN =							488;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_PAUL =						489;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_JOE =							490;
	protected static final short INTERFACE_CONSOLE_REASONING_FAMILY_DISPLAY_INFORMATION_ABOUT_LAURA =						491;

	// Console (font size sub-menu)
	protected static final short INTERFACE_CONSOLE_FONT_SIZE_DECREASE =														500;
	protected static final short INTERFACE_CONSOLE_FONT_SIZE_INCREASE =														501;

	// Console (help sub-menu)
	protected static final short INTERFACE_CONSOLE_HELP_DISPLAY_INFO_ABOUT_THE_LANGUAGES =									510;
	protected static final short INTERFACE_CONSOLE_HELP_DISPLAY_INFO_ABOUT_THE_LISTS =										511;
	protected static final short INTERFACE_CONSOLE_HELP_DISPLAY_INFO_ABOUT_THE_USERS =										512;
	protected static final short INTERFACE_CONSOLE_HELP_DISPLAY_INFO_ABOUT_THE_WORD_TYPES =									513;
	protected static final short INTERFACE_CONSOLE_HELP_DISPLAY_THE_QUERY_COMMANDS =										514;
	protected static final short INTERFACE_CONSOLE_HELP_DISPLAY_THE_COPYRIGHT =												515;
	protected static final short INTERFACE_CONSOLE_HELP_DISPLAY_THE_GPLv2_LICENSE =											516;
	protected static final short INTERFACE_CONSOLE_HELP_DISPLAY_THE_WARRANTY =												517;

	// Console (common)
	protected static final short INTERFACE_CONSOLE_INITIAL_MESSAGE_DEMO_BUTTONS =											1000;
	protected static final short INTERFACE_CONSOLE_INITIAL_MESSAGE_ENTER_SENTENCE =											1001;
	protected static final short INTERFACE_CONSOLE_WAITING_FOR_INPUT =														1002;
	protected static final short INTERFACE_CONSOLE_WAITING_FOR_SECURE_INPUT =												1003;
	protected static final short INTERFACE_CONSOLE_ALREADY_LOGGED_IN_START =												1004;
	protected static final short INTERFACE_CONSOLE_LOGIN_END =																1005;
	protected static final short INTERFACE_CONSOLE_LOGIN_FAILED =															1006;
	protected static final short INTERFACE_CONSOLE_WELCOME_MESSAGE_EXPERT =													1007;

	// Console (status and progress)
	protected static final short INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_START =										1010;
	protected static final short INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_END =											1011;

	// Grammar
	protected static final short INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_START =											1100;
	protected static final short INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_MIDDLE =											1101;
	protected static final short INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_END =												1102;
	protected static final short INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_START =									1103;
	protected static final short INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_MIDDLE =								1104;
	protected static final short INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_END =									1105;
	protected static final short INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_START =										1106;
	protected static final short INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_MIDDLE =									1107;
	protected static final short INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_START =									1108;
	protected static final short INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_TO =										1109;
	protected static final short INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_MIDDLE =									1110;
	protected static final short INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END =										1111;
	protected static final short INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START =												1112;
	protected static final short INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_ENDING =												1113;

	// Imperative (notifications)
	protected static final short INTERFACE_IMPERATIVE_NOTIFICATION_MY_MIND_IS_CLEAR =										1200;
	protected static final short INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_UNDO =									1201;
	protected static final short INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO =									1202;
	protected static final short INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_START =						1203;
	protected static final short INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_END =							1204;
	protected static final short INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START =						1205;
	protected static final short INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END =							1206;
	protected static final short INTERFACE_IMPERATIVE_NOTIFICATION_I_DONT_HAVE_THE_REQUESTED_INFO =							1207;

	// Imperative (warnings)
	protected static final short INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_START =			1210;
	protected static final short INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_END =			1211;
	protected static final short INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START =			1212;
	protected static final short INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END =				1213;
	protected static final short INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_TO_DO_WITH_RELATION =								1214;
	protected static final short INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_WHICH_FILE_TO_READ =								1215;
	protected static final short INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED =								1216;
	protected static final short INTERFACE_IMPERATIVE_WARNING_PREPOSITION_NOT_USED_FOR_THIS_ACTION =						1217;
	protected static final short INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_START =									1218;
	protected static final short INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_END =										1219;

	// Listing (conflicts)
	protected static final short INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_ITSELF =										1300;
	protected static final short INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF =										1301;
	protected static final short INTERFACE_LISTING_CONFLICT =																1302;
	protected static final short INTERFACE_LISTING_SENTENCE_DOESNT_LEAVE_ANY_OPTION_OPEN =									1303;
	protected static final short INTERFACE_LISTING_SENTENCE_DEFINITION_SPECIFICATION_IS_ALREADY_ASSIGNED =					1304;
	protected static final short INTERFACE_LISTING_SENTENCE_ASSIGNMENT_IN_CONFLICT_WITH_DEFINITION_SPECIFICATION =			1305;

	// Listing (current information)
	protected static final short INTERFACE_LISTING_YOUR_INFORMATION =														1310;
	protected static final short INTERFACE_LISTING_YOUR_QUESTIONS =															1311;
	protected static final short INTERFACE_LISTING_MY_INFORMATION =															1312;
	protected static final short INTERFACE_LISTING_MY_CONCLUSIONS =															1313;
	protected static final short INTERFACE_LISTING_MY_ASSUMPTIONS =															1314;
	protected static final short INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_ADJUSTED =										1315;
	protected static final short INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONCLUDED =										1316;
	protected static final short INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_NOT_HIDDEN_ANYMORE =								1317;
	protected static final short INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_NOT_HIDDEN_ANYMORE =								1318;
	protected static final short INTERFACE_LISTING_MY_QUESTIONS =															1319;
	protected static final short INTERFACE_LISTING_MY_ADJUSTED_QUESTIONS =													1320;
	protected static final short INTERFACE_LISTING_MY_ANSWER =																1321;
	protected static final short INTERFACE_LISTING_I_ONLY_KNOW =															1322;
	protected static final short INTERFACE_LISTING_SPECIFICATIONS =															1323;
	protected static final short INTERFACE_LISTING_RELATED_INFORMATION =													1324;
	protected static final short INTERFACE_LISTING_RELATED_QUESTIONS =														1325;

	// Listing (old information)
	protected static final short INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION_BUT_NOT_ITS_RELATION =			1330;
	protected static final short INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION_BUT_NOT_ITS_RELATION =			1331;
	protected static final short INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION =		1332;
	protected static final short INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION =		1333;
	protected static final short INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED =										1334;
	protected static final short INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_CONFIRMED =										1335;
	protected static final short INTERFACE_LISTING_MY_EARLIER_ASSUMPTIONS_THAT_HAVE_RELATION_WORDS_NOW =					1336;
	protected static final short INTERFACE_LISTING_MY_EARLIER_CONCLUSIONS_THAT_HAVE_RELATION_WORDS_NOW =					1337;
	protected static final short INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION =							1338;
	protected static final short INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION =							1339;
	protected static final short INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION =						1340;
	protected static final short INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO =						1341;
	protected static final short INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION =						1342;
	protected static final short INTERFACE_LISTING_YOUR_QUESTIONS_THAT_ARE_ANSWERED =										1343;
	protected static final short INTERFACE_LISTING_MY_QUESTIONS_THAT_ARE_ANSWERED =											1344;
	protected static final short INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_KNOWLEDGE =									1345;
	protected static final short INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_OPPOSITE_QUESTION =							1346;

	// Listing (justification report)
	protected static final short INTERFACE_JUSTIFICATION_BECAUSE =															1350;
	protected static final short INTERFACE_JUSTIFICATION_AND =																1351;
	protected static final short INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NOUN_ENDING =							1352;
	protected static final short INTERFACE_JUSTIFICATION_FEMININE_PROPER_NOUN_ENDING =										1353;
	protected static final short INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NOUN_ENDING =										1354;
	protected static final short INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_START =											1355;
	protected static final short INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_END =											1356;

	// Query
	protected static final short INTERFACE_QUERY_NO_ITEM_WAS_FOUND =														1400;
	protected static final short INTERFACE_QUERY_NO_REFERENCE_ITEM_WAS_FOUND =												1401;
	protected static final short INTERFACE_QUERY_INVALID_CHARACTER_IN_LIST =												1402;
	protected static final short INTERFACE_QUERY_NO_LIST_WAS_FOUND =														1403;
	protected static final short INTERFACE_QUERY_NO_WORD_WAS_FOUND =														1404;
	protected static final short INTERFACE_QUERY_NO_WORD_REFERENCE_WAS_FOUND =												1405;
	protected static final short INTERFACE_QUERY_NO_STRING_WAS_FOUND =														1406;
	protected static final short INTERFACE_QUERY_NO_WORD_TYPE_WAS_FOUND =													1407;
	protected static final short INTERFACE_QUERY_NO_PARAMETER_WAS_FOUND =													1408;

	protected static final short INTERFACE_QUERY_NO_ITEMS_WERE_FOUND =														1409;
	protected static final short INTERFACE_QUERY_NO_WORDS_WERE_FOUND =														1410;
	protected static final short INTERFACE_QUERY_NO_WORD_REFERENCES_WERE_FOUND =											1411;
	protected static final short INTERFACE_QUERY_NO_STRINGS_WERE_FOUND =													1412;

	protected static final short INTERFACE_QUERY_EMPTY_WORD_SPECIFICATION =													1413;
	protected static final short INTERFACE_QUERY_EMPTY_WORD_REFERENCE =														1414;
	protected static final short INTERFACE_QUERY_EMPTY_STRING_SPECIFICATION =												1415;

	protected static final short INTERFACE_QUERY_ERROR =																	1499;

	// Response (questions)
	protected static final short INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE =										1500;
	protected static final short INTERFACE_QUESTION_I_HAD_THE_SAME_QUESTION_BEFORE =										1501;
	protected static final short INTERFACE_QUESTION_YOU_HAD_A_SIMILAR_QUESTION_BEFORE =										1502;
	protected static final short INTERFACE_QUESTION_I_HAD_A_SIMILAR_QUESTION_BEFORE =										1503;
	protected static final short INTERFACE_QUESTION_YOU_HAD_A_RELATED_QUESTION_BEFORE =										1504;
	protected static final short INTERFACE_QUESTION_I_HAD_A_RELATED_QUESTION_BEFORE =										1505;
	protected static final short INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_START =									1506;
	protected static final short INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_END =									1507;
	protected static final short INTERFACE_QUESTION_IS_ALREADY_ANSWERED =													1508;
	protected static final short INTERFACE_QUESTION_I_DONT_KNOW_THE_ANSWER_TO_THIS_QUESTION =								1509;

	// Sentence (notifications)
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NOUN_TO_BE =								1600;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_SINGULAR_NOUN_TO_BE =							1601;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE =										1602;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE =										1603;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_I_KNOW =													1604;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START =						1605;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END =							1606;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END =							1607;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_QUESTION_MISSING_RELATION =						1608;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_MISSING_RELATION_I_ASSUME_YOU_MEAN =						1609;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_WITH_NOUN_START =					1610;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START =			1611;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START =		1612;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END =		1613;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_START =		1614;
	protected static final short INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_END =			1615;

	// Sentence (warnings)
	protected static final short INTERFACE_SENTENCE_WARNING_NOT_CONFORM_GRAMMAR =											1620;
	protected static final short INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_START =						1621;
	protected static final short INTERFACE_SENTENCE_WARNING_DONT_UNDERSTAND_FROM_WORD_POSITION_END =						1622;
	protected static final short INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_START =			1623;
	protected static final short INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_POSITION =		1624;
	protected static final short INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_END =			1625;
	protected static final short INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_START =				1626;
	protected static final short INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_END =				1627;
	protected static final short INTERFACE_SENTENCE_WARNING_NOT_ABLE_TO_LINK_YES_NO_TO_QUESTION =							1628;
	protected static final short INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MISSING_RELATION =							1629;
	protected static final short INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MULTIPLE_SPECIFICATIONS_RELATIONS =			1630;

	// Sentence (errors)
	protected static final short INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE =								1640;
	protected static final short INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE =									1641;
	protected static final short INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE_DUE_TO_WORDS =					1642;
	protected static final short INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_START =								1643;
	protected static final short INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE =										1644;
	protected static final short INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_DUE_TO_WORDS =							1645;
	protected static final short INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_START =				1646;
	protected static final short INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_END =				1647;

	// Chinese constants
	protected static final char SYMBOL_CHINESE_ZERO =											'零';
	protected static final char SYMBOL_CHINESE_ONE =											'一';
	protected static final char SYMBOL_CHINESE_TWO =											'二';
	protected static final char SYMBOL_CHINESE_THREE =											'三';
	protected static final char SYMBOL_CHINESE_FOUR =											'四';
	protected static final char SYMBOL_CHINESE_FIVE =											'五';
	protected static final char SYMBOL_CHINESE_SIX =											'六';
	protected static final char SYMBOL_CHINESE_SEVEN =											'七';
	protected static final char SYMBOL_CHINESE_EIGHT =											'八';
	protected static final char SYMBOL_CHINESE_NINE =											'九';

	protected static final char SYMBOL_CHINESE_DOT =											'·';
	protected static final char SYMBOL_CHINESE_COLON =											'。';
	protected static final char SYMBOL_CHINESE_COMMA =											'、';

	protected static final char SYMBOL_CHINESE_ADJECTIVE_CALLED =								'叫';
	protected static final char SYMBOL_CHINESE_ADJECTIVE_EVERY =								'每';
	protected static final char SYMBOL_CHINESE_CONJUNCTION_AND =								'和';
	protected static final char SYMBOL_CHINESE_NOUN_MAN =										'男';
	protected static final char SYMBOL_CHINESE_NOUN_WOMAN =										'女';
	protected static final char SYMBOL_CHINESE_NUMERAL_ALL =									'都';
	protected static final char SYMBOL_CHINESE_PREPOSITION_OF =									'的';
	protected static final char SYMBOL_CHINESE_QUESTION_MARK =									'？';
	protected static final char SYMBOL_CHINESE_VERB_HAS =										'有';
	};

/*************************************************************************
 *	"He will bless those who fear the Lord,
 *	both great and lowly." (Psalm 115:13)
 *************************************************************************/
