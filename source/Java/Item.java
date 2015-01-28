/*
 *	Class:		Item
 *	Purpose:	Base class for the knowledge structure
 *	Version:	Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

class Item
	{
	// Private constructible variables

	private short userNr_;

	private int activeSentenceNr_;
	private int inactiveSentenceNr_;
	private int originalSentenceNr_;
	private int creationSentenceNr_;
	private int archivedSentenceNr_;
	private int replacedSentenceNr_;
	private int deletedSentenceNr_;

	private int itemNr_;

	private char statusChar_;

	// Private loadable variables

	private List myList_;
	private WordItem myWordItem_;
	private String moduleNameString_;

	// Protected constructible variables

	protected boolean isAvailableForRollbackAfterDelete;
	protected boolean isSelectedByQuery;

	protected char previousStatusChar;

	protected Item previousItem;
	protected Item nextItem;


	// Private methods

	private String myWordTypeString( short queryWordTypeNr )
		{
		String wordTypeString = null;

		if( myList_ != null &&
		// Don't show my word string if the item is in an admin list
		!myList_.isAdminList() &&
		myWordItem_ != null &&
		( wordTypeString = myWordItem_.wordTypeString( true, queryWordTypeNr ) ) == null )
			wordTypeString = myWordItem_.anyWordTypeString();

		return wordTypeString;
		}


	// Constructor / deconstructor

	protected Item()
		{
		// Private constructible variables

		userNr_ = Constants.NO_USER_NR;

		originalSentenceNr_ = Constants.NO_SENTENCE_NR;
		creationSentenceNr_ = CommonVariables.currentSentenceNr;

		activeSentenceNr_ = Constants.NO_SENTENCE_NR;
		inactiveSentenceNr_ = Constants.NO_SENTENCE_NR;
		archivedSentenceNr_ = Constants.NO_SENTENCE_NR;
		replacedSentenceNr_ = Constants.NO_SENTENCE_NR;
		deletedSentenceNr_ = Constants.NO_SENTENCE_NR;

		itemNr_ = Constants.NO_ITEM_NR;

		statusChar_ = Constants.QUERY_ACTIVE_CHAR;

		// Private loadable variables

		myList_ = null;
		myWordItem_ = null;
		moduleNameString_ = this.getClass().getName();

		// Protected constructible variables

		isAvailableForRollbackAfterDelete = false;
		isSelectedByQuery = false;
		previousStatusChar = Constants.QUERY_ACTIVE_CHAR;

		nextItem = null;
		previousItem = null;
		}


	// Protected error methods

	protected byte addErrorInItem( int methodLevel, String moduleNameString, String errorString )
		{
		Presentation.showError( Constants.SYMBOL_QUESTION_MARK, ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), null, ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}
	protected byte addErrorInItem( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		Presentation.showError( Constants.SYMBOL_QUESTION_MARK, ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), wordNameString, ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}
	protected byte addErrorInItem( char listChar, int methodLevel, String moduleNameString, String errorString )
		{
		Presentation.showError( listChar, ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), null, ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}

	protected byte addErrorInItem( char listChar, int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		Presentation.showError( listChar, ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), wordNameString, ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}

	protected byte startErrorInItem( int methodLevel, String moduleNameString, String errorString )
		{
		addErrorInItem( ( methodLevel + 1 ), moduleNameString, null, errorString );

		CommonVariables.result = Constants.RESULT_ERROR;

		return Constants.RESULT_ERROR;
		}

	protected byte startErrorInItem( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		addErrorInItem( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );

		CommonVariables.result = Constants.RESULT_ERROR;

		return Constants.RESULT_ERROR;
		}

	protected byte startErrorInItem( char listChar, int methodLevel, String moduleNameString, String errorString )
		{
		addErrorInItem( listChar, ( methodLevel + 1 ), moduleNameString, errorString );

		CommonVariables.result = Constants.RESULT_ERROR;

		return Constants.RESULT_ERROR;
		}

	protected byte startSystemErrorInItem( int methodLevel, String moduleNameString, String errorString )
		{
		return startSystemErrorInItem( ( methodLevel + 1 ), moduleNameString, null, errorString );
		}

	protected byte startSystemErrorInItem( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		char textChar;
		int errorStringPosition = 0;
		StringBuffer tempStringBuffer = new StringBuffer();

		while( errorStringPosition < errorString.length() )
			{
			if( errorString.charAt( errorStringPosition ) == Constants.TEXT_DIACRITICAL_CHAR )
				{
				errorStringPosition++;

				if( errorStringPosition < errorString.length() &&
				( textChar = Presentation.convertDiacriticalChar( errorString.charAt( errorStringPosition ) ) ) != Constants.NEW_LINE_CHAR )
					tempStringBuffer.append( textChar );
				}
			else
				tempStringBuffer.append( errorString.charAt( errorStringPosition ) );

			errorStringPosition++;
			}

		addErrorInItem( ( methodLevel + 1 ), moduleNameString, wordNameString, tempStringBuffer.toString() );

		CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;

		return Constants.RESULT_SYSTEM_ERROR;
		}


	// Protected virtual methods

	protected void showString( boolean isReturnQueryToPosition )
		{
		// This is a virtual method. Therefore the given variables are unreferenced
		}

	protected void showWordReferences( boolean isReturnQueryToPosition )
		{
		// This is a virtual method. Therefore the given variables are unreferenced
		}

	protected boolean isSorted( Item nextSortItem )
		{
		return ( nextSortItem != null &&
				// Descending creationSentenceNr_
				creationSentenceNr_ > nextSortItem.creationSentenceNr_ );
		}

	protected boolean hasFoundParameter( int queryParameter )
		{
		// This is a virtual method. Therefore the given variables are unreferenced
		return false;
		}

	protected boolean hasFoundWordType( short queryWordTypeNr )
		{
		// This is a virtual method. Therefore the given variables are unreferenced
		return false;
		}

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		// This is a virtual method. Therefore the given variables are unreferenced
		return false;
		}

	protected ReferenceResultType findMatchingWordReferenceString( String queryString )
		{
		// This is a virtual method. Therefore the given variables are unreferenced
		return new ReferenceResultType();
		}

	protected byte checkForUsage()
		{
		return Constants.RESULT_OK;
		}

	protected void showWords( boolean isReturnQueryToPosition, short queryWordTypeNr )
		{
		// This is a virtual method. Therefore the given variables are unreferenced
		String myWordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( ( myWordString = myWordTypeString( queryWordTypeNr ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			if( !isActiveItem() )	// Show status if not active
				CommonVariables.queryStringBuffer.append( statusChar_ );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( myWordString );
			}
		}

	protected String itemString()
		{
		return null;
		}

	protected String extraItemString()
		{
		return null;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		// This is a virtual method. Therefore the given variables are unreferenced
		return null;
		}

	protected StringBuffer baseToStringBuffer( short queryWordTypeNr )
		{
		String myWordString = myWordTypeString( queryWordTypeNr );
		String userNameString = ( myWordItem_ == null ? null : myWordItem_.userNameString( userNr_ ) );
		CommonVariables.queryStringBuffer = new StringBuffer( Constants.EMPTY_STRING );

		if( !isActiveItem() )	// Show status if not active
			CommonVariables.queryStringBuffer.append( statusChar_ );

		if( myWordString != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_WORD_START_CHAR + myWordString + Constants.QUERY_WORD_END_CHAR );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_LIST_START_STRING + ( myList_ == null ? Constants.QUERY_NO_LIST_CHAR : myList_.listChar() ) + Constants.QUERY_LIST_END_CHAR );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_ITEM_START_STRING + creationSentenceNr_ + Constants.QUERY_SEPARATOR_CHAR + itemNr_ + Constants.QUERY_ITEM_END_CHAR );

		if( isAvailableForRollbackAfterDelete )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isAvailableForRollbackAfterDelete" );
/*
		if( isSelectedByQuery )		// Don't show: Always true during query
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSelectedByQuery" );
*/
		if( isArchivedItem() ||
		isReplacedItem() )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "previousStatusChar:" + previousStatusChar );

		if( userNr_ > Constants.NO_USER_NR )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "user:" + ( userNameString == null ? userNr_ : userNameString ) );
			}

		if( originalSentenceNr_ > Constants.NO_SENTENCE_NR &&
		originalSentenceNr_ != creationSentenceNr_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "originalSentenceNr:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + originalSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		if( activeSentenceNr_ > Constants.NO_SENTENCE_NR &&
		activeSentenceNr_ != creationSentenceNr_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "activeSentenceNr:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + activeSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		if( inactiveSentenceNr_ > Constants.NO_SENTENCE_NR &&
		inactiveSentenceNr_ != creationSentenceNr_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "inactiveSentence:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + inactiveSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		if( archivedSentenceNr_ > Constants.NO_SENTENCE_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "archivedSentenceNr:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + archivedSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		if( replacedSentenceNr_ > Constants.NO_SENTENCE_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "replacedSentenceNr:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + replacedSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		if( deletedSentenceNr_ > Constants.NO_SENTENCE_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "deletedSentenceNr:" + Constants.QUERY_ITEM_SENTENCE_NR_START_CHAR + deletedSentenceNr_ + Constants.QUERY_ITEM_SENTENCE_NR_END_CHAR );

		return CommonVariables.queryStringBuffer;
		}


	// Protected common methods

	protected void setActiveStatus()
		{
		statusChar_ = Constants.QUERY_ACTIVE_CHAR;
		}

	protected void setInactiveStatus()
		{
		statusChar_ = Constants.QUERY_INACTIVE_CHAR;
		}

	protected void setArchivedStatus()
		{
		statusChar_ = Constants.QUERY_ARCHIVED_CHAR;
		}

	protected void setReplacedStatus()
		{
		statusChar_ = Constants.QUERY_REPLACED_CHAR;
		}

	protected void setDeletedStatus()
		{
		statusChar_ = Constants.QUERY_DELETED_CHAR;
		}

	protected void setActiveSentenceNr()
		{
		if( activeSentenceNr_ == Constants.NO_SENTENCE_NR )
			activeSentenceNr_ = CommonVariables.currentSentenceNr;
		}

	protected void setInactiveSentenceNr()
		{
		if( inactiveSentenceNr_ == Constants.NO_SENTENCE_NR )
			inactiveSentenceNr_ = CommonVariables.currentSentenceNr;
		}

	protected void setArchivedSentenceNr()
		{
		if( archivedSentenceNr_ == Constants.NO_SENTENCE_NR )
			archivedSentenceNr_ = CommonVariables.currentSentenceNr;
		}

	protected void setReplacedSentenceNr()
		{
		if( replacedSentenceNr_ == Constants.NO_SENTENCE_NR )
			replacedSentenceNr_ = CommonVariables.currentSentenceNr;
		}

	protected void setDeletedSentenceNr()
		{
		deletedSentenceNr_ = CommonVariables.currentSentenceNr;
		}

	protected void clearArchivedSentenceNr()
		{
		archivedSentenceNr_ = Constants.NO_SENTENCE_NR;
		}

	protected void clearReplacedSentenceNr()
		{
		replacedSentenceNr_ = Constants.NO_SENTENCE_NR;
		}

	// Strictly for initialization of AdminItem
	protected void initializeItemVariables( WordItem myWordItem )
		{
		// Private constructible variables

//		AdminItem has no constructible variables to be initialized

		// Private loadable variables

//		AdminItem has no myList_;
		myWordItem_ = myWordItem;

		if( myWordItem_ == null )
			startSystemErrorInItem( 1, null, null, "The given my word is undefined" );
		}

	protected void initializeItemVariables( int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, List myList, WordItem myWordItem )
		{
		// Private loadable variables

		myList_ = myList;
		myWordItem_ = myWordItem;

		// Private constructible variables

		userNr_ = CommonVariables.currentUserNr;

		originalSentenceNr_ = ( originalSentenceNr == Constants.NO_SENTENCE_NR ? CommonVariables.currentSentenceNr : originalSentenceNr );
		activeSentenceNr_ = ( originalSentenceNr == Constants.NO_SENTENCE_NR ? CommonVariables.currentSentenceNr : activeSentenceNr );
		inactiveSentenceNr_ = inactiveSentenceNr;
		archivedSentenceNr_ = archivedSentenceNr;
		replacedSentenceNr_ = Constants.NO_SENTENCE_NR;

		itemNr_ = ++CommonVariables.currentItemNr;

		if( myList_ != null )
			{
			if( myWordItem_ == null )
				startSystemErrorInItem( 1, null, null, "The given my word is undefined" );
			}
		else
			startSystemErrorInItem( 1, null, null, "The given my list is undefined" );
		}

	protected boolean hasActiveSentenceNr()
		{
		return ( activeSentenceNr_ > Constants.NO_SENTENCE_NR );
		}

	protected boolean hasInactiveSentenceNr()
		{
		return ( inactiveSentenceNr_ > Constants.NO_SENTENCE_NR );
		}

	protected boolean hasArchivedSentenceNr()
		{
		return ( archivedSentenceNr_ > Constants.NO_SENTENCE_NR );
		}

	protected boolean hasReplacedSentenceNr()
		{
		return ( replacedSentenceNr_ > Constants.NO_SENTENCE_NR );
		}

	protected boolean hasCurrentCreationSentenceNr()
		{
		return ( creationSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentActiveSentenceNr()
		{
		return ( activeSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentInactiveSentenceNr()
		{
		return ( inactiveSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentArchivedSentenceNr()
		{
		return ( archivedSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentReplacedSentenceNr()
		{
		return ( replacedSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean hasCurrentDeletedSentenceNr()
		{
		return ( deletedSentenceNr_ == CommonVariables.currentSentenceNr );
		}

	protected boolean isOlderItem()
		{
		return ( originalSentenceNr_ < CommonVariables.currentSentenceNr );
		}

	protected boolean isActiveItem()
		{
		return ( statusChar_ == Constants.QUERY_ACTIVE_CHAR );
		}

	protected boolean isInactiveItem()
		{
		return ( statusChar_ == Constants.QUERY_INACTIVE_CHAR );
		}

	protected boolean isArchivedItem()
		{
		return ( statusChar_ == Constants.QUERY_ARCHIVED_CHAR );
		}

	protected boolean isReplacedItem()
		{
		return ( statusChar_ == Constants.QUERY_REPLACED_CHAR );
		}

	protected boolean isDeletedItem()
		{
		return ( statusChar_ == Constants.QUERY_DELETED_CHAR );
		}

	protected boolean isReplacedOrDeletedItem()
		{
		return ( isReplacedItem() ||
				isDeletedItem() );
		}

	protected boolean isMoreRecent( Item checkItem )
		{
		return ( checkItem != null &&

				( creationSentenceNr_ > checkItem.creationSentenceNr_ ||

				( creationSentenceNr_ == checkItem.creationSentenceNr_ &&
				itemNr_ > checkItem.itemNr_ ) ) );
		}

	protected boolean wasActiveBefore()
		{
		return ( previousStatusChar == Constants.QUERY_ACTIVE_CHAR );
		}

	protected boolean wasInactiveBefore()
		{
		return ( previousStatusChar == Constants.QUERY_INACTIVE_CHAR );
		}

	protected short userNr()
		{
		return userNr_;
		}

	protected int activeSentenceNr()
		{
		return activeSentenceNr_;
		}

	protected int inactiveSentenceNr()
		{
		return inactiveSentenceNr_;
		}

	protected int originalSentenceNr()
		{
		return originalSentenceNr_;
		}

	protected int creationSentenceNr()
		{
		return creationSentenceNr_;
		}

	protected int archivedSentenceNr()
		{
		return archivedSentenceNr_;
		}

	protected int replacedSentenceNr()
		{
		return replacedSentenceNr_;
		}

	protected int deletedSentenceNr()
		{
		return deletedSentenceNr_;
		}

	protected int itemNr()
		{
		return itemNr_;
		}

	protected byte decrementActiveSentenceNr()
		{
		if( activeSentenceNr_ > Constants.NO_SENTENCE_NR )
			activeSentenceNr_--;
		else
			return startErrorInItem( 1, moduleNameString_, "The active sentence number is too low for a decrement" );

		return Constants.RESULT_OK;
		}

	protected byte decrementInactiveSentenceNr()
		{
		if( inactiveSentenceNr_ > Constants.NO_SENTENCE_NR )
			inactiveSentenceNr_--;
		else
			return startErrorInItem( 1, moduleNameString_, "The inactive sentence number is too low for a decrement" );

		return Constants.RESULT_OK;
		}

	protected byte decrementOriginalSentenceNr()
		{
		if( originalSentenceNr_ > Constants.NO_SENTENCE_NR )
			originalSentenceNr_--;
		else
			return startErrorInItem( 1, moduleNameString_, "The original sentence number is too low for a decrement" );

		return Constants.RESULT_OK;
		}

	protected byte decrementCreationSentenceNr()
		{
		if( creationSentenceNr_ > Constants.NO_SENTENCE_NR )
			creationSentenceNr_--;
		else
			return startErrorInItem( 1, moduleNameString_, "The creation sentence number is too low for a decrement" );

		return Constants.RESULT_OK;
		}

	protected byte decrementArchivedSentenceNr()
		{
		if( archivedSentenceNr_ > Constants.NO_SENTENCE_NR )
			archivedSentenceNr_--;
		else
			return startErrorInItem( 1, moduleNameString_, "The archived sentence number is too low for a decrement" );

		return Constants.RESULT_OK;
		}

	protected byte decrementReplacedSentenceNr()
		{
		if( replacedSentenceNr_ > Constants.NO_SENTENCE_NR )
			replacedSentenceNr_--;
		else
			return startErrorInItem( 1, moduleNameString_, "The replaced sentence number is too low for a decrement" );

		return Constants.RESULT_OK;
		}

	protected byte decrementItemNr( int decrementOffset )
		{
		if( itemNr_ > decrementOffset )
			itemNr_ -= decrementOffset;
		else
			return startErrorInItem( 1, moduleNameString_, "The given decrement offset is higher than the item number itself" );

		return Constants.RESULT_OK;
		}

	protected char statusChar()
		{
		return statusChar_;
		}

	protected List myList()
		{
		return myList_;
		}

	protected WordItem myWordItem()
		{
		return myWordItem_;
		}


	// Protected definition methods

	protected boolean isAssumption( short justificationTypeNr )
		{
		return ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION );
		}

	protected boolean isConclusion( short justificationTypeNr )
		{
		return ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION );
		}

	protected boolean isAdjectiveParameter( short checkParameter )
		{
		return ( checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_CURRENT_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_CURRENT_FEMININE_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE );
//				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_NEXT_NEUTRAL ||
//				checkParameter == Constants.WORD_PARAMETER_ADJECTIVE_NEXT_FEMININE_MASCULINE );
		}

	protected boolean isDefiniteArticleParameter( short checkParameter )
		{
		return ( checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_NEUTRAL ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE );
		}

	protected boolean isIndefiniteArticleParameter( short checkParameter )
		{
		return ( checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE ||

				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ||
				checkParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE );
		}

	protected boolean isFeminineArticleParameter( short articleParameter )
		{
		return ( articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ||

				articleParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE );
		}

	protected boolean isMasculineArticleParameter( short articleParameter )
		{
		return ( articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ||

				articleParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE ||
				articleParameter == Constants.WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE );
		}

	protected boolean isReasoningWordType( short wordTypeNr )
		{
		return ( wordTypeNr == Constants.WORD_TYPE_PROPER_NAME ||
				isSingularOrPluralNoun( wordTypeNr ) );
		}

	protected boolean isSingularOrPluralNoun( short wordTypeNr )
		{
		return ( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isMatchingWordType( short firstWordTypeNr, short secondWordTypeNr )
		{
		return	( firstWordTypeNr == secondWordTypeNr ||

				( isSingularOrPluralNoun( firstWordTypeNr ) &&
				isSingularOrPluralNoun( secondWordTypeNr ) ) );
		}

	protected boolean isStartingWithPhoneticVowel( String textString )
		{
		short index = 0;

		if( textString != null )
			{
			while( index < Constants.NUMBER_OF_PHONETIC_VOWELS )
				{
				if( textString.charAt( 0 ) == Constants.PHONETIC_VOWEL[index] )
					return true;

				index++;
				}
			}

		return false;
		}

	protected short assumptionGrade( short justificationTypeNr )
		{
		switch( justificationTypeNr )
			{
			case Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION:
				return 0;

			default:
				return 1;
			}
		}
	};

/*************************************************************************
 *	"Give thanks to him who made the heavenly lights-
 *		His faithful love endures forever.
 *	the sun to rule the day
 *		His faithful love endures forever.
 *	and the moon and stars to rule the night.
 *		His faithful love endures forever." (Psalm 136:7-9)
 *************************************************************************/
