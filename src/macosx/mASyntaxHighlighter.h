/*----------------------------------------------------------------------------
miniAudicle
Cocoa GUI to chuck audio programming environment

Copyright (c) 2005 Spencer Salazar.  All rights reserved.
http://chuck.cs.princeton.edu/
http://soundlab.cs.princeton.edu/

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// file: mASyntaxHighlighter.h
// desc: Syntax highlighting module.  Based heavily on Glenn Andreas' IDEKit,
// but there are modifications.  
//
// author: Spencer Salazar (ssalazar@cs.princeton.edu)
// date: Summer 2006
//-----------------------------------------------------------------------------

typedef unsigned int mALexerState;
enum
{
    MA_LS_DEFAULT,
    MA_LS_NUMBER,
    MA_LS_KEYWORD,
    MA_LS_CLASS,
    MA_LS_FUNCTION,
    MA_LS_OPERATOR,
    MA_LS_STRING,
    MA_LS_COMMENT,
    MA_LS_COMMENT_START,
    MA_LS_COMMENT_END,
    MA_LS_LINECOMMENT,
    MA_LS_LINECOMMENT_START,
    MA_LS_COUNT
};

@interface mASyntaxDefinition : NSObject
{
    NSMutableDictionary * keywords;
    NSMutableDictionary * operators;
    NSMutableDictionary * classes;
    NSMutableDictionary * user1;
        
    struct 
    {
        NSColor * text;
        NSColor * background;
        NSFont * font;
    } attributes[MA_LS_COUNT];
}

+ (mASyntaxDefinition *)masterDefinition;
- (id)init;
- (void)addKeyword:(NSString *)s;
- (void)addOperator:(NSString *)s;
- (void)addClass:(NSString *)s;
- (void)addUser1Keyword:(NSString *)s;

- (void)setStyleForLexerState:(mALexerState)state 
                    textColor:(NSColor *)text
              backgroundColor:(NSColor *)background
                         font:(NSFont *)f;

@end

@interface mASyntaxHighlighter : NSObject<NSTextStorageDelegate>
{
    NSTextStorage * s;
//    NSArray * defs;
    mASyntaxDefinition * def;
    
    NSCharacterSet * idchars;
    NSCharacterSet * fidchars;
    NSCharacterSet * nonidchars;
    NSCharacterSet * whitespace;
}

- (id)initWithTextStorage:(NSTextStorage *)str;
//- (void)pushSyntaxDefinition:(mASyntaxDefinition *)sd;

@end

//
//  IDEKit_UserSettings.h
//
//  Created by Glenn Andreas on Sat Mar 22 2003.
//  Copyright (c) 2003, 2004 by Glenn Andreas
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//  
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

// This is shared between the preferences panel and project
// Everything that is written to a user defaults (or similar thing) is
// declared here.

#import <Foundation/Foundation.h>

extern NSString *IDEKit_TextColorsPrefKey;
extern NSString *IDEKit_TextColorDefaultStateKey;
extern NSString *IDEKit_TextColorDefaultBrowserKey;

extern NSString *IDEKit_TextFontNameKey;
extern NSString *IDEKit_TextFontSizeKey;

extern NSString *IDEKit_TabStopKey;
extern NSString *IDEKit_TabStopUnitKey;
extern NSString *IDEKit_TabSavingKey;
extern NSString *IDEKit_TabSizeKey;
extern NSString *IDEKit_TabIndentSizeKey;
extern NSString *IDEKit_TabAutoConvertKey;
extern NSString *IDEKit_TextAutoCloseKey;


extern NSMutableDictionary *IDEKit_DefaultUserSettings();

extern NSString *IDEKit_UserSettingsChangedNotification;

extern NSString *IDEKit_TemplatesKey;
extern NSString *IDEKit_KeyBindingsKey;

extern NSString *IDEKit_UserPathsKey;

//
//  IDEKit_Parser.h
//  IDEKit
//
//  Created by Glenn Andreas on Mon May 26 2003.
//  Copyright (c) 2003, 2004 by Glenn Andreas
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//  
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#import <Cocoa/Cocoa.h>

extern NSString *IDEKit_LexIDKey;
extern NSString *IDEKit_LexColorKey;

extern NSString *IDEKit_LexParserStartState;
@protocol IDEKit_LexParserColorer
- (NSColor *)colorForIdentifier: (NSString *)ident;
@end

enum {
    IDEKit_kLexKindMask =   0x0fff0000,
    IDEKit_kLexIDMask =     0x0000ffff,
    
    IDEKit_kLexKindOperator =   0x00000000, // so '+' is actaully then '+'
    IDEKit_kLexKindKeyword =    0x00010000, // this includes explicit operators
    IDEKit_kLexKindPrePro =     0x00020000,
    IDEKit_kLexKindSpecial =    0x0fff0000,
    
    IDEKit_kLexEOF = IDEKit_kLexKindSpecial | 0,
    IDEKit_kLexError = IDEKit_kLexKindSpecial | 1,
    IDEKit_kLexWhiteSpace = IDEKit_kLexKindSpecial | 2,
    IDEKit_kLexEOL = IDEKit_kLexKindSpecial | 3,
    IDEKit_kLexComment = IDEKit_kLexKindSpecial | 4,
    //    IDEKit_kLexPreProcessor = -5,
    IDEKit_kLexString = IDEKit_kLexKindSpecial | 6,
    IDEKit_kLexCharacter = IDEKit_kLexKindSpecial | 7,
    IDEKit_kLexNumber = IDEKit_kLexKindSpecial | 8,
    //    IDEKit_kLexOperator = IDEKit_kLexKindSpecial | 9,
    IDEKit_kLexIdentifier = IDEKit_kLexKindSpecial | 10,
    // for markup languages, we treat "content" as differnet than markup
    IDEKit_kLexMarkupStart = IDEKit_kLexKindSpecial | 11,
    IDEKit_kLexMarkupEnd = IDEKit_kLexKindSpecial | 12,
    IDEKit_kLexContent = IDEKit_kLexKindSpecial | 13, // what is like a string between the markup
    IDEKit_kLexToken = IDEKit_kLexKindKeyword, // for a token, we normally return the lexID, unless it is 0, when we return IDEKit_kLexToken
};

@class IDEKit_TokenEnumerator;

@interface IDEKit_LexParser : NSObject {
    NSMutableDictionary *myKeywords;    // both regular and alt
    NSMutableDictionary *myOperators;
    
    NSString *myPreProStart;
    NSMutableArray *myPreProcessor;
    
    NSMutableArray *myStrings;
    NSMutableArray *myCharacters;
    NSMutableArray *myMultiComments;
    NSMutableArray *mySingleComments;
    NSString *myMarkupStart;
    NSString *myMarkupEnd;
    
    NSCharacterSet *myIdentifierChars;
    NSCharacterSet *myFirstIdentifierChars;
    
    unsigned myCurLoc;
    unsigned myStopLoc;
    unsigned myCurState;
    unsigned mySubStart;
    NSString *myCloser;
    int mySubColor;
    int mySubLexID;
    NSString *myString;
    BOOL doneWithToken;
    BOOL myCaseSensitive;
    int myTempBackState;
}
- (void) setCaseSensitive: (BOOL) sensitive;
- (id) addKeyword: (NSString *)string color: (int) color lexID: (int) lexID;
- (id) addOperator: (NSString *)string lexID: (int) lexID;
- (void) addStringStart: (NSString *)start end: (NSString *) end;
- (void) addCharacterStart: (NSString *)start end: (NSString *) end;
- (void) addCommentStart: (NSString *)start end: (NSString *) end;
- (void) addMarkupStart: (NSString *)start end: (NSString *) end;
- (void) addSingleComment: (NSString *)start;
- (void) setIdentifierChars: (NSCharacterSet *)set;
- (void) setFirstIdentifierChars: (NSCharacterSet *)set;
- (void) setPreProStart: (NSString *)start;
- (void) addPreProcessor: (NSString *)token;

- (void) colorString: (NSMutableAttributedString *)string range: (NSRange) range colorer: (id) colorer;
- (void) startParsingString: (NSString *)string range: (NSRange) range;
- (int) parseOneToken: (NSRangePointer) result ignoreWhiteSpace: (BOOL) ignoreWS;
@end

/*
 *  IDEKit_TextColors.h
 *  IDEKit
 *
 *  Created by Glenn Andreas on Wed May 21 2003.
 *  Copyright (c) 2003, 2004 by Glenn Andreas
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *  
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

enum {
    IDEKit_kLangColor_Background = 0,
    IDEKit_kLangColor_NormalText,
    IDEKit_kLangColor_Invisibles,
    IDEKit_kLangColor_Adorners,
    IDEKit_kLangColor_Errors,
    IDEKit_kLangColor_OtherInternal2,   // 5
    IDEKit_kLangColor_OtherInternal3,
    IDEKit_kLangColor_OtherInternal4,
    // first the browser symbol coloring
    IDEKit_kLangColor_Classes,      // 8
    IDEKit_kLangColor_Constants,
    IDEKit_kLangColor_Enums,        // 10
    IDEKit_kLangColor_Functions,
    IDEKit_kLangColor_Globals,
    IDEKit_kLangColor_Macros,
    IDEKit_kLangColor_Templates,
    IDEKit_kLangColor_Typedefs, // 15
    IDEKit_kLangColor_OtherSymbol1,
    IDEKit_kLangColor_OtherSymbol2,
    IDEKit_kLangColor_OtherSymbol3,
    IDEKit_kLangColor_OtherSymbol4,
    // more syntax coloring
    IDEKit_kLangColor_Comments, // 20
    IDEKit_kLangColor_Keywords,
    IDEKit_kLangColor_Strings,
    IDEKit_kLangColor_FieldsBG, // for background completion templates
    IDEKit_kLangColor_Preprocessor,
    IDEKit_kLangColor_AltKeywords,  // 25
    IDEKit_kLangColor_DocKeywords,
    IDEKit_kLangColor_Characters,
    IDEKit_kLangColor_Numbers,
    IDEKit_kLangColor_OtherSyntax6,
    IDEKit_kLangColor_OtherSyntax7, // 30
    IDEKit_kLangColor_OtherSyntax8,
    IDEKit_kLangColor_UserKeyword1, // 32
    IDEKit_kLangColor_UserKeyword2,
    IDEKit_kLangColor_UserKeyword3,
    IDEKit_kLangColor_UserKeyword4,
    IDEKit_kLangColor_End
};
extern NSColor *IDEKit_TextColorForColor(int color);
extern NSString *IDEKit_NameForColor(int color);

@interface NSColor(IDEKit_StringToColors)
+ (NSColor *)colorWithHTML: (NSString *)hex;
+ (NSColor *)colorWithRGB: (NSString *)triplet;
- (NSString *)htmlString;
@end

